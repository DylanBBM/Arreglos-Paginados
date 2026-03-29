#ifndef RADIXSORT_H
#define RADIXSORT_H

#include "../sorter/PagedArray.h"
int getMax(PagedArray &arr, int n);
void countSort(PagedArray &arr, int n, int exp);
void radixsort(PagedArray &arr, int n);
#endif