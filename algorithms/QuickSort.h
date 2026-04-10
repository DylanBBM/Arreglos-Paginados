
#ifndef QUICKSORT_H
#define QUICKSORT_H

#include "../sorter/PagedArray.h"

int partition(PagedArray& arr, int low, int high);
void quickSort(PagedArray& arr, int low, int high);

#endif