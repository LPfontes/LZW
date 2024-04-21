#ifndef COMPRESSOR
#define COMPRESSOR
#include "BaseInputOutputBit.h"
class Compressor : public BaseInputOutputBit {

    public:
        Compressor(std::ifstream& in,std::ofstream& out,unsigned int maximo,int versao,unsigned int* tamanho,double* RC);
        unsigned int bufferSaida;   // Buffer de saída
        unsigned int bytesLidos;    // Quantidade de bytes lidos do arquivo de entrada
        unsigned int bitsEscritos; // Quantidade de bits Escritos no arquivo de saída
        std::chrono::time_point<std::chrono::high_resolution_clock> inicio;
        std::chrono::time_point<std::chrono::high_resolution_clock> fim;
        std::ofstream* saida;
        bool operator>> ( char &c);     // Sobrecarga do operador de leitura
        void operator<< (const unsigned int &i); // Sobrecarga do operador de escrita
        void flush(const int val); // metodo que escreve os bits no arquivo de saida
        void TaxaDeCompresao(); // metodo que calcula o RC atual
        ~Compressor(); 


};
#endif