#ifndef MERGESORT_H
#define MERGESORT_H

#include "../sorter/PagedArray.h"

void merge(PagedArray &arr, int left, int mid, int right);
void mergeSort(PagedArray &arr, int left, int right);

#endif