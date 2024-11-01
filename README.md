# Sistema de Compressão com Algoritmo de Huffman
[Descrição](#descrição) | [Funcionalidades](#funcionalidades) | [Instruções para Uso](#instruções-para-uso)

## Descrição
Este projeto implementa o algoritmo de compressão de Huffman para arquivos de texto (.txt), permitindo compactar e descompactar arquivos eficientemente. Ele foi desenvolvido com foco na compressão de textos, uma vez que esse método não é tão eficiente para imagens ou outros tipos de arquivos binários.

## Funcionalidades
- **Compactação de Arquivos de Texto**  
  - Permite a compressão de arquivos de texto armazenados na pasta `Arquivos_Originais`, transformando-os em um formato binário mais compacto. Os arquivos comprimidos são salvos automaticamente na pasta `Arquivos_Compactados`.

- **Descompactação de Arquivos de Texto**  
  - O sistema permite descompactar os arquivos binários previamente comprimidos. Os arquivos descompactados são restaurados e salvos na pasta `Arquivos_Descompactados`.
 
- **Exemplos de Texto**  
  - Na pasta `Arquivos_Originais`, você encontrará três exemplos de arquivos de texto (pequeno, médio e grande), que podem ser usados para testar as funcionalidades de compactação e descompactação do sistema.

## Instruções para Uso
1. **Preparação dos Arquivos**  
   - Coloque os arquivos de texto (.txt) que deseja comprimir na pasta `Arquivos_Originais`.
   
2. **Compactação**  
   - Ao escolher a opção de compactar, digite apenas o nome do arquivo, sem a extensão. O sistema localizará automaticamente o arquivo na pasta `Arquivos_Originais` e o comprimirá para a pasta `Arquivos_Compactados`.

3. **Descompactação**  
   - Para descompactar, basta fornecer o nome do arquivo comprimido (sem a extensão) quando solicitado. O sistema buscará o arquivo na pasta `Arquivos_Compactados` e salvará o conteúdo descompactado na pasta `Arquivos_Descompactados`.

### Como Compilar
Para compilar o programa, use o seguinte comando no terminal:
```bash
gcc main.c Bibliotecas/*.c -I"Bibliotecas" -o Huffman
```
Esse comando compila os arquivos principais e gera o executável Huffman.

Como Executar
Após compilar, execute o programa com o seguinte comando:
```bash
./Huffman
```
Isso iniciará o menu interativo, onde você poderá escolher as opções de compactar ou descompactar um arquivo.
