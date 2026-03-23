#include <iostream>
#include <fstream>
#include <string>
#include "PagedArray.h"
#include "../algorithms/QuickSort.h"

int main() {
    //Esto es fijo es para testear por ahora.
    std::string folder = "C:\\Users\\Dylan\\Desktop\\Archivos del PagedArray\\";
    std::string inputFile = folder + "MediumBinaryIntFile.dat";
    std::string outputFile = folder + "MediumSortedIntFile";

    //ate es at end lo abre de una vez a diferencia de end, pero el cursor se puede mover despues
    std::ifstream checkFile(inputFile, std::ios::binary | std::ios::ate); 
    if (!checkFile.is_open()) {
        std::cerr << "No se pudo abrir" << std::endl;
        return 1;
    }

    std::streamsize size = checkFile.tellg(); //Para saber de cuanto es
    //std::cout << "Tamaño del archivo original: " << size << " bytes" << std::endl;
    //Volver al inicio
    checkFile.seekg(0, std::ios::beg);

    //src es el archivo que uno quiere copiar dst el que se cpopia
    std::ifstream src(inputFile, std::ios::binary);
    std::ofstream dst(outputFile, std::ios::binary);
    if (!src || !dst) {
        std::cerr << "Error al abrir archivos para copiar" << std::endl;
        return 1;
    }
    dst << src.rdbuf();
    src.close();
    dst.close();

    // PagedArray
    size_t pageSize = 4096; //para test
    PagedArray arr(outputFile, pageSize);
    std::cout << "Cantidad de enteros en el archivo: " << arr.getSize() << std::endl;

    // Ordenar con QuickSort
    if (arr.getSize() > 0) {
        quickSort(arr, 0, arr.getSize() - 1);
    }

    std::cout << "Archivo ordenado" << std::endl;
    std::cout << "Page Hits: " << arr.getPageHits() << std::endl;
    std::cout << "Page Faults: " << arr.getPageFaults() << std::endl;

    // guardarlo para verlo en txt
    std::ofstream txtFile(folder + "MediumSortedIntFile.txt");
    if (!txtFile) {
        std::cerr << "Error creadno el archivo" << std::endl;
        return 1;
    }

    for (size_t i = 0; i < arr.getSize(); i++) {
        txtFile << arr[i]; //operator, escribe ese entero en el archivo
        if (i != arr.getSize() - 1) txtFile << ","; //poner una coma en los numeros excepto despues del ultiom
    }
    txtFile.close();

    std::cout << "Archivo generado " << std::endl;

    return 0;
}