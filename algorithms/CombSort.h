#ifndef COMBSORT_H
#define COMBSORT_H

#include "../sorter/PagedArray.h"

int getNextGap(int gap);
void combSort(PagedArray& arr, int size);

#endif