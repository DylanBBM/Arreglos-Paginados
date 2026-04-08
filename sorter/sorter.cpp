#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <stdexcept>
#include <filesystem>
#include "PagedArray.h"
#include "../algorithms/QuickSort.h"
#include "../algorithms/MergeSort.h"
#include "../algorithms/HeapSort.h"
#include "../algorithms/ShellSort.h"
#include "../algorithms/SelectionSort.h"

//"Configuracion" del sorter
struct Config {
    std::string inputFile;//El que toma con todo y ruta
    std::string outputFile;//El copiado que ordena con todo y ruta
    std::string algorithm; //El algoritmo seleccionado
    size_t pageSize = 0; //Cantidad de enteros que caben en una pagina
    size_t pageCount = 0; //Cantidad de paginas en memoria
};

//Metodo que verifica si la cantidad de argumentos es la correcta
void inputSizeValidation(int argc) {
    if (argc != 11) {
        throw std::invalid_argument("Uso: sorter -input <archivo> -output <archivo> -alg <algoritmo> -pageSize <tam> -pageCount <count>");
    }
}

//Metodo que "asigna" los argumentos ingresados
void parseArgs(int argc, char* argv[], Config& config) {

    for (int i = 1; i < argc; i++) {

        //Convertir en string
        std::string arg = argv[i];

        if (arg == "-input"){
            if(i+1 >= argc){
                throw std::invalid_argument("Error: Falta valor para -input");
            }
            config.inputFile = argv[++i];//++i lo que hace es sumar y asignar, si i es 1 actualmente se suma 1 entonces 2 y asigna ese "2"
        }
        else if(arg == "-output"){
            if(i+1 >= argc){
                throw std::invalid_argument("Error: Falta valor para -output");
            }
            std::string outputPath = argv[++i];

            // Si si es una carpeta, genera los nombres automaticamente
            if (std::filesystem::is_directory(outputPath)) {
                config.outputFile = (std::filesystem::path(outputPath) / "SortedInts.dat").string();
            } else {
                config.outputFile = outputPath;
            }
        }
        else if(arg == "-alg"){
            if(i+1 >= argc){
                throw std::invalid_argument("Error: Falta valor para -alg");
            }
            config.algorithm = argv[++i];
        }
        else if(arg == "-pageSize"){
            if(i+1 >= argc){
                throw std::invalid_argument("Error: Falta valor para -pageSize");
            }
            config.pageSize = std::stoul(argv[++i]); //Unsigned long, c++ lo convierte en size_T
        }
        else if (arg == "-pageCount"){ 
            if(i+1 >= argc){
                throw std::invalid_argument("Error: Falta valor para -pageCount");
            }
            config.pageCount = std::stoul(argv[++i]);
        }
    }
}

//Metodo que verifica ciertos parametros de la configuracion del sorter
void validateConfig(const Config& config) {

    /*if (config.pageSize % 2 != 0){
        throw std::invalid_argument("Error: El pageSize debe ser multiplo de 2");
    }*/

    if (config.pageSize == 0) {
        throw std::invalid_argument("Error: pageSize no puede ser 0");
    }

    if (config.pageCount == 0) {
        throw std::invalid_argument("Error: pageCount no puede ser 0");
    }

    //Abre el archivo a copiar en modo lectura, si no esta abierto error.
    std::ifstream checkFile(config.inputFile, std::ios::binary);

    if (!checkFile.is_open()){
        throw std::invalid_argument("Error: No se pudo abrir el archivo de input ");
    }
    checkFile.close();

    //Verifica si la carpeta donde sera guardado el archivo existe

    std::filesystem::path pathObj(config.outputFile); //Convierte el string en una ruta
    std::filesystem::path parent = pathObj.parent_path(); //Extrae solo la carpeta padre

    if (!parent.empty() && !std::filesystem::exists(parent)){
        throw std::invalid_argument("Error: La carpeta de destino no existe");
    }

}

