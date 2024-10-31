#include <stdio.h>
#include <stdlib.h>

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

Caracter* inserirOrdenado(Caracter *lista, unsigned char caracter) {
    Caracter *atual = lista;
    Caracter *anterior = NULL;

    while (atual != NULL && atual->caracter != caracter) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual != NULL) {
        atual->quantidade++;
        
        if (anterior != NULL) {
            anterior->proximo = atual->proximo;
        } else {
            lista = atual->proximo;
        }
    } else {
        atual = novoCaracter(caracter,1);
    }

    if (lista == NULL || lista->quantidade >= atual->quantidade) {
        atual->proximo = lista;
        lista = atual;
    } else {
        Caracter *aux = lista;
        while (aux->proximo != NULL && aux->proximo->quantidade < atual->quantidade) {
            aux = aux->proximo;
        }
        atual->proximo = aux->proximo;
        aux->proximo = atual;
    }

    return lista;
}

Caracter* construirArvore(Caracter *lista) {
    while (lista != NULL && lista->proximo != NULL) {
        Caracter *primeiro = lista;
        Caracter *segundo = lista->proximo;
        
        lista = segundo->proximo;

        Caracter *novoNo = novoCaracter("", primeiro->quantidade + segundo->quantidade);
        novoNo->esquerda = primeiro;
        novoNo->direita = segundo;

        lista = inserirOrdenado(lista, novoNo->caracter);
    }
    
    return lista;
}

// Função de DFS para exibir a árvore com "0" para esquerda e "1" para direita
void exibirDFS(Caracter *raiz, char *caminho, int profundidade) {
    if (raiz == NULL) return;
    printf("normal:%d esquerda:%d direita:%d ",raiz->quantidade,raiz->esquerda->quantidade,raiz->direita->quantidade);
    // Se o nó é uma folha, exiba o caractere e o caminho percorrido
    if (raiz->esquerda == NULL && raiz->direita == NULL) {
        caminho[profundidade] = '\0'; // Finaliza a string de caminho
        printf("Caractere: %c (ASCII %d) | Quantidade: %d | Caminho: %s\n", raiz->caracter, raiz->caracter, raiz->quantidade, caminho);
    } else {
        // Se é um nó intermediário, mostre sua quantidade e o caminho
        caminho[profundidade] = '0';
        exibirDFS(raiz->esquerda, caminho, profundidade + 1);

        caminho[profundidade] = '1';
        exibirDFS(raiz->direita, caminho, profundidade + 1);
    }
}


void exibirLista(Caracter *lista) {
    printf("Exibindo lista de caracteres:\n");
    while (lista != NULL) {
        printf("Caracter: %c (ASCII %d) | Quantidade: %d\n", lista->caracter, lista->caracter, lista->quantidade);
        lista = lista->proximo;
    }
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
        lista = inserirOrdenado(lista, (unsigned char)caracter);
    }

    fclose(arquivo);

    printf("Lista de caracteres e quantidades:\n");
    exibirLista(lista);
    construirArvore(lista);
    char caminho[100]; // Supondo que a profundidade máxima seja 100
    printf("Estrutura da árvore binária (DFS):\n");
    exibirDFS(lista, caminho, 0);

    // Liberar a memória da lista
    liberarLista(lista);

    return 0;
}
