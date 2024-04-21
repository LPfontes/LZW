#ifndef BASEINPUTOUTPUTBIT
#define BASEINPUTOUTPUTBIT
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <chrono>
#define CODEEOF 256
#define BYTE 8              // Número de bits em um byte
#define MASK 0xFF           // Máscara para um byte
#define INITIAL_SIZE 9      // Tamanho inicial para a tabela de codificação
#define NEXT_BUMP 512  // Próximo aumento de tamanho para a tabela de codificação
#define CURRENT_CODE 257   // Código atual
#define ZERO 0              // Representação do número zero
#define ONE 1               // Representação do número um
#define taxaAtualizacao 1000
class BaseInputOutputBit {
protected:
    std::ifstream& input;           // arquivo de entrada
    std::ofstream& output;          // arquivo de saida
    unsigned int tamanhoIndexAtual; // Tamanho atual do Index
    int bitsDisponiveis;            // Número de bits disponíveis
    unsigned int* IndexAtual;       // Index atual
    unsigned int proximaAmpliacaoIndex;   // Próxima ampliação do Index da tabela
    unsigned int valorMaximoIndex;    // Valor máximo do Index da tabela
    int versao; // armazena a versão do processo
    double* RC; // ponteiro para alterar o valor de RC do LZW.cpp
    bool flagV3; // Flag para reiniciar a quantidade de bits necessarios para escrever o index atual na V3
    double RCs[taxaAtualizacao]; // variavel que armazena os valores RCs, razão compressão 
    int indexRC; // variavel que armazena o index atual do RCs, é reiniciado ao chegar em taxaAtualizacao


public:
    BaseInputOutputBit(std::ifstream& input,std::ofstream& output, unsigned int maximo,int versao,unsigned int* tamanho,double* RC);
    ~BaseInputOutputBit();
    void setFlagV3();
};

#endif 

