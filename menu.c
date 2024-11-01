#include <stdio.h>
#include <stdlib.h>
#include "arvore.h"

void limparTerminal() {
    // Comando para limpar o terminal
    #ifdef _WIN32
        system("cls");  // Para Windows
    #else
        system("clear"); // Para Linux e macOS
    #endif
}

void exibirMenu() {
    limparTerminal();
    printf("=============================\n");
    printf("=       Huffman Tree        =\n");
    printf("=============================\n");
    printf("= 1. Compactar Arquivo      =\n");
    printf("= 2. Descompactar Arquivo   =\n");
    printf("= 3. Sair                   =\n");
    printf("=============================\n\n");
}

void executarOpcao(int opcao, CodigoHuffman *tabela, int tamanhoTabela) {
    char * arquivoOriginal =  malloc (10000 * sizeof(char));
    char * arquivoCompactado = malloc (10000 * sizeof(char)); 
    
    switch (opcao) {
        case 1: 
            printf("Digite o nome do arquivo original: ");
            scanf("%[^\n]%*c", arquivoOriginal);
            printf("Digite o nome do arquivo compactado: ");
            scanf("%[^\n]%*c", arquivoCompactado);
            compactar("Exemplo.txt","Exemplo.bin");

            printf("\nArquivo compactado com sucesso!\n Aperte enter para continuar");

            scanf("%*c");

            free(arquivoCompactado);
            free(arquivoOriginal);
            
            break;

        case 2:  // Descompactar Arquivo
            printf("Digite o nome do arquivo compactado: ");
            scanf("%[^\n]%*c", arquivoCompactado);
            printf("Digite o nome do arquivo descompactado: ");
            scanf("%[^\n]%*c", arquivoOriginal);
            
            descompactarArquivo("Exemplo.bin","Exemplo2.txt");
            printf("\nArquivo descompactado com sucesso!\n Aperte enter para continuar");

            scanf("%*c");

            free(arquivoCompactado);
            free(arquivoOriginal);
            break;

        case 3:  // Sair
            printf("Saindo do programa...\n");
            break;

        default:
            printf("Opção inválida! Por favor, escolha novamente.\n");
            break;
    }
}