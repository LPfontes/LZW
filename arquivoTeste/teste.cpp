#include <cstdlib> // Para EXIT_SUCCESS e EXIT_FAILURE
#include <iostream> // Para std::cout, std::cerr
#include <string> // Para std::string, std::to_string
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
namespace fs = std::filesystem;
int main() {
    std::string fileName; // Nome do arquivo base
    std::string diretorio = "C:\\Users\\lpfon\\OneDrive\\Documentos\\projetolzw\\arquivoTeste\\silesia";
    std::vector<std::string> tamanhos = {"4K", "32K", "256K", "2M"};
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path parentPath = currentPath.parent_path();
    for (const auto& arquivo : fs::directory_iterator(diretorio)) {
        // Verifica se o arquivo é um arquivo regular
        if (fs::is_regular_file(arquivo.path())) {
            // Copia o nome do arquivo
            fileName = arquivo.path().filename().string();
        }
    
        // Executa para cada tamanho (1, 2, 3 e 4)
        for (int tamanho = 1; tamanho <= 4; ++tamanho) {
            // Executa para cada versão (1, 2 e 3)
            for (int versao = 1; versao <= 3; ++versao) {
                // Teste compressão
                std::cout << "Testando compressao para tamanho " << tamanho << " e versao " << versao << std::endl;
                std::string comando_compressao = parentPath.string() + "./main " + std::to_string(tamanho) + " " +"..\\arquivoTeste\\silesia\\"+ fileName + " " + std::to_string(versao) + " c";
                int result_compressao = system(comando_compressao.c_str());
                if (result_compressao != EXIT_SUCCESS) {
                    std::cerr << "Erro ao testar compressão para tamanho " << tamanho << " e versão " << versao << std::endl;
                    return EXIT_FAILURE;
                }

                // Teste descompressão
                std::cout << "Testando descompressao para tamanho " << tamanho << " e versao " << versao << "..." << std::endl;
                std::string comando_descompressao = parentPath.string()+ "./main " +  std::to_string(tamanho) + " .." + "\\compactados\\" + fileName + "_V" + std::to_string(versao) + "_" + tamanhos[tamanho-1] + ".lzwlp"  + " " + std::to_string(versao) + " d";
                int result_descompressao = system(comando_descompressao.c_str());
                if (result_descompressao != EXIT_SUCCESS) {
                    std::cerr << "Erro ao testar descompressão para tamanho " << tamanho << " e versão " << versao << std::endl;
                    return EXIT_FAILURE;
                }
            }
        }
    }
    std::cout << "Todos os testes foram bem-sucedidos!" << std::endl;
    return EXIT_SUCCESS;
}