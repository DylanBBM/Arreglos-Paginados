#ifndef HEAPSORT_H
#define HEAPSORT_H

#include "../sorter/PagedArray.h"

void heapify(PagedArray &arr, int n, int i);
void heapSort(PagedArray &arr, int n);


#endif