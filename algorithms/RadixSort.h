#ifndef RADIXSORT_H
#define RADIXSORT_H

#include "../sorter/PagedArray.h"

void countSort(PagedArray& arr, size_t size, int exp);
void radixSort(PagedArray& arr);

#endif