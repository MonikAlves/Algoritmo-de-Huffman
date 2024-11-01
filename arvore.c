#include <stdio.h>
#include "arvore.h"
#include <stdlib.h>
#include <string.h>

Caracter* novoCaracter(unsigned char caracter,int quantidade) {
    Caracter *novo = (Caracter*)malloc(sizeof(Caracter));
    if (novo == NULL) {
        printf("Erro ao alocar memória.\n");
        exit(1);
    }
    novo->caracter = caracter;
    novo->quantidade = quantidade;
    novo->proximo = NULL;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

Caracter* construirArvoreHuffman(Caracter *lista) {
    while (lista != NULL && lista->proximo != NULL) {
        Caracter *primeiro = lista;
        Caracter *segundo = lista->proximo;
        lista = segundo->proximo;

        Caracter *novoNo = novoCaracter('\0', primeiro->quantidade + segundo->quantidade);
        novoNo->esquerda = primeiro;
        novoNo->direita = segundo;

        lista = ordenar(lista, novoNo);
    }

    return lista; 
}

Caracter* adicionarCaracter(Caracter *lista, unsigned char caracter) {
    Caracter *atual = lista;
    Caracter *anterior = NULL;

    while (atual != NULL) {
        if(atual->caracter == caracter) break;
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual != NULL) {
        atual->quantidade++;
        if (anterior != NULL) anterior->proximo = atual->proximo;
        else lista = atual->proximo;
    } else {
        atual = novoCaracter(caracter, 1);
    }

    return ordenar(lista, atual);
}

Caracter* ordenar(Caracter *lista, Caracter *novo) {
    if (lista == NULL || lista->quantidade >= novo->quantidade) {
        novo->proximo = lista;
        return novo;
    }

    Caracter *atual = lista;
    while (atual->proximo != NULL) {
        if(atual->proximo->quantidade >= novo->quantidade) break;
        atual = atual->proximo;
    }
    novo->proximo = atual->proximo;
    atual->proximo = novo;
    
    return lista;
}

void criarRepresentação(Caracter *no, char *codigoAtual, int profundidade, CodigoHuffman *tabela, int *indice) {
    if (no->esquerda == NULL && no->direita == NULL) {
        codigoAtual[profundidade] = '\0';
        tabela[*indice].caracter = no->caracter;
        strcpy(tabela[*indice].codigo, codigoAtual);
        (*indice)++;
        return;
    }

    if (no->esquerda) {
        codigoAtual[profundidade] = '0';
        criarRepresentação(no->esquerda, codigoAtual, profundidade + 1, tabela, indice);
    }

    if (no->direita) {
        codigoAtual[profundidade] = '1';
        criarRepresentação(no->direita, codigoAtual, profundidade + 1, tabela, indice);
    }
}

char* encontrarCodigo(unsigned char caracter, CodigoHuffman *tabela, int tamanhoTabela) {
        for (int i = 0; i < tamanhoTabela; i++) {
            if (tabela[i].caracter == caracter) {
                return tabela[i].codigo;
            }
        }
        return NULL;
}

void criarCompactado(char *arquivoOriginal, char *arquivoCompactado, CodigoHuffman *tabela, int tamanhoTabela) {
    FILE *entrada = fopen(arquivoOriginal, "r");
    FILE *saida = fopen(arquivoCompactado, "wb");

    if (!entrada || !saida) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    fwrite(&tamanhoTabela, sizeof(int), 1, saida);

    for (int i = 0; i < tamanhoTabela; i++) {
        fputc(tabela[i].caracter, saida);
        int comprimentoCodigo = strlen(tabela[i].codigo);
        fputc(comprimentoCodigo, saida);
        fwrite(tabela[i].codigo, sizeof(char), comprimentoCodigo, saida);
    }

    unsigned char byte = 0;
    int bits = 0;

    int caracter = fgetc(entrada);
    while (caracter != EOF) {
        char *codigo = encontrarCodigo((unsigned char)caracter, tabela, tamanhoTabela);
        
        for (int i = 0; codigo[i] != '\0'; i++) {

            if (codigo[i] == '1') {
                byte |= (1 << (7 - bits));
            }
            bits++;
             printf("Bit: %d\n", codigo[i] - '0'); // Converte '0'/'1' para 0/1
        printf("Byte atual: %02X, Bits utilizados: %d\n", byte, bits);

            if (bits == 8) { 
                fputc(byte, saida);
                byte = 0;
                bits = 0;
            }
        }

        caracter = fgetc(entrada);
    }

    if (bits > 0) {
        fputc(byte, saida);
    }

    fclose(entrada);
    fclose(saida);
}

void refazerArvore(Caracter **raiz, unsigned char caracter, const char *codigo) {
    Caracter *atual = *raiz;

    for (int i = 0; codigo[i] != '\0'; i++) {
        if (codigo[i] == '0') {
            if (atual->esquerda == NULL) atual->esquerda = novoCaracter('\0', 0);
            atual = atual->esquerda;
        } else {
            if (atual->direita == NULL) atual->direita = novoCaracter('\0', 0);
            atual = atual->direita;
        }
    }
    atual->caracter = caracter;
}

Caracter* pegarTabela(FILE *arquivo) {
    int tamanhoTabela;
    fread(&tamanhoTabela, sizeof(int), 1, arquivo);

    Caracter *raiz = novoCaracter('\0', 0);

    for (int i = 0; i < tamanhoTabela; i++) {
        unsigned char caracter = fgetc(arquivo);
        int comprimentoCodigo = fgetc(arquivo);
        char codigo[comprimentoCodigo + 1];
        fread(codigo, sizeof(char), comprimentoCodigo, arquivo);
        codigo[comprimentoCodigo] = '\0';
        refazerArvore(&raiz, caracter, codigo);
    }

    return raiz;
}

void descompactar(char *arquivoCompactado, char *arquivoDescompactado) {
    char * nomeCompactado = malloc(1000 * sizeof(char));
    sprintf(nomeCompactado,"Arquivos_Compactados/%s.bin",arquivoCompactado);
    char* nomeDescompactado = malloc(1000 * sizeof(char));
    sprintf(nomeDescompactado,"Arquivos_Descompactados/%s",arquivoDescompactado);
    FILE *entrada = fopen(nomeCompactado, "rb");
    FILE *saida = fopen(nomeDescompactado, "wb");

    if (!entrada) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    Caracter *raiz = pegarTabela(entrada);

    unsigned char byte = 0;
    Caracter *atual = raiz;

    while (fread(&byte, sizeof(unsigned char), 1, entrada) == 1) {
        for (int i = 0; i < 8; i++) {
            int bit = (byte >> (7 - i)) & 1;
            printf("Bit: %d\n", bit);
            if (bit == 0) atual = atual->esquerda;
            else atual = atual->direita;

            if (atual->esquerda == NULL && atual->direita == NULL) {
                fputc(atual->caracter, saida);
                printf("Caractere escrito: %c\n", atual->caracter);
                atual = raiz;
            }
        }
    }

    printf("\nArquivo descompactado com sucesso!");

    free(nomeCompactado);
    free(nomeDescompactado);

    fclose(entrada);
    fclose(saida);
}

void compactar(char * original, char * novo){
    FILE *arquivo;
    int caracter;
    Caracter *lista = NULL;
    char * nomeOriginal = malloc(1000 * sizeof(char));
    sprintf(nomeOriginal,"Arquivos_Originais/%s",original);
    char* nomeNovo = malloc(1000 * sizeof(char));
    sprintf(nomeNovo,"Arquivos_Compactados/%s.bin",novo);

    arquivo = fopen(nomeOriginal, "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    while ((caracter = fgetc(arquivo)) != EOF) {
        lista = adicionarCaracter(lista, (unsigned char)caracter);
    }

    fclose(arquivo);
    
    Caracter *raiz = construirArvoreHuffman(lista);
    CodigoHuffman tabela[256];
    char codigoAtual[256];
    int indice = 0;
    criarRepresentação(raiz, codigoAtual, 0, tabela, &indice);
    criarCompactado(nomeOriginal,nomeNovo,tabela,indice);
    
    printf("\nArquivo compactado com sucesso!\n\n");
    
    liberarLista(lista);
    lista = NULL;
    liberarLista(raiz);
    raiz = NULL;
    free(nomeNovo);
    free(nomeOriginal);
}


void liberarLista(Caracter *lista) {
    Caracter *atual = lista;
    while (atual != NULL) {
        Caracter *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}