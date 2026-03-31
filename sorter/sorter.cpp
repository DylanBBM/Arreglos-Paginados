#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include "PagedArray.h"
#include "../algorithms/QuickSort.h"
#include "../algorithms/MergeSort.h"
#include "../algorithms/HeapSort.h"
#include "../algorithms/SelectionSort.h"
#include "../algorithms/RadixSort.h"


int main(int argc, char* argv[]) {

    string inputFile, outputFile, algoritmo;
    size_t pageSize = 0;
    size_t pageCount = 0;

    //Leer 
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];

        if (arg == "-input") inputFile = argv[++i];
        else if (arg == "-output") outputFile = argv[++i];
        else if (arg == "-alg") algoritmo = argv[++i];
        else if (arg == "-pageSize") pageSize = std::stoul(argv[++i]);
        else if (arg == "-pageCount") pageCount = std::stoul(argv[++i]); // FIX
    }

    // Validaciones
    if (inputFile.empty() || outputFile.empty() || algoritmo.empty() || pageSize == 0 || pageCount == 0){ 
        std::cout << "Uso:\n";
        std::cout << "sorter -input <archivo> -output <archivo> -alg <algoritmo> -pageSize <tam> -pageCount <count>" << '\n';
        return 1;
    }

    if (pageSize % 2 != 0) {
        std::cout << "El pageSize debe ser multiplo de 2\n";
        return 1;
    }

    // Verificar archivo
    std::ifstream checkFile(inputFile, std::ios::binary | std::ios::ate); 
    if (!checkFile.is_open()) {
        std::cerr << "No se pudo abrir archivo input\n";
        return 1;
    }
    checkFile.close();

    // Copiar archivo
    std::ifstream src(inputFile, std::ios::binary);
    std::ofstream dst(outputFile, std::ios::binary);
    if (!src || !dst) {
        std::cerr << "Error al copiar archivos\n";
        return 1;
    }
    dst << src.rdbuf();
    src.close();
    dst.close();

    // PagedArray
    PagedArray arr(outputFile, pageSize, pageCount);

    std::cout << "Cantidad de enteros: " << arr.getSize() << std::endl;

    // Medir tiempo
    auto start = std::chrono::high_resolution_clock::now();

    
    if (arr.getSize() > 0) {

        if (algoritmo == "quick") {
            quickSort(arr, 0, arr.getSize() - 1);
        }
        else if (algoritmo == "merge") {
            mergeSort(arr, 0, arr.getSize() - 1);
        }
        else if (algoritmo == "heap") {
            heapSort(arr, arr.getSize());
        }
        else if (algoritmo == "selection") {
            if (arr.getSize() > 1000000) {
                std::cout << "SelectionSort no permitido para archivos grandes\n";
                return 1;
            }
            selectionSort(arr);
        }
        else if (algoritmo == "radix") {
            radixsort(arr, arr.getSize());
        }
        else {
            std::cout << "Algoritmo no valido\n";
            return 1;
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;


    std::cout << "Archivo ordenado con " << algoritmo << std::endl;
    std::cout << "Tiempo: " << duration.count() << " segundos" << std::endl;
    std::cout << "Page Hits: " << arr.getPageHits() << std::endl;
    std::cout << "Page Faults: " << arr.getPageFaults() << std::endl;
    std::cout << "Con PageSize: " << pageSize << std::endl;
    std::cout << "Con PageCount: " << pageCount << std::endl;



    std::ofstream txtFile(outputFile + ".txt");
    if (!txtFile) {
        std::cerr << "Error creando archivo txt\n";
        return 1;
    }

    for (size_t i = 0; i < arr.getSize(); i++) {
        txtFile << arr[i];
        if (i != arr.getSize() - 1) txtFile << ",";
    }

    txtFile.close();

    std::cout << "Archivo generado correctamente\n";

    return 0;
}