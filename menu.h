#ifndef __MENU_H__
#define __MENU_H__

#include "arvore.h"

void limparTerminal();
void exibirMenu();
void executarOpcao(int opcao, CodigoHuffman *tabela, int tamanhoTabela);

#endif