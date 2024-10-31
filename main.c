#include <stdio.h>
#include <stdlib.h>

typedef struct caracter {
    unsigned char caracter;
    int quantidade;
    struct caracter *proximo;
} Caracter;

Caracter* novoCaracter(unsigned char caracter) {
    Caracter *novo = (Caracter*)malloc(sizeof(Caracter));
    novo->caracter = caracter;
    novo->quantidade = 1;
    novo->proximo = NULL;
    return novo;
}

Caracter* inserirOrdenado(Caracter *lista, unsigned char caracter) {
    Caracter *atual = lista;
    Caracter *anterior = NULL;

    // Procurar se o caracter já está na lista
    while (atual != NULL && atual->caracter != caracter) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual != NULL) {
        // Caracter já está na lista, incrementa a quantidade
        atual->quantidade++;
    } else {
        // Novo caracter, cria e insere na lista ordenada
        Caracter *novo = novoCaracter(caracter);
        
        if (lista == NULL || lista->quantidade >= novo->quantidade) {
            novo->proximo = lista;
            lista = novo;
        } else {
            atual = lista;
            while (atual->proximo != NULL && atual->proximo->quantidade < novo->quantidade) {
                atual = atual->proximo;
            }
            novo->proximo = atual->proximo;
            atual->proximo = novo;
        }
    }
    
    return lista;
}

void exibirLista(Caracter *lista) {
    while (lista != NULL) {
        printf("Caracter: %c (ASCII %d) | Quantidade: %d\n", lista->caracter, lista->caracter, lista->quantidade);
        lista = lista->proximo;
    }
}

int main() {
    FILE *arquivo;
    unsigned char caracter;
    Caracter *lista = NULL;

    arquivo = fopen("Arquivos_Original/Exemplo.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    caracter = fgetc(arquivo);
    while (caracter != EOF) {
        lista = inserirOrdenado(lista, caracter);
        caracter = fgetc(arquivo);
    }

    fclose(arquivo);

    printf("Lista de caracters e quantidades:\n");
    exibirLista(lista);

    return 0;
}
