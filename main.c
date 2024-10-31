#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct caracter {
    unsigned char caracter;
    int quantidade;
    struct caracter *proximo;
    struct caracter * esquerda;
    struct caracter * direita;
} Caracter;

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

Caracter* atualizarOuAdicionarCaracter(Caracter *lista, unsigned char caracter) {
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

void exibirLista(Caracter *lista) {
    printf("Exibindo lista de caracteres:\n");
    while (lista != NULL) {
        printf("Caracter: %c (ASCII %d) | Quantidade: %d\n", lista->caracter, lista->caracter, lista->quantidade);
        lista = lista->proximo;
    }
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
        exibirLista(lista);
    }

    return lista; 
}


void liberarLista(Caracter *lista) {
    Caracter *atual = lista;
    while (atual != NULL) {
        Caracter *proximo = atual->proximo;
        printf("Liberando caractere '%c' com quantidade %d\n", atual->caracter, atual->quantidade);
        free(atual);
        atual = proximo;
    }
}

int main() {
    FILE *arquivo;
    int caracter;
    Caracter *lista = NULL;

    arquivo = fopen("Arquivos_Original/Exemplo.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Ler e processar cada caractere do arquivo
    while ((caracter = fgetc(arquivo)) != EOF) {
        lista = atualizarOuAdicionarCaracter(lista, (unsigned char)caracter);
    }

    fclose(arquivo);

    printf("Lista de caracteres e quantidades:\n");
    exibirLista(lista);
    
    Caracter *raiz = construirArvoreHuffman(lista);

    liberarLista(lista);

    return 0;
}
