//Template
#include <stdio.h>
#include <stdlib.h>
#define SIZE 10 // sizeof(n_array) / sizeof(int)

int compare(int a, int b)
{
    if (a > b)
        return 1;
    else
        return 0;
}

void BubbleSort(int *arr)
{
    for (int i = 0; i < SIZE - 1; i++)
    {
        for (int j = 0; j < SIZE - i - 1; j++)
        {
            if (compare(arr[j], arr[j + 1]))
            {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

/***************************************************************
define bubble sort that uses pointer to 'compare' function above
***************************************************************/

int main()
{
    int n_array[SIZE] = {2, -9, 10, 15, 1, 3, -12, 5, 4, 1};

    //source code
    BubbleSort(n_array);
    for (int i = 0; i < SIZE; i++)
    {
        printf("%d ", n_array[i]);
    }
    return 0;
}
