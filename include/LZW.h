#ifndef LZW_H
#define LZW_H
#define EXTENSION_ERROR "Extensão de arquivo compactado não reconhecida"
#define EXTENSION "lzwlp"
#define DOT "."
#define ALPHABET_SIZE 256
#define K 1024
#define M 1048576
#define PATH1 "../compactados/"
#define PATH2 "..\\descompactados\\"

#include "compressor.h"
#include "descompressor.h"
using namespace std;
class LZW
{
    unsigned int tamanhoMaximo; // Tamanho maximo do dicionario
    string tamanho; // string com o tamanho 4K, 32K, 256K e 2M
    double RC; // variavel que armazena o valor RC razão compressão calculado ao longo dos processos
    double RCAtual; // variavel que armazena a soma de todos os RCS em acumRC quando os mesmo chegam a 1000
    double RCInicial;// variavel que armazena a soma anterior de todos os RCS em acumRC 
    public:
        LZW(int tamanhoMaximo );
        void compressor(string filename,int versao);
        string descompressor(string filename,int versao);
       
        ~LZW();
};

#endif 
