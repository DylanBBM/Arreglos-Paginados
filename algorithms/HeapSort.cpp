#include <iostream>
#include "HeapSort.h"
using namespace std;

// Reorganiza el subárbol para mantener la propiedad de max-heap
void heapify(PagedArray& arr, int size, int rootIndex)
{
    int largest = rootIndex;
    int leftChild = 2 * rootIndex + 1;
    int rightChild = 2 * rootIndex + 2;

    // Verifica si el hijo izquierdo es mayor que la raíz
    if (leftChild < size && arr[leftChild] > arr[largest])
        largest = leftChild;

    // Verifica si el hijo derecho es mayor que el mayor actual
    if (rightChild < size && arr[rightChild] > arr[largest])
        largest = rightChild;

    // Si el mayor no es la raíz, intercambia y sigue reorganizando recursivamente
    if (largest != rootIndex) {
        int temp = arr[rootIndex];
        arr[rootIndex] = arr[largest];
        arr[largest] = temp;

        heapify(arr, size, largest);
    }
}

void heapSort(PagedArray& arr, int size)
{
    // Construye el max-heap reorganizando desde el último nodo interno hasta la raíz
    for (int rootIndex = size / 2 - 1; rootIndex >= 0; rootIndex--)
        heapify(arr, size, rootIndex);

    // Extrae uno por uno los elementos del heap
    for (int last = size - 1; last >= 0; last--) {
        // Mueve la raíz (mayor elemento) al final del arreglo
        int temp = arr[0];
        arr[0] = arr[last];
        arr[last] = temp;

        // Reorganiza el heap reducido
        heapify(arr, last, 0);
    }
}