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
    printf("=          Huffman          =\n");
    printf("=============================\n");
    printf("= 1. Compactar Arquivo      =\n");
    printf("= 2. Descompactar Arquivo   =\n");
    printf("= 3. Sair                   =\n");
    printf("=============================\n\n");
}

long tamanhoArquivo(char * caminho){
 FILE *arquivo = fopen(caminho, "rb");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo: %s\n", caminho);
        return -1;
    }
    
    fseek(arquivo, 0, SEEK_END);
    long tamanho = ftell(arquivo);
    fclose(arquivo);
    return tamanho;
}

void compararTamanhoArquivos(char *arquivo1, char *arquivo2){
    char * caminho1 = malloc(1000 * sizeof(char));
    char * caminho2 = malloc(1000 * sizeof(char));
    sprintf(caminho1,"Arquivos_Originais/%s.txt",arquivo1);
    sprintf(caminho2,"Arquivos_Compactados/%s.huff",arquivo2);
    long tamanho1 = tamanhoArquivo(caminho1);
    long tamanho2 = tamanhoArquivo(caminho2);

    if (tamanho1 != -1 && tamanho2 != -1) {
        double tamanho1KB = (double)tamanho1 / 1024;
        double tamanho2KB = (double)tamanho2 / 1024;
        double diferencaKB = tamanho1KB - tamanho2KB;
        double diferenca = tamanho1 - tamanho2;

        printf("Tamanho do %s: %.2f KB (%.0f bytes)\n", arquivo1, tamanho1KB, (double)tamanho1);
        printf("Tamanho do %s: %.2f KB (%.0f bytes)\n", arquivo2, tamanho2KB, (double)tamanho2);
        printf("Diferença de tamanho: %.2f KB (%.0f bytes)\n", diferencaKB,diferenca);
    }

    free(caminho1);
    free(caminho2);
}

void executarOpcao(int opcao) {
    char * arquivoOriginal =  malloc (1000 * sizeof(char));
    char * arquivoCompactado = malloc (1000 * sizeof(char)); 
    
    switch (opcao) {
        case 1: 
            printf("Digite o nome do arquivo original: ");
            scanf("%[^\n]%*c", arquivoOriginal);
            printf("Digite o nome do arquivo compactado: ");
            scanf("%[^\n]%*c", arquivoCompactado);
            compactar(arquivoOriginal,arquivoCompactado);

            compararTamanhoArquivos(arquivoOriginal,arquivoCompactado);
            printf("\n Aperte enter para continuar");

            scanf("%*c");

            free(arquivoCompactado);
            free(arquivoOriginal);
            
            break;

        case 2:  // Descompactar Arquivo
            printf("Digite o nome do arquivo compactado: ");
            scanf("%[^\n]%*c", arquivoCompactado);
            printf("Digite o nome do arquivo descompactado: ");
            scanf("%[^\n]%*c", arquivoOriginal);
            
            descompactar(arquivoCompactado,arquivoOriginal);
            printf("\n Aperte enter para continuar");

            scanf("%*c");

            free(arquivoCompactado);
            free(arquivoOriginal);
            break;

        case 3:  // Sair
            printf("Saindo do programa...\n");
            free(arquivoCompactado);
            free(arquivoOriginal);
            break;

        default:
            printf("Opção inválida! Por favor, escolha novamente.\n");
            break;
    }
}