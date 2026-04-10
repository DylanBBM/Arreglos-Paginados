#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "../sorter/PagedArray.h"

void heapify(PagedArray& arr, int size, int rootIndex);
void heapSort(PagedArray& arr, int size);

#endif