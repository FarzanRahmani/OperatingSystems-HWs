#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
    int size_of_array = 0;
    int *array = NULL;
    scanf("%d", &size_of_array);
    array = (int *)malloc(size_of_array * sizeof(int));
    for (int i = 0; i < size_of_array; i++) {
        scanf("%d", &array[i]);
    }
    int max_profit = 0;
    for (int len = 0; len <= size_of_array; len++) {
        for (int start = 0; start <= size_of_array - len; start++) {
            int sum = 0;
            for (int j = start; j < start + len; j++) {
                sum += array[j];
            }
            if (sum > max_profit) {
                max_profit = sum;
            }
        }
    }
    printf("%d\n", max_profit);
}