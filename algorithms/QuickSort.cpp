#include "QuickSort.h"

// Ubica el pivote en su posición correcta, menores a la izquierda y mayores a la derecha
int partition(PagedArray& arr, int low, int high) {
    int pivot = arr[high];
    int smallerIndex = low - 1;

    for (int current = low; current <= high - 1; current++) {
        // Si el elemento actual es menor o igual al pivote se mueve a la izquierda
        if (arr[current] <= pivot) {
            smallerIndex++;
            int temp = arr[smallerIndex];
            arr[smallerIndex] = arr[current];
            arr[current] = temp;
        }
    }
    // Coloca el pivote en su posición final
    int temp = arr[smallerIndex + 1];
    arr[smallerIndex + 1] = arr[high];
    arr[high] = temp;

    return smallerIndex + 1;
}

void quickSort(PagedArray& arr, int low, int high) {
    if (low < high) {
        // Obtiene el índice del pivote ya ubicado correctamente
        int pivotIndex = partition(arr, low, high);
        if(pivotIndex > 0) quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}