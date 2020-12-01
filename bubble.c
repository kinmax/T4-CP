#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//#define DEBUG 1            // comentar esta linha quando for medir tempo
#define ARRAY_SIZE 10000      // trabalho final com o valores 10.000, 100.000, 1.000.000

void bs(int n, int * vetor)
{
    int c=0, d, troca, trocou =1;

    while (c < (n-1) & trocou )
    {
        trocou = 0;
        for (d = 0 ; d < n - c - 1; d++)
        {
            if (vetor[d] > vetor[d+1])
            {
                troca      = vetor[d];
                vetor[d]   = vetor[d+1];
                vetor[d+1] = troca;
                trocou = 1;
            }
        }
        c++;
    }
}

int main()
{
    clock_t t1, t2;
    t1 = clock();
    int vetor[ARRAY_SIZE];
    int i;

    for (i=0 ; i<ARRAY_SIZE; i++)              /* init array with worst case for sorting */
    {
        vetor[i] = ARRAY_SIZE-i;
    }
   

    #ifdef DEBUG
    printf("\nVetor: ");
    for (i=0 ; i<ARRAY_SIZE; i++)              /* print unsorted array */
    {
        printf("[%03d] ", vetor[i]);
    }
    #endif

    bs(ARRAY_SIZE, vetor);                     /* sort array */


    #ifdef DEBUG
    printf("\nVetor: ");
    for (i=0 ; i<ARRAY_SIZE; i++)                              /* print sorted array */
    {
        printf("[%03d] ", vetor[i]);
    }
    #endif

    t2 = clock();

    printf("Time: %lf seconds\n", (double)(t2 - t1) / CLOCKS_PER_SEC);

    return 0;
}
