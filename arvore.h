#ifndef __ARVORE_H__
#define __ARVORE_H__

#include <stdio.h>

typedef struct caracter {
    unsigned char caracter;
    int quantidade;
    struct caracter *proximo;
    struct caracter * esquerda;
    struct caracter * direita;
} Caracter;

typedef struct codigoHuffman {
    unsigned char caracter;
    char codigo[256];
} CodigoHuffman;

Caracter* novoCaracter(unsigned char caracter,int quantidade);
Caracter* adicionarCaracter(Caracter *lista, unsigned char caracter) ;
Caracter* inserirOrdenado(Caracter *lista, Caracter *novo);
void liberarLista(Caracter *lista);

Caracter* construirArvoreHuffman(Caracter *lista);
void gerarCodigos(Caracter *no, char *codigoAtual, int profundidade, CodigoHuffman *tabela, int *indice);
void reconstruirArvoreDeTabela(Caracter **raiz, unsigned char caracter, const char *codigo);
Caracter* lerTabelaEReconstruirArvore(FILE *arquivo);

void descompactarArquivo(char *arquivoCompactado, char *arquivoDescompactado);
void escreverArquivoCompactado(char *arquivoOriginal, char *arquivoCompactado, CodigoHuffman *tabela, int tamanhoTabela);
void compactar(char * original, char * novo);

#endif