//Metodo que copia el archivo de input al output
void copyFile(const Config& config) {

    std::ifstream src(config.inputFile, std::ios::binary);
    std::ofstream dst(config.outputFile, std::ios::binary);
    //Si alguno de los 2 no se pudo abrir
    if (!src || !dst)
        throw std::runtime_error("Error al copiar archivos");
    //Copia todo el contenido del archivo (std::cout << insercionstream)
    dst << src.rdbuf();
}

//Metodo que ejecuta el ordenamiento de los enteros
void runSortAlgorithm(const Config& config, PagedArray& arr) {

    if (arr.getSize() == 0) {
        return;
    }

    //Limites de algunos algoritmos, por que duran mucho.
    size_t heapLimit      = (64LL  * 1024 * 1024) / sizeof(int); //64MB
    size_t selectionLimit = (1LL   * 1024 * 1024) / sizeof(int); //32MB
    size_t shellLimit     = (32LL  * 1024 * 1024) / sizeof(int); //1MB

    //Quicksort
    if (config.algorithm == "quick") {
        quickSort(arr, 0, arr.getSize() - 1);
    }

    //Merge Sort
    else if (config.algorithm == "merge") {
        mergeSort(arr, 0, arr.getSize() - 1);
    }

    //Selection Sort
    else if (config.algorithm == "selection") {

        if (arr.getSize() > selectionLimit){ 
            throw std::invalid_argument("SelectionSort no esta permitido para archivos de mas de 1MB");
        }
        selectionSort(arr);
    }

    //Heap Sort
    else if (config.algorithm == "heap") {

        if (arr.getSize() > heapLimit){
            throw std::invalid_argument("HeapSort no esta permitido para archivos mayores a 64MB");
        }
        heapSort(arr, (int)arr.getSize());
    }

    //Shell Sort
    else if (config.algorithm == "shell") {
        if (arr.getSize() > shellLimit){
            throw std::invalid_argument("ShellSort no esta permitido para archivos mayores a 64MB");
        }    
        shellSort(arr, (int)arr.getSize());
    }
    //Alguno no valido
    else {
        throw std::invalid_argument("Error: Algoritmo no valido");
    }
}

//Metodo que convierte el archivo ordenado en legible (.txt)
void writeOutput(const Config& config, PagedArray& arr) {
    //Crea el archivo de texto
    std::ofstream txtFile(config.outputFile + ".txt");
    //Si no se pudo abrir
    if (!txtFile){
        throw std::runtime_error("Error creando archivo txt");
    }
    //Recorre todos los enteros
    for (size_t i = 0; i < arr.getSize(); i++) {
        //Escribe el numero en el txt
        txtFile << arr[i];

        //Agrega coma excepto en el ultimo
        if (i != arr.getSize() - 1){
            txtFile << ","; 
        } 
    }
}

int main(int argc, char* argv[]) {

    try {

        inputSizeValidation(argc);
        Config config;
        parseArgs(argc, argv, config);
        validateConfig(config);
        copyFile(config);

        //Crea paged array, que ordenara el output
        PagedArray arr(config.outputFile, config.pageSize, config.pageCount);
        std::cout << "Cantidad de enteros: " << arr.getSize() << '\n';

        //Guarda el momento exacto cuando se ejecuta
        auto start = std::chrono::high_resolution_clock::now();

        runSortAlgorithm(config, arr);
        auto end = std::chrono::high_resolution_clock::now();

        //Duracion cuando termina final menos inicio
        std::chrono::duration<double> duration = end - start;

        std::cout << "Archivo ordenado con: " << config.algorithm << '\n';
        std::cout << "Tiempo: "<< duration.count() << " segundos" << '\n';
        std::cout << "Page Hits: "<< arr.getPageHits() << '\n';
        std::cout << "Page Faults: "<< arr.getPageFaults() << '\n';
        std::cout << "Con PageSize: "<< config.pageSize << '\n';
        std::cout << "Con PageCount: "<< config.pageCount << '\n';

        writeOutput(config, arr);

        std::cout << "Archivo para visualizar en .txt generado" << std::endl;
    }
    catch (const std::exception& error) {
        std::cout << error.what() << std::endl;
        return 1;
    }
    return 0;
}