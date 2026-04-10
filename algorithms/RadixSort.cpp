#include "RadixSort.h"

// Ordena los elementos según el dígito representado por exp
void countSort(PagedArray& arr, size_t size, int exp)
{
    int* output = new int[size];
    int count[10] = {0};

    // Cuenta las ocurrencias de cada dígito
    for (size_t currentIndex = 0; currentIndex < size; currentIndex++)
        count[(arr[currentIndex] / exp) % 10]++;

    // Ajusta count para que contenga la posición real de cada dígito en output
    for (int digitIndex = 1; digitIndex < 10; digitIndex++)
        count[digitIndex] += count[digitIndex - 1];

    // Construye el arreglo de salida recorriendo de derecha a izquierda
    for (int currentIndex = size - 1; currentIndex >= 0; currentIndex--) {
        output[count[(arr[currentIndex] / exp) % 10] - 1] = arr[currentIndex];
        count[(arr[currentIndex] / exp) % 10]--;
    }

    // Copia el arreglo de salida al arreglo original
    for (size_t currentIndex = 0; currentIndex < size; currentIndex++)
        arr[currentIndex] = output[currentIndex];

    delete[] output;
}

void radixSort(PagedArray& arr)
{
    size_t size = arr.getSize();

    // Encuentra el valor máximo para saber cuántos dígitos hay
    int maxValue = arr[0];
    for (size_t currentIndex = 1; currentIndex < size; currentIndex++) {
        if (arr[currentIndex] > maxValue)
            maxValue = arr[currentIndex];
    }

    // Ejecuta countSort para cada dígito
    for (int exp = 1; maxValue / exp > 0; exp *= 10)
        countSort(arr, size, exp);
}