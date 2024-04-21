#include "../include/Compressor.h"
Compressor::Compressor(std::ifstream& in,std::ofstream& out,unsigned int maximo,int versao,unsigned int* tamanho,double* RC)
    : BaseInputOutputBit(in,out, maximo,versao,tamanho,RC), bufferSaida(0), bytesLidos(0),bitsEscritos(0) {
    // Construtor de Compressor
    indexRC = 0;
    //saida = new std::ofstream("grafico.csv",std::ios::binary);
}

void Compressor::operator<< (const unsigned int &i) {
    // Insere o símbolo atual no buffer de saída, deslocando-o para a esquerda
    bufferSaida |= i << bitsDisponiveis;
    // Atualiza o número de bits disponíveis no buffer
    bitsDisponiveis += tamanhoIndexAtual;
    // Limpa o buffer se o número de bits disponíveis for suficiente para um byte
    flush(BYTE);
    if (bitsEscritos == 0){
        inicio = std::chrono::high_resolution_clock::now();
    } 
    bitsEscritos += tamanhoIndexAtual;
    /*double l = static_cast<double>(bitsEscritos)/static_cast<double>(bytesLidos*8);
    auto fim = std::chrono::high_resolution_clock::now();
    auto duracao = std::chrono::duration_cast<std::chrono::microseconds>(fim - inicio);
    *saida << l <<","<< duracao.count() <<std::endl;*/
    TaxaDeCompresao();
    // Atualiza o tamanhoIndexAtual
    if (*IndexAtual < valorMaximoIndex - 1)
        {
            if (*IndexAtual  == proximaAmpliacaoIndex)
            {
                proximaAmpliacaoIndex <<= ONE;
                tamanhoIndexAtual++;
            }
        }else{
            if(*IndexAtual !=  valorMaximoIndex - 1){
                switch (versao){
                case 1:
                    break;
                case 2:
                    tamanhoIndexAtual = INITIAL_SIZE;
                    proximaAmpliacaoIndex = NEXT_BUMP;
                    break;
                case 3:
                    tamanhoIndexAtual = INITIAL_SIZE;
                    proximaAmpliacaoIndex = NEXT_BUMP;
                    break;
                }
            }
            
        }
}
bool Compressor::operator>> ( char &c){
    // Verifica se é possível extrair um caractere do fluxo de entrada
    if( input.get(c) ){ 
        bytesLidos = bytesLidos + 1;
        TaxaDeCompresao();
        return true;  // Se a extração for bem-sucedida, retorna true
    }else
        return false; // Caso contrário, retorna false
}
void Compressor::flush(const int val)
{
    // Enquanto houver bits disponíveis no buffer para saída
    while(bitsDisponiveis >= val){
        // Escreve o byte menos significativo do buffer no arquivo de saída
        output.put(bufferSaida & MASK);
        // Desloca o buffer para a direita em um byte
        bufferSaida >>= BYTE;
        // Reduz o número de bits disponíveis no buffer
        bitsDisponiveis -= BYTE;
        
    }
}
void Compressor::TaxaDeCompresao(){
    double bl = (bytesLidos != 1) ? bytesLidos : 2;
    double rc = static_cast<double>(bitsEscritos)/static_cast<double>((bl-1)*8); // calcula o RC atual, bl-1 para sicronizar com o descompressor
    rc = 100 * rc; 
    if (indexRC == taxaAtualizacao) // caso indexRC seja = taxaAtualizacao soma todos os RCs armazenados e atualiza o valor de RC
    {
        double aux = 0.0;
        for (int i = 0; i < taxaAtualizacao; i++)
        {
            aux += RCs[i]; // soma todos os valores em RCs
        }
        *RC = aux; // atualiza o valor de RC de LZW.cpp com a soma 
       indexRC = 0; // reinicia o valor de indexRC 
    }
    RCs[indexRC] = rc; // armazena o valor do rc calculado em RCs
    indexRC = indexRC + 1;
}


Compressor::~Compressor(){
    // Escreve o símbolo de fim de arquivo no arquivo de saída
    *this << CODEEOF;
    // Limpa o buffer de saída
    flush(ZERO);
    // Fecha os arquivos
    output.close();
    input.close();
}

