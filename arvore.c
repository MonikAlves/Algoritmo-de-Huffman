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

        lista = inserirOrdenado(lista, novoNo);
    }

    return lista; 
}

Caracter* adicionarCaracter(Caracter *lista, unsigned char caracter) {
    Caracter *atual = lista, *anterior = NULL;

    while (atual != NULL && atual->caracter != caracter) {
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

    return inserirOrdenado(lista, atual);
}

Caracter* inserirOrdenado(Caracter *lista, Caracter *novo) {
    if (lista == NULL || lista->quantidade >= novo->quantidade) {
        novo->proximo = lista;
        return novo;
    }

    Caracter *atual = lista;
    while (atual->proximo != NULL && atual->proximo->quantidade < novo->quantidade) {
        atual = atual->proximo;
    }
    novo->proximo = atual->proximo;
    atual->proximo = novo;
    
    return lista;
}

void gerarCodigos(Caracter *no, char *codigoAtual, int profundidade, CodigoHuffman *tabela, int *indice) {
    if (no->esquerda == NULL && no->direita == NULL) {
        codigoAtual[profundidade] = '\0';
        tabela[*indice].caracter = no->caracter;
        strcpy(tabela[*indice].codigo, codigoAtual);
        (*indice)++;
        return;
    }

    if (no->esquerda) {
        codigoAtual[profundidade] = '0';
        gerarCodigos(no->esquerda, codigoAtual, profundidade + 1, tabela, indice);
    }

    if (no->direita) {
        codigoAtual[profundidade] = '1';
        gerarCodigos(no->direita, codigoAtual, profundidade + 1, tabela, indice);
    }
}
void reconstruirArvoreDeTabela(Caracter **raiz, unsigned char caracter, const char *codigo) {
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

Caracter* lerTabelaEReconstruirArvore(FILE *arquivo) {
    int tamanhoTabela;
    fread(&tamanhoTabela, sizeof(int), 1, arquivo);

    Caracter *raiz = novoCaracter('\0', 0);
    for (int i = 0; i < tamanhoTabela; i++) {
        unsigned char caracter = fgetc(arquivo);
        int comprimentoCodigo = fgetc(arquivo);
        char codigo[comprimentoCodigo + 1];
        fread(codigo, sizeof(char), comprimentoCodigo, arquivo);
        codigo[comprimentoCodigo] = '\0';
        reconstruirArvoreDeTabela(&raiz, caracter, codigo);
    }

    return raiz;
}


void liberarLista(Caracter *lista) {
    Caracter *atual = lista;
    while (atual != NULL) {
        Caracter *proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
}





void descompactarArquivo(char *arquivoCompactado, char *arquivoDescompactado) {
    FILE *entrada = fopen(arquivoCompactado, "rb");
    FILE *saida = fopen(arquivoDescompactado, "wb");

    if (!entrada || !saida) {
        printf("Erro ao abrir arquivos.\n");
        return;
    }

    Caracter *raiz = lerTabelaEReconstruirArvore(entrada);

    unsigned char buffer = 0;
    int contadorBits = 0;
    Caracter *atual = raiz;

    while (fread(&buffer, sizeof(unsigned char), 1, entrada) == 1) {
        for (int i = 0; i < 8; i++) {
            int bit = (buffer >> (7 - i)) & 1;

            if (bit == 0) atual = atual->esquerda;
            else atual = atual->direita;

            if (atual->esquerda == NULL && atual->direita == NULL) {
                fputc(atual->caracter, saida);
                atual = raiz;
            }
        }
    }

    fclose(entrada);
    fclose(saida);
}

    // Função para encontrar o código de um caractere na tabela
char* encontrarCodigo(unsigned char caracter, CodigoHuffman *tabela, int tamanhoTabela) {
        for (int i = 0; i < tamanhoTabela; i++) {
            if (tabela[i].caracter == caracter) {
                return tabela[i].codigo;
            }
        }
        return NULL;
}

void escreverArquivoCompactado(char *arquivoOriginal, char *arquivoCompactado, CodigoHuffman *tabela, int tamanhoTabela) {
    FILE *entrada = fopen(arquivoOriginal, "r");
    FILE *saida = fopen(arquivoCompactado, "wb");

    if (!entrada || !saida) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }

    // Escreve o tamanho da tabela para que seja mais fácil reconstruir na descompactação
    fwrite(&tamanhoTabela, sizeof(int), 1, saida);

    // Escreve cada caractere e seu código de Huffman na tabela
    for (int i = 0; i < tamanhoTabela; i++) {
        fputc(tabela[i].caracter, saida);
        int comprimentoCodigo = strlen(tabela[i].codigo);
        fputc(comprimentoCodigo, saida);
        fwrite(tabela[i].codigo, sizeof(char), comprimentoCodigo, saida);
    }

    unsigned char buffer = 0;
    int contadorBits = 0;

    // Codificação do conteúdo do arquivo original
    int caracter = fgetc(entrada);
    while (caracter != EOF) {
        char *codigo = encontrarCodigo((unsigned char)caracter, tabela, tamanhoTabela);
        
        for (int i = 0; codigo[i] != '\0'; i++) {
            if (codigo[i] == '1') {
                buffer |= (1 << (7 - contadorBits)); // Adiciona o bit 1
            }
            contadorBits++;

            if (contadorBits == 8) {  // Escreve um byte completo no arquivo de saída
                fputc(buffer, saida);
                buffer = 0;
                contadorBits = 0;
            }
        }

        caracter = fgetc(entrada);
    }

    // Grava o restante do buffer se houver bits não escritos
    if (contadorBits > 0) {
        fputc(buffer, saida);
    }

    fclose(entrada);
    fclose(saida);
}

void compactar(char * original, char * novo){
    FILE *arquivo;
    int caracter;
    Caracter *lista = NULL;

    arquivo = fopen("Exemplo.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    // Ler e processar cada caractere do arquivo
    while ((caracter = fgetc(arquivo)) != EOF) {
        lista = adicionarCaracter(lista, (unsigned char)caracter);
    }

    fclose(arquivo);
    
    Caracter *raiz = construirArvoreHuffman(lista);
     CodigoHuffman tabela[256];  // Tabela para armazenar os códigos (assumindo 256 caracteres únicos)
    char codigoAtual[256];
    int indice = 0;
    gerarCodigos(raiz, codigoAtual, 0, tabela, &indice);

    // Escrever arquivo compactado
    escreverArquivoCompactado(original,novo,tabela,indice);


    liberarLista(lista);
}