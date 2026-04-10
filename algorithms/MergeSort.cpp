#include <iostream>
#include "MergeSort.h"
using namespace std;

void merge(PagedArray& arr, int left, int mid, int right){
    int leftSize = mid - left + 1;
    int rightSize = right - mid;

    // Subarreglos temporales para almacenar las mitades
    int* leftArray = new int[leftSize];
    int* rightArray = new int[rightSize];

    // Copia los datos a los subarreglos temporales
    for (int leftIndex = 0; leftIndex < leftSize; leftIndex++)
        leftArray[leftIndex] = arr[left + leftIndex];
    for (int rightIndex = 0; rightIndex < rightSize; rightIndex++)
        rightArray[rightIndex] = arr[mid + 1 + rightIndex];

    int leftIndex = 0, rightIndex = 0;
    int mergeIndex = left;

    // Mezcla los subarreglos comparando elemento por elemento
    while (leftIndex < leftSize && rightIndex < rightSize) {
        if (leftArray[leftIndex] <= rightArray[rightIndex]) {
            arr[mergeIndex] = leftArray[leftIndex];
            leftIndex++;
        }
        else {
            arr[mergeIndex] = rightArray[rightIndex];
            rightIndex++;
        }
        mergeIndex++;
    }

    // Copia los elementos restantes del subarreglo izquierdo
    while (leftIndex < leftSize) {
        arr[mergeIndex] = leftArray[leftIndex];
        leftIndex++;
        mergeIndex++;
    }

    // Copia los elementos restantes del subarreglo derecho
    while (rightIndex < rightSize) {
        arr[mergeIndex] = rightArray[rightIndex];
        rightIndex++;
        mergeIndex++;
    }

    delete[] leftArray;
    delete[] rightArray;
}

// left es el índice izquierdo y right el índice derecho del subarreglo a ordenar
void mergeSort(PagedArray& arr, int left, int right){

    if (left >= right)
        return;

    // Calcula el punto medio evitando desbordamiento
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    // Combina las dos mitades ordenadas
    merge(arr, left, mid, right);
}