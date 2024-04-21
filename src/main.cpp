

#include "../include/main.h"

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Uso: " << argv[0] << " <tamanho> <nome_arquivo> <versao> <tipo>" << std::endl;
        return EXIT_FAILURE;
    }

    int tamanho = std::stoi(argv[1]); // Tamanho
    std::string fileName = argv[2];    // Nome do arquivo base
    int versao = std::stoi(argv[3]);   // Versão
    char tipo = argv[4][0];            // Tipo (c para compressão, d para descompressão)

    std::string extensao = ".lzwlp"; // Extensão do arquivo
    LZW* algoritmo;
    std::vector<std::string> tamanhos = {"4K", "32K", "256K", "2M"};
    cout<<fileName<<endl;
    // Verifica o tipo
    if (tipo != 'c' && tipo != 'd') {
        std::cerr << "Tipo inválido. Use 'c' para compressão ou 'd' para descompressão." << std::endl;
        return EXIT_FAILURE;
    }

    // Verifica o tamanho
    if (tamanho < 1 || tamanho > 4) {
        std::cerr << "Tamanho inválido. Use um valor entre 1 e 4." << std::endl;
        return EXIT_FAILURE;
    }

    // Verifica a versão
    if (versao < 1 || versao > 3) {
        std::cerr << "Versão inválida. Use um valor entre 1 e 3." << std::endl;
        return EXIT_FAILURE;
    }

    // Cria uma instância da classe LZW com o tamanho especificado
    algoritmo = new LZW(tamanho);
    string saida;
    // Executa a compressão ou descompressão 
    switch (tipo)
    {
    case 'c':
        // Comprime o arquivo
        try
        {
        algoritmo->compressor(fileName, versao);
        std::string  arquivoComprimido = fileName + "_V" + std::to_string(versao) + "_" + tamanhos[tamanho-1] + extensao;
        std::cout << "Arquivo comprimido criado: " << arquivoComprimido << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Erro: " << e.what() << std::endl;
            return 1;
        }
        
        break;
    case 'd': 
        // Descomprime o arquivo
        try
        {
        saida = algoritmo->descompressor(fileName, versao);
        std::cout << "Arquivo descomprimido criado: "<< saida<< std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Erro: " << e.what() << std::endl;
            return 1;
        }
        break;
    default:
        return EXIT_FAILURE;
        break;
    }
           
    // Destrói a instância da classe LZW para liberar memória
    delete algoritmo;

    return EXIT_SUCCESS;
}
