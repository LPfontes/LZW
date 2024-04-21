#ifndef DESCOMPRESSOR
#define DESCOMPRESSOR

#include "BaseInputOutputBit.h"
class Descompressor : public BaseInputOutputBit {
private:
   unsigned int bitsNaoLidos;  // Buffer de saída

public:
    Descompressor(std::ifstream& in,std::ofstream& out,unsigned int maximo,int versao,unsigned int* tamanho,double* RC);
    unsigned int* tamanhoTabela;
    unsigned int bytesEscritos;
    unsigned int bitsLidos;
    
    void operator<< (const std::string& simbolo);
    bool operator>> (unsigned int& index); 
    void TaxaDeCompresao();
    ~Descompressor();
};
#endif