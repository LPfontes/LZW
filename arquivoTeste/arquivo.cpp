#include <cstdlib> // Para EXIT_SUCCESS e EXIT_FAILURE
#include <iostream> // Para std::cout, std::cerr
#include <string> // Para std::string, std::to_string
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>

namespace fs = std::filesystem;



int main() {
    std::string fileName; 
    std::string diretorio = "C:\\Users\\lpfon\\OneDrive\\Documentos\\projetolzw\\arquivoTeste\\silesia";
    std::vector<std::string> tamanhos = {"4K", "32K", "256K", "2M"};
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path parentPath = currentPath.parent_path();
    for (const auto& arquivo : fs::directory_iterator(diretorio)) {
        // Verifica se o arquivo é um arquivo regular
        if (fs::is_regular_file(arquivo.path())) {
            // Copia o nome do arquivo
            fileName = arquivo.path().filename().string();
            std::cout<<fileName<<std::endl;
        }
    }
    return 0;
}