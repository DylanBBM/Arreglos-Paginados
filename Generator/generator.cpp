#include <iostream>
#include <fstream> // Para trabajar con archivos
#include <random> //Para mt19937 y random device
#include <algorithm>
#include <cstdint> // Para asegurarse siempre que los numeros sean de 4 bytes
#include <stdexcept>
#include <string>
#include <filesystem> 

long long getLimit(const std::string& size) {

    // Por el limite de los ints
    long long limit = 0;
    if(size == "small") {  
        limit = 32LL * 1024 * 1024 / 4; 
    } 
    else if(size == "medium") {
        limit = 64LL * 1024 * 1024 / 4; 
    } 
    else if(size == "large") {
        limit = 128LL * 1024 * 1024 / 4; 
    }

    // Limite de enteros del archivo
    return limit;
}

void writeFile(const std::string& outputFilePath, long long Limit) {

    // Escribe en el archivo de manera binaria
    std::ofstream BinaryFile(outputFilePath, std::ios::binary);
    if(!BinaryFile){
        throw std::runtime_error("Error: no se pudo crear o abrir el archivo");
    }

    // Mersene Twister 19937, random_device puede usar hardware o ruido.
    std::mt19937 randomSeedGenerator(std::random_device{}());
    std::uniform_int_distribution<int32_t> dist(0, INT32_MAX);
    int32_t randomNumber;

    for(long long i = 0; i < Limit; i++){
        randomNumber = dist(randomSeedGenerator);
        //Escribir en el archivo
        BinaryFile.write(reinterpret_cast<char*>(&randomNumber), sizeof(int32_t));
    }

    BinaryFile.close();
}

bool checkSize(const std::string& size){
    std::string sizeLower = size;
    std::transform(sizeLower.begin(), sizeLower.end(), sizeLower.begin(), ::tolower);

    return (sizeLower == "small" || sizeLower == "medium" || sizeLower == "large");
}

std::string getOutputPath(char* argv[]){

    if(std::string(argv[3]) != "-output"){
        throw std::invalid_argument("Error: Uso correcto: generator -size <SIZE> -output <OUTPUT FILE PATH>");
    }

    // Obtener ruta
    std::string outputPath = argv[4];

    // Obtener carpeta padre
    std::filesystem::path pathObj(outputPath);
    // Si es una carpeta existente, genera un nombre automático
    if (std::filesystem::is_directory(pathObj)) {
        outputPath = (pathObj / "BinaryIntFile.dat").string();
    } 
    else {
        // Si tiene carpeta padre, validarla
        std::filesystem::path parent = pathObj.parent_path();
        if (!parent.empty() && !std::filesystem::exists(parent)) {
            throw std::invalid_argument("Error: La carpeta destino no existe.");
        }
    }

    return outputPath;
}


void inputSizeValidation(int argc){
    // Valida cantidad de argumentos
    if(argc != 5){
        throw std::invalid_argument("Error: La cantidad de argumentos recibidos debe ser de 5 exactamente.");
    }
}

std::string getSize(char* argv[]){ 
    if(std::string(argv[1]) != "-size") {
        throw std::invalid_argument("Error: Uso correcto: generator -size <SIZE> -output <OUTPUT FILE PATH>");
    }

    // Obtenemos el size del archivo
    std::string size = argv[2];
    std::transform(size.begin(), size.end(), size.begin(), ::tolower);

    if(!checkSize(size)){
        throw std::invalid_argument("Error: El argumento <SIZE> debe ser small, medium o large");
    }

    return size;
}

int main(int argc, char* argv[]){ 
    try{
        inputSizeValidation(argc);
        std::string size = getSize(argv);
        long long Limit = getLimit(size); // Ahora sí después de validar size
        std::string outputFilePath = getOutputPath(argv);
        writeFile(outputFilePath, Limit);

        std::cout << "Archivo generado correctamente en " << outputFilePath << " con size de " << Limit << " enteros aleatorios" << '\n';

    }
    catch(const std::exception& error){
        std::cout << error.what() << std::endl;
        return 1;
    }

    return 0;
}