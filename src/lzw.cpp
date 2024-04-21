#include "../include/LZW.h" 
#define TC 1
LZW::LZW(int tamanho)
{
 switch (tamanho)
 {
    case 1:
        tamanhoMaximo = 4*K;
        this->tamanho = "4K";
    break;
    case 2:
        tamanhoMaximo = 32*K;
        this->tamanho = "32K";
    break;
    case 3:
        tamanhoMaximo = 256*K;
        this->tamanho = "256K";
    break;
    case 4:
        tamanhoMaximo = 2*M;
        this->tamanho = "2M";
    break;
    default:
        throw std::runtime_error("Tamanho Dicionário inválido");
    break;
    
 }
 RCAtual = 0.0;
 RCInicial = 0.0;
}

void LZW::compressor(string filename,int versao) // Metodo do compressor lzw retorna o nome do arquivo descompactado
{
    // Armazena o nome do arquivo original
    string nomeArquivoOriginal = filename;
    string nomeArquivoModificado;
    nomeArquivoModificado = nomeArquivoOriginal;
    /*size_t posBarra = nomeArquivoModificado.find_last_of('\\');
    if (posBarra != string::npos){
        // Extrai o nome do arquivo
        nomeArquivoModificado = nomeArquivoModificado.substr(posBarra+1);
    }
    posBarra = nomeArquivoModificado.find_last_of('/');
    if (posBarra != string::npos){
        // Extrai o nome do arquivo
        nomeArquivoModificado = nomeArquivoModificado.substr(posBarra+1);
    }*/
    unsigned int bytes = 0;
    // Abre o arquivo original para leitura
    ifstream arquivoEntrada(nomeArquivoOriginal,ios::binary);
    // Verifica se o arquivo foi aberto com sucesso
    if (!arquivoEntrada) {
        throw std::runtime_error("Erro ao abrir o arquivo de entrada.");
        
    }
    // Define o nome do arquivo compactado com a extensão específica
    nomeArquivoModificado = nomeArquivoModificado +"_V"+to_string(versao)+"_"+tamanho+DOT+ EXTENSION;
    // Abre o arquivo compactado para escrita
    ofstream arquivoSaida(nomeArquivoModificado,ios::binary);
    if (!arquivoSaida) {
         
        throw std::runtime_error("Erro ao abrir o arquivo de saida.");
     
    }
    // Inicializa o Dicionário de símbolos com os 256 caracteres do alfabeto 
    unordered_map<string, unsigned int> simbolos(tamanhoMaximo);
    for (unsigned int i = 0; i < ALPHABET_SIZE; i++)
        simbolos[string (1,i)] = int(i);
    
    simbolos[""] = 256; // defini a chave 256 como EOF
    string simbolo_atual = ""; // String para armazenar o símbolo atual
    char caracter;             // Variável para armazenar o caractere lido
    unsigned int proximo_index = ALPHABET_SIZE + 1; // Próximo index disponível
    unsigned int tamanhoDicionario = simbolos.size();
    // Loop para ler e comprimir o arquivo original
    auto inicio = chrono::high_resolution_clock::now();
    Compressor compressor(arquivoEntrada,arquivoSaida,tamanhoMaximo,versao,&tamanhoDicionario,&RC);
    while(compressor >> caracter){ // ler o caracter(byte) do arquivo
        bytes++;
        simbolo_atual += caracter; // Adiciona o caractere à string do símbolo atual
        
        // Verifica se o símbolo atual não está no Dicionário
        if(simbolos.find(simbolo_atual) == simbolos.end()){
            // Verifica se ainda há espaço para adicionar novos indexs
            if(proximo_index < tamanhoMaximo - 1){
                simbolos [simbolo_atual] = proximo_index; // Adiciona o simbolo_atual ao Dicionário
                tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
                proximo_index++;
                simbolo_atual.erase(simbolo_atual.size()-1);
                // Escreve o index correspondente ao símbolo atual no arquivo compactado
                compressor << simbolos[simbolo_atual];    
            }else{ 
                switch (versao){
                    case 1: // V1 caso o Dicionário esteja cheio apenas escreve o simbolo no arquivo de saida
                        simbolo_atual.erase(simbolo_atual.size()-1);
                        compressor << simbolos[simbolo_atual];
                         
                    break;
                    case 2: // V2 caso o Dicionário esteja cheio reinicia o mesmo
                        simbolo_atual.erase(simbolo_atual.size()-1);
                        tamanhoDicionario = simbolos.size() + 1;
                        // Escreve o index correspondente ao símbolo atual no arquivo compactado
                        compressor << simbolos[simbolo_atual];
                        //reiniciação o dicionario
                        proximo_index = ALPHABET_SIZE + 1;
                        simbolos.clear();
                        // Reinicia o Dicionário de símbolos com os 256 caracteres do alfabeto
                        for (unsigned int i = 0; i < ALPHABET_SIZE; i++){
                            simbolos[string(1, i)] = i;
                        }
                        simbolos[""] = 256; // defini a chave 256 como EOF
                        tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
                    break;
                    case 3:// V3 caso o Dicionário esteja cheio reinicia o mesmo caso RCInicial seja maior que RCAtual indicando perda na compressão
                        RCAtual = RC;
                        if (RCInicial+0.1 < RCAtual && RCInicial != 0){
                        simbolo_atual.erase(simbolo_atual.size()-1);
                        tamanhoDicionario = simbolos.size() + 1; // define o tamanhoDicionario para que o mesmo seja reinicado no Compressor.cpp
                        // Escreve o index correspondente ao símbolo atual no arquivo compactado
                        compressor << simbolos[simbolo_atual];
                        // Define o símbolo atual como o caractere atual
                        proximo_index = ALPHABET_SIZE + 1;
                        simbolos.clear(); // Limpa o Dicionário
                        // Reinicia o Dicionário de símbolos com os 256 caracteres do alfabeto
                        for (unsigned int i = 0; i < ALPHABET_SIZE; i++){
                            simbolos[string(1, i)] = i;
                        }
                        simbolos[""] = 256; // defini a chave 256 como EOF
                        tamanhoDicionario = simbolos.size();// Atualiza o tamanho do Dicionário
                        }else{
                            simbolo_atual.erase(simbolo_atual.size()-1);
                            // Escreve o index correspondente ao símbolo atual no arquivo compactado
                            compressor << simbolos[simbolo_atual];
                        }
                    RCInicial = RCAtual; // atualiza o valor de RCinical com o do RCAtual
                    break;
                }    
            } 
            simbolo_atual = caracter;    
        }
    }
    // Escreve o index do último símbolo atual no arquivo compactado
    if( simbolo_atual.size()){
        compressor << simbolos[simbolo_atual];
        
    }
    auto fim = chrono::high_resolution_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    cout<< "Tempo de compresão: "<< duracao.count() <<" milissegundos."<< endl;
}

