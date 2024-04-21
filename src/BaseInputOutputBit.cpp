#include "../include/BaseInputOutputBit.h"

BaseInputOutputBit::BaseInputOutputBit(std::ifstream& input,std::ofstream& output, unsigned int maximo,int versao,unsigned int* tamanho,double* RC)
    : input(input),output(output), tamanhoIndexAtual(INITIAL_SIZE), bitsDisponiveis(ZERO),
     IndexAtual(tamanho), proximaAmpliacaoIndex(NEXT_BUMP),
    valorMaximoIndex(maximo),versao(versao),RC(RC),flagV3(false) {}

void BaseInputOutputBit::setFlagV3(){
    flagV3 = true;
}
BaseInputOutputBit::~BaseInputOutputBit() {}