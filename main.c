#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "menu.h"

int main() {
    CodigoHuffman tabela[256];
    int tamanhoTabela = 0;
    int opcao;

    do {
        exibirMenu();
        printf("Escolha uma opção: ");
        scanf("%d%*c", &opcao);
        executarOpcao(opcao, tabela, tamanhoTabela);
    } while (opcao != 3);

    return 0;
}