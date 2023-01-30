#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
int size = 2;
int p = 0;
int b = 4;
int array[2][2] = {{1, 2}, {3, 4}};
int resultofp2[2][2] = {{1, 2}, {3, 4}};
int resultofp3[2][2] = {{1, 2}, {3, 4}};

typedef struct
{
    int *data;
} myarg_t;
pthread_barrier_t barrier;

static void *poweringusebarrier(void *arg)
{
    myarg_t *m = (myarg_t *)arg;
    int i, j, k;
    int *data = m->data;

    i = data[0];
    j = data[1];
    int sum = 0;
    // operation one deraye
    for (k = 0; k < size; k++)
    {
        sum = sum + array[i][k] * resultofp2[k][j];
    }
    resultofp2[i][j] = sum;
    pthread_barrier_wait(&barrier);
    int sum2 = 0;
    for (k = 0; k < size; k++)
    {
        sum2 = sum2 + array[i][k] * resultofp3[k][j];
    }
    resultofp3[i][j] = sum2;

    return NULL;

    // struct v *data = (struct v *)arg;

    // size_t l;
    // for(l=0; l < N; l++)
    // {
    //     size_t i=(data[l]).i;
    //     size_t j=(data[l]).j;
    //     double sum=0;
    //     size_t d;

    //     for (d = 0; d < N; d++)
    //     {
    //         sum = sum + A[i][d]*B[d][j];
    //     }

    //     C[i][j] = sum;
    //     sum = 0;
    // }
    // return 0;
}

int main(void)
{

    //     size_t i, k;

    //     struct v **data;

    //     // variables
    //   int n;

    // take row and column size
    //   printf("Ener size: ");
    //   scanf("%d", &n);
    //     pthread_t threads[n];
    //   // declare array
    //   int arr[n][n];

    //   // take matrix elements as input
    //   printf("Enter elements for %dx%d matrix:\n", n, n);
    //   for(int i=0;i<n;i++)
    //   {
    //     for(int j=0;j<n;j++)
    //     {
    //       printf("arr[%d][%d]: ",i,j);
    //       scanf("%d", &arr[i][j]);
    //     }
    //     printf("\n");
    //   }
    pthread_t threads[size];
    pthread_barrier_init(&barrier, NULL, b);
    myarg_t *m = (myarg_t *)malloc(sizeof(myarg_t));
    int data[2];
    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            data[0] = i;
            data[1] = k;
        }

        m->data = data;

        pthread_create(&threads[i], NULL, poweringusebarrier, m);
    }
    // for (int i = 0; i < 2; i++)
    // {
    //     for (int j = 0; j < 2; j++)
    //     {
    //         myarg_t *m = (myarg_t *)malloc(sizeof(myarg_t));
    //         int *data = (int *)malloc(2 * sizeof(int));
    //         data[0] = i;
    //         data[1] = j;
    //         m->data = data;
    //         pthread_create(&threads[i], NULL, poweringusebarrier, m);
    //     } 
    // }
    
    for (int i = 0; i < size; i++)
    {
        pthread_join(threads[i], NULL);
    }
    for (int i = 0; i < size; i++)
    {
        for (int k = 0; k < size; k++)
        {
            printf("%d\t", array[i][k]);
        }

        printf("\n");
    }
    free(m);
    // int **data;
    // data = (int **)malloc(N * sizeof(int *));
    // for (i = 0; i < N; i++)
    // {
    //     data[i] = (int *)malloc(N * sizeof(int));
    // }

    // for(i = 0; i < n; i++)
    // {
    //     pthread_join(threads[i], NULL);
    // }

    // for (i = 0; i < N; i++)
    // {
    //     for (k = 0; k < N; k++)
    //     {
    //         printf("%lf\t", C[i][k]);
    //     }

    //     printf("\n");

    //     free(data[i]);
    // }

    return 0;
}