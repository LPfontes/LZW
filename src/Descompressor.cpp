#include"../include/Descompressor.h"
Descompressor::Descompressor(std::ifstream& in,std::ofstream& out,unsigned int maximo,int versao,unsigned int* tamanho,double* RC)
    : BaseInputOutputBit(in,out, maximo,versao,tamanho,RC), bitsNaoLidos(0),bitsLidos(0),bytesEscritos(0) {
    indexRC = 0;
}
bool Descompressor::operator>> (unsigned int& index)
{
    if (*IndexAtual < valorMaximoIndex - 1){
        if (*IndexAtual+1  == proximaAmpliacaoIndex ){
            proximaAmpliacaoIndex <<= ONE;
            tamanhoIndexAtual++;
        }
    }else{   
        switch (versao){
        case 1:
            break;
        case 2:
            tamanhoIndexAtual = INITIAL_SIZE;
            proximaAmpliacaoIndex = NEXT_BUMP;
            break;
        case 3:
            break;
        default:
            break;
        }
        }
        if (flagV3){
            tamanhoIndexAtual = INITIAL_SIZE;
            proximaAmpliacaoIndex = NEXT_BUMP;
            flagV3 = false;
        }
    // Enquanto não foram lidos todos os bits do símbolo atual
    while (bitsNaoLidos < tamanhoIndexAtual)
    {
        char c;
        // Tenta obter um caractere do arquivo de entrada
        if (!input.get(c)){
             return false; // Se falhar, retorna false
        }   
        // Adiciona os bits do caractere ao número de bits disponíveis
        bitsDisponiveis |= (c & MASK) << bitsNaoLidos;
        // Incrementa o número de bits não lidos
        bitsNaoLidos += BYTE;
       
    }
    // Extrai o símbolo do símbolo atual
    index = bitsDisponiveis & ~(~ZERO << tamanhoIndexAtual);
    // Remove os bits do símbolo do número de bits disponíveis
    bitsDisponiveis >>= tamanhoIndexAtual;
    // Atualiza o número de bits não lidos
    bitsNaoLidos -= tamanhoIndexAtual;
    bitsLidos += tamanhoIndexAtual;
    TaxaDeCompresao();
    if (index == CODEEOF)
        return false;
    else
        return true;
}
void Descompressor::operator<< (const std::string& simbolo)
{
    int tamanho = simbolo.size();
    for (int i = 0; i <tamanho ; i++)
    {
        output << simbolo[i];
        bytesEscritos++;
        TaxaDeCompresao();
               
    }
} 

Descompressor::~Descompressor()
{
    input.close(); // Fecha o arquivo de entrada
    output.close();// Fecha o arquivo de saida
}
void Descompressor::TaxaDeCompresao(){
    double bl = (bytesEscritos != 0) ? bytesEscritos : 1;
    double rc = static_cast<double>(bitsLidos-tamanhoIndexAtual)/static_cast<double>((bl)*8);
    rc = 100 * rc; 
    if (indexRC == 1000)
    {
        double aux = 0.0;
        for (int i = 0; i < 1000; i++)
        {
            aux += RCs[i];
        }
        *RC = aux;
       indexRC = 0;
    }
    RCs[indexRC] = rc;
    indexRC = indexRC + 1;
}