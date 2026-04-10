#include "CombSort.h"


// Calcula el siguiente intervalo reduciendo por el factor 1.3
int getNextGap(int gap)
{
    gap = (gap * 10) / 13;
    if (gap < 1)
        return 1;
    return gap;
}

void combSort(PagedArray& arr, int size)
{
    int gap = size;
    bool swapped = true;

    // Continúa mientras el intervalo sea mayor a 1 o haya habido intercambios
    while (gap != 1 || swapped == true)
    {
        gap = getNextGap(gap);
        swapped = false;

        // Compara elementos separados por el intervalo actual
        for (int currentIndex = 0; currentIndex < size - gap; currentIndex++)
        {
            if (arr[currentIndex] > arr[currentIndex + gap])
            {
                int temp = arr[currentIndex];
                arr[currentIndex] = arr[currentIndex + gap];
                arr[currentIndex + gap] = temp;
                swapped = true;
            }
        }
    }
}