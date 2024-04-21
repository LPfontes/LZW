#include <iostream> // Para std::cout, std::cerr
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <map>
#include <cmath>
int main() {
     
    std::map<char, int> contagem;
    
    std::ifstream arquivoEntrada("C:\\Users\\lpfon\\OneDrive\\Documentos\\projetolzw\\arquivoTeste\\silesia\\todos_arquivos", std::ios::binary);
    char c;

    while ( arquivoEntrada.get(c)) {
        contagem[c]++;
        
    }
    int total = 0;
    for (const auto& par : contagem) {
        total += par.second;
    }
    double entropia = 0.0;
    double probabilidade = 0.0;
    for (const auto& par : contagem) {
        probabilidade = double(par.second)/total;
        entropia += probabilidade * std::log2(1/probabilidade);
    }
    std::cout << entropia << std::endl;
}