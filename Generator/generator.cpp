#include <iostream>
#include <fstream> // Para trabajar con archivos
#include <random>
#include <cstdint> // Para asegurarse siempre que los numeros sean de 4 bytes

int main(int argc, char* argv[]){ 

    if(argc != 5){
        std::cout << "Uso correcto: $> generator -size <SIZE> -output <OUTPUT FILE PATH>" << '\n';
        std::cout << "SIZE Puede ser: SMALL (512MB) MEDIUM (1024MB) LARGE (2048MB)" << '\n';
        std::cout << "<OUTPUT FILE PATH> corresponde a la ruta del sistema de archivos donde se guarda el archivo generador." << '\n';
        std::cout << "Presione ENTER para salir" << std::endl;
        std::cin.get();
        return 1;
    }

    // Obtener el tamaño del archivo 
    std::string size = argv[2];

    long long Limit = 0; // Numero de enteros de 4 bytes
    if(size == "SMALL") {
        //Numero de enteros:
        Limit = 512LL * 1024 * 1024 / 4; 
    } else if(size == "MEDIUM") {
        Limit = 1024LL * 1024 * 1024 / 4; 
    } else if(size == "LARGE") {
        Limit = 2048LL * 1024 * 1024 / 4; 
    } else {
        std::cout << "Error, la casilla <SIZE> debe ser SMALL, MEDIUM o LARGE" << '\n';
        std::cout << "Presione ENTER para salir" << std::endl;
        std::cin.get();
        return 1;
    }

    // Obtener la ruta
    std::string outputFilePath = argv[4];
    //Objeto para escribir
    std::ofstream BinaryFile(outputFilePath, std::ios::binary);
    if(!BinaryFile){
        std::cout << "Error no se pudo abrir el archivo" << '\n';
        std::cout << "Presiona Enter para salir" << std::endl;
        std::cin.get();
        return 1;
    }
    //Hace una semilla para los numeros random
    std::mt19937 randomSeedGenerator(std::random_device{}());
    int32_t randomNumber;

    //Hasta el limite
    for(long long i = 0; i < Limit; i++){
        randomNumber = randomSeedGenerator();
        //Write (direccion char, tamanio)
        BinaryFile.write((char*)&randomNumber, sizeof(int32_t));
    }
    BinaryFile.close();
    //Notificar 
    std::cout << "Archivo generado correctamente en " << outputFilePath 
              << "con size de " << Limit << " enteros aleatorios" << '\n';
    std::cout << "Presione Enter para salir" << std::endl;
    std::cin.get();
    return 0;
}