string LZW::descompressor(string filename,int versao)
{
       // Armazena o nome do arquivo original
    string nomeArquivoOriginal = filename;
    string nomeArquivoModificado;
    // Encontra a posição do último ponto no nome do arquivo
    int posicao = nomeArquivoOriginal.find_last_of(DOT);
    // Extrai o nome do arquivo sem extensão
    nomeArquivoModificado = nomeArquivoOriginal.substr(0, posicao);    
    // Obtém a extensão do arquivo
    string extensao = nomeArquivoOriginal.substr(posicao + 1);
    // Verifica se a extensão do arquivo é diferente da extensão esperada
    if (extensao != EXTENSION)
    {
        // Se a extensão não corresponder, exibe uma mensagem de erro e encerra o programa
        cerr << EXTENSION_ERROR;
        exit(EXIT_FAILURE);
    }
    /*size_t posBarra = nomeArquivoModificado.find_last_of('\\');
    if (posBarra != string::npos){
        // Extrai o nome do arquivo
        nomeArquivoModificado = nomeArquivoModificado.substr(posBarra+1);
    }
    posBarra = nomeArquivoModificado.find_last_of('/');
    if (posBarra != string::npos){
        // Extrai o nome do arquivo
        nomeArquivoModificado = nomeArquivoModificado.substr(posBarra+1);
    }*/
    nomeArquivoModificado += ".descompact";
    // Abre o arquivo de entrada para leitura
    ifstream arquivoEntrada(nomeArquivoOriginal,ios::binary);
    if (!arquivoEntrada) {
        throw std::runtime_error("Erro ao abrir o arquivo.");
    }
    // Abre o arquivo de saida para escrita
    ofstream arquivoSaida(nomeArquivoModificado,ios::binary);
    if (!arquivoSaida) {
        throw std::runtime_error("Erro ao abrir o arquivo.");
    }
    // Inicializa o Dicionário de símbolos com os 256 caracteres do alfabeto
    unordered_map<unsigned int, string> simbolos(tamanhoMaximo);
    for (unsigned int i = 0; i < ALPHABET_SIZE; i++)
        simbolos[i] = string(1, i);

    simbolos[256] = string(""); // defini a chave 256 como EOF
    string simbolo_anterior;    // Armazena o símbolo anterior
    unsigned int index;        // Armazena o index lido
    unsigned int proximo_index = ALPHABET_SIZE + 1; // Próximo index disponível
    unsigned int indexLidos = 0; // Armazena a quatidade de index lidos
    unsigned int tamanhoDicionario = simbolos.size(); // Armazena a quatidade de index lidos
    Descompressor* descompressor;
    descompressor = new Descompressor(arquivoEntrada,arquivoSaida,tamanhoMaximo,versao,&tamanhoDicionario,&RC); 
    // Loop para ler e descomprimir o arquivo compactado
    auto inicio = chrono::steady_clock::now();
    while(*descompressor >> index){ // ler o index do arquivo compactado
        indexLidos++;
        if (simbolos.find(index) == simbolos.end()) // Se o index não estiver no Dicionário 
        {
            simbolos[index] = simbolo_anterior + simbolo_anterior[0]; // adiciona o simbolo anterior com o primeiro caracter do mesmo
            tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
            proximo_index++;
            *descompressor << simbolos[index]; // Escreve o símbolo correspondente ao index no arquivo descompactado
            switch (versao) // Para reniciar o Dicionário na V3
            {
            case 3: // V3 caso o Dicionário esteja cheio reinicia o mesmo se RCInicial seja maior que RCAtual indicando perda na compressão
                RCAtual = (tamanhoDicionario < tamanhoMaximo-1) ? 0: RC; // RCAtual só será atualizado caso o Dicionário esteja cheio 
                if (RCInicial+0.1 < RCAtual && RCInicial != 0){ // Se RCInicial for maior que  RCAtual então aconteceu perda de compressão entao reinicia o Dicionário;
                    proximo_index = ALPHABET_SIZE + 1;
                    simbolos.clear();// Limpa o Dicionário
                    // Reinicia o Dicionário de símbolos com os 256 caracteres do alfabeto
                    for (unsigned int i = 0; i < ALPHABET_SIZE; i++)
                        simbolos[i] = string(1, i);
                    simbolos[256] = string(""); // Defini a chave 256 como EOF
                    descompressor->setFlagV3();// Flag para reinicar a quantidade de bits com que o index é escrito em Descompressor.cpp
                    tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
                    *descompressor >> index; // ler o index do arquivo compactado
                    *descompressor << simbolos[index]; // Escreve o símbolo correspondente ao index no arquivo descompactado
                }
                RCInicial = (RCAtual != 0) ? RCAtual : RCInicial; // atualiza o valor de RCinical com o do RCAtual
                break;
            default:
                break;
            }
        }else{
            if(simbolo_anterior.size()){ // se simbolo_anterior for diferente de vazio
                // Escreve o símbolo correspondente ao index no arquivo descompactado
                *descompressor << simbolos[index];
                switch (versao){
                case 1:// Para V1 caso o Dicionário esteja cheio não sera adicionado mais nenhum simbolo novo
                    if (tamanhoDicionario < tamanhoMaximo-1) 
                    {
                        simbolos[proximo_index] =  simbolo_anterior +  simbolos[index][0]; // adiciona o simbolo anterior com o primeiro do simbolo do index
                        tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
                        proximo_index++; 
                    }
                    break;
                case 2: // V2 caso o Dicionário esteja cheio reinicia o mesmo
                    if (tamanhoDicionario < tamanhoMaximo-1) 
                    {
                        simbolos[proximo_index] =  simbolo_anterior +  simbolos[index][0]; // adiciona o simbolo anterior com o primeiro do simbolo do index
                        tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
                        proximo_index++; 
                    }else{
                        proximo_index = ALPHABET_SIZE + 1;
                        simbolos.clear();// Limpa o Dicionário
                        // Reinicia o Dicionário de símbolos com os 256 caracteres do alfabeto
                        for (unsigned int i = 0; i < ALPHABET_SIZE; i++)
                            simbolos[i] = string(1, i);
                        simbolos[256] = string("");  // defini a chave 256 como EOF
                        tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
                    }
                    break;
                default:
                case 3:// V3 caso o Dicionário esteja cheio reinicia o mesmo se RCInicial seja maior que RCAtual indicando perda na compressão
                    if (tamanhoDicionario < tamanhoMaximo-1)  
                    {
                        simbolos[proximo_index] =  simbolo_anterior +  simbolos[index][0]; // adiciona o simbolo anterior com o primeiro do simbolo do index
                        tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
                        proximo_index++; 
                    }
                    RCAtual = (tamanhoDicionario < tamanhoMaximo-1) ? 0: RC;
                    if (RCInicial+0.1 < RCAtual && RCInicial != 0){
                            
                        proximo_index = ALPHABET_SIZE + 1;
                        simbolos.clear(); // Limpa o Dicionário
                        // Reinicia o Dicionário de símbolos com os 256 caracteres do alfabeto
                        for (unsigned int i = 0; i < ALPHABET_SIZE; i++) 
                            simbolos[i] = string(1, i);
                        simbolos[256] = string("");            // defini a chave 256 como EOF
                        descompressor->setFlagV3();           // Flag para reinicar a quantidade de bits com que o index é escrito em Descompressor.cpp
                        tamanhoDicionario = simbolos.size(); // Atualiza o tamanho do Dicionário
                        *descompressor >> index;            // ler o index do arquivo compactado
                        *descompressor << simbolos[index]; // Escreve o símbolo correspondente ao index no arquivo descompactado
                        }
                    RCInicial = (RCAtual != 0) ? RCAtual : RCInicial; // atualiza o valor de RCinical com o do RCAtual
                    break;
                }
            }else{
              *descompressor << simbolos[index]; // Escreve o símbolo correspondente ao index no arquivo descompactado
            }
        }
        // Atualiza o símbolo anterior com o símbolo atual
        simbolo_anterior = simbolos[index];
    }
    auto fim = chrono::steady_clock::now();
    auto duracao = chrono::duration_cast<chrono::milliseconds>(fim - inicio);
    delete descompressor;
    std::cout<< "Tempo de descompresão: "<< duracao.count() <<" milissegundos."<< std::endl;        
    return PATH2 + nomeArquivoModificado;     
}

// Destrutor da classe LZW
LZW::~LZW(){

}

