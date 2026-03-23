
#include "QuickSort.h"
#include <algorithm> // Para std::swap

int partition(PagedArray &arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(PagedArray &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        if(pi > 0) quickSort(arr, low, pi - 1); // izquierda
        quickSort(arr, pi + 1, high);           // derecha
    }
}