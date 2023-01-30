#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void swap(int* a,int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        int i, j, k;
        int n1 = m - l + 1;
        int n2 = r - m;
        int L[n1], R[n2];
        for (i = 0; i < n1; i++)
            L[i] = arr[l + i];
        for (j = 0; j < n2; j++)
            R[j] = arr[m + 1 + j];
        i = 0;
        j = 0;
        k = l;
        while (i < n1 && j < n2) {
            if (L[i] <= R[j]) {
                arr[k] = L[i];
                i++;
            }
            else {
                arr[k] = R[j];
                j++;
            }
            k++;
        }
        while (i < n1) {
            arr[k] = L[i];
            i++;
            k++;
        }
        while (j < n2) {
            arr[k] = R[j];
            j++;
            k++;
        }

    }
}

void Merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1; // size of L (left array)
    int n2 = r - m; // size of R (right array)
    int L[n1], R[n2];
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];
    i = 0; // for L (left)
    j = 0; // for R (right)
    k = l; // for arr (merged)
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

// sort array with vfork()
int main(int argc, char const *argv[])
{
    int n = 0, m = 8;
    printf("Enter the number of elements in the array: ");
    scanf("%d", &n);
    printf("Number of elements must be dividable by number of threads.\n");
    printf("Enter the number of processes to be created: ");
    scanf("%d", &m);
    int *array = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
    {
        printf("Enter the element %d: ", i);
        scanf("%d", &array[i]);
    }

    // sort the slices of array
    for (int i = 0; i < m; i++)
    {
        int r = vfork();
        if (r == 0) // Child Process
        {
            int slice_unit = n / m;
            int start = i * slice_unit;
            int end = (i + 1) * slice_unit;
            for (int j = start; j < end; j++)
            {
                for (int k = j + 1; k < end; k++) // selection sort
                {
                    if (array[j] > array[k])
                    {
                        swap(&array[j], &array[k]);
                    }
                }
            }
            exit(0);
        }
    }
    // merge sort
    // mergeSort(array, 0, n - 1);

    for (int i = 0; i < m - 1; i++)
    {
        Merge(array, 0, (i + 1) * (n / m) - 1, (i + 2) * (n / m) - 1);
    }
    
    
    // print sorted array
    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");

    return 0;
}
