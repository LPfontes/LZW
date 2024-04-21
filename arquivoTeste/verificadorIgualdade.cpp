#include <iostream> // Para std::cout, std::cerr
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
bool saoArquivosIguais(const std::string& arquivo1, const std::string& arquivo2) {
    std::ifstream file1(arquivo1);
    std::ifstream file2(arquivo2);

    if (!file1 || !file2) {
        std::cerr << "Erro ao abrir os arquivos.\n";
        return false;
    }

    char c1, c2;
    while (file1.eof() && file2.eof()) {
        file1.get(c1);
        file2.get(c2);
        if (c1 != c2) {
            return false; // Os arquivos diferem em algum ponto
        }
    }
    return true;
}
int main() {
    std::string fileName = "dickens"; // Nome do arquivo base
    std::vector<std::string> tamanhos = {"4K", "32K", "256K", "2M"};
    std::string file;
    for (int tamanho = 1; tamanho <= 4; ++tamanho) {
        // Executa para cada versão (1, 2 e 3)
        for (int versao = 1; versao <= 3; ++versao) {
            file = "..\\descompactados\\"+fileName + "_V" + std::to_string(versao) + "_" + tamanhos[tamanho-1]+ ".descompact";
            if (saoArquivosIguais(fileName,file)) {
                std::cout << "Os arquivos "+fileName+" "+file + " sao iguais."<<std::endl;
            } else {
                std::cout << "Os arquivos sao diferentes."<<std::endl;
            }
        }

            
    }
    
    return EXIT_SUCCESS;
}