#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arvore.h"
#include "menu.h"

int main() {
    int opcao;

    while (opcao != 3) {
        exibirMenu();
        printf("Escolha uma opção: ");
        scanf("%d%*c", &opcao);
        executarOpcao(opcao);
    }

    return 0;
}