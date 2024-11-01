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
Caracter* ordenar(Caracter *lista, Caracter *novo);
void liberarLista(Caracter *lista);

Caracter* construirArvoreHuffman(Caracter *lista);
void criarRepresentação(Caracter *no, char *codigoAtual, int profundidade, CodigoHuffman *tabela, int *indice);
char* encontrarCodigo(unsigned char caracter, CodigoHuffman *tabela, int tamanhoTabela) ;
void criarCompactado(char *arquivoOriginal, char *arquivoCompactado, CodigoHuffman *tabela, int tamanhoTabela);
void compactar(char * original, char * novo);

Caracter* pegarTabela(FILE *arquivo) ;
void refazerArvore(Caracter **raiz, unsigned char caracter, const char *codigo) ;
void descompactar(char *arquivoCompactado, char *arquivoDescompactado);




#endif