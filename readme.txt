Este é um projeto de compressor LZW (Lempel-Ziv-Welch) implementado em C++. 
O LZW é um algoritmo de compressão de dados sem perdas que é frequentemente 
utilizado em aplicações de compactação de arquivos. 
Este projeto foi criado com o propósito educacional de 
entender os princípios básicos por trás do algoritmo LZW 
e como ele pode ser implementado em C++.

#Funcionalidades
Comprime dados utilizando o algoritmo LZW com 4 tipos de tamanho de dicionário possíveis
e 3 versões diferentes sendo elas 

V1. Manter dicionário estático (ED)
V2. Reiniciar dicionário (RD)
V3. Manter dicionário estático enquanto RC estiver estável ou crescente, se RC 
decrescer reiniciar dicionário.

Descomprime dados previamente comprimidos com o mesmo programa.

#Requisitos
Compilador C++ compatível com o padrão C++11 ou superior.
Ambiente de desenvolvimento C++.

#Como usar
Va ate a pasta projetolzw e compile o código-fonte utilizando g++,
com o comando g++ src/*cpp -o lzwlp
Execute o programa gerado.
Especifique o arquivo que deseja comprimir ou descomprimir, 
juntamente com as opções apropriadas.
o primeiro argumento é o tamanho do dicionario, o segundo caminho para o arquivo,
o terceiro e a versão e o ultimo é 'c' para compressor e 'd' para descomprimir 
O programa irá processar o arquivo e gerar o arquivo alterado.

#Exemplo de Uso
Para comprimir um arquivo:
powershell
./lzwlp 3 arquivoTeste\silesia\dickens 1 c
cmd
lzwlp 3 arquivoTeste\silesia\dickens 1 c
Para descomprimir um arquivo:
powershell
./lzwlp 3 arquivoTeste\silesia\dickens_V1_256K.lzwlp 1 d
cmd
lzwlp 3 arquivoTeste\silesia\dickens_V1_256K.lzwlp 1 d