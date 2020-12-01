#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

//#define DEBUG 1            // printar ou não array antes e depois (comentar para medir tempo)
#define ARRAY_SIZE 10000      // número de elmentos no vetor
#define TROCA_SIZE 300 // número de elementos que serão trocados com vizinho

// função para ordenar um vetor com bubble sort
// parâmetros: int n - número de elementos no vetor
//             int *vetor - ponteiro para primeira posição do vetor
// retorno: void
void bs(int n, int *vetor)
{
    int c = 0, d, troca, trocou = 1;

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

int main(int argc, char **argv)
{
    int my_rank, proc_n; // meu rank e número de processos
    MPI_Status status; // status retornado

    double t1, t2;

    MPI_Init (&argc , & argv); // inicializo MPI

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pego meu rank
    MPI_Comm_size(MPI_COMM_WORLD, &proc_n); // pego número de processos

    if(my_rank == 0) 
    {
        t1 = MPI_Wtime(); // capturo o tempo de início
    }
    int i, j; // iteradores

    int received; // recebido do cara da esquerda
    int my_ok = 0; // meu ok -> 1 = OK, 0 = NOK
    int oks = 0; // contador de respostas OK

    int pronto = 0; // variável de controle para saber se as fases acabaram
    int meu_tam = ARRAY_SIZE/proc_n; // tamanho do meu vetor local
    int *vetor = malloc(sizeof(int)*meu_tam); // vetor de tamanho meu_tam
    printf("%d\n", meu_tam);
    int tam_geral = meu_tam; // tamanho do vetor local dos processos que não são o último
    if(my_rank == proc_n-1) // sou o último processo
    {
        meu_tam += ARRAY_SIZE % proc_n; // a meu tamanho é adicionado o que faltou pela divisão inteira
    }

    int start_index = my_rank * tam_geral;

    for (j = 0, i = start_index ; j < meu_tam, i < (start_index + meu_tam); i++, j++) // incializo vetor local no pior caso (invertido)
    {
        vetor[j] = ARRAY_SIZE-i;
    }

    #ifdef DEBUG
    printf("\nProcesso %d - Vetor Original: ", my_rank);
    for (i=0 ; i<meu_tam; i++) // imprimo array invertido
    {
        printf("[%03d] ", vetor[i]);
    }
    printf("\n");
    #endif

    while(!pronto) // ainda não está totalmente ordenado
    {
        // FASE 1

        bs(meu_tam, vetor); // ordeno o vetor local com o bubble        

        // FASE 2

        if(my_rank < proc_n - 1)
        {
            MPI_Send(&vetor[meu_tam-1], 1, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD); // mando meu maior pro processo da direita
        }
        oks = 0; // inicializo contador de oks
        my_ok = 0; // começo como não ok
        if(my_rank > 0) // não sou o mais da esquerda, tenho coisa para receber da esquerda
        {
            MPI_Recv(&received, 1, MPI_INT, my_rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo o maior do processo da esquerda

            if(vetor[0] > received) // o meu menor é maior do que o maior da esquerda
            {
                my_ok = 1; // estou ok
            }
        }
        else // sou o mais da esquerda
        {
            my_ok = 1; // estou ok
        }

        for(i = 0; i < proc_n; i++) // iterando para fazer o broadcast
        {
            if(i == my_rank) // é minha vez
            {
                MPI_Bcast(&my_ok, 1, MPI_INT, i, MPI_COMM_WORLD); // mando o meu ok
                oks += my_ok; // somo meu ok no contador de oks
            }
            else // tenho que receber de alguém
            {
                MPI_Bcast(&received, 1, MPI_INT, i, MPI_COMM_WORLD); // recebo o ok de alguém
                oks += received; // somo o ok recebido no contador de oks
            }            
        }

        if(oks == proc_n) // está todo mundo ok
        {
            pronto = 1; // terminou
            break; // saio do while
        }

        // FASE 3

        int received_array[TROCA_SIZE]; // o que vou receber
        int buffer[TROCA_SIZE*2]; // o que vou receber mais os meus mais altos para ordenar
        int out_array[TROCA_SIZE]; // o que vou mandar

        if(my_rank > 0) // não sou o zero, tenho vizinhos à esquerda
        {
            for(i = 0; i < TROCA_SIZE; i++)
            {
                out_array[i] = vetor[i];
            }

            MPI_Send(&out_array, TROCA_SIZE, MPI_INT, my_rank-1, 0, MPI_COMM_WORLD); // mando meus menores pro vizinho da esquerda

        }

        if(my_rank < proc_n - 1) // não sou o último, tenho vizinhos à direita
        {
            MPI_Recv(&received_array, TROCA_SIZE, MPI_INT, my_rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo os menores do vizinho da direita
            if(vetor[meu_tam-1] < received_array[0]) // já estou ordenado com o vizinho da direita
            {
                MPI_Send(&received_array, TROCA_SIZE, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD); // devolvo o vetor original que o vizinho mandou
            }
            else // não está ordenado, tenho que ordenar
            {
                for(j = meu_tam-TROCA_SIZE, i = 0; j < meu_tam && i < TROCA_SIZE; i++, j++)
                {
                    buffer[i] = vetor[j]; // pego parte alta do meu vetor e coloco na parte baixa do buffer
                }
                for(j = TROCA_SIZE, i = 0; j < (TROCA_SIZE*2) && i < TROCA_SIZE; i++, j++) 
                {
                    buffer[j] = received_array[i]; // pego o vetor do vizinho e coloco na parte alta do buffer
                }

                bs(TROCA_SIZE*2, buffer); // ordeno o buffer

                for(j = meu_tam-TROCA_SIZE, i = 0; j < meu_tam && i < TROCA_SIZE; i++, j++)
                {
                    vetor[j] = buffer[i] ; // guardo parte baixa do buffer ordenado no alto do meu vetor
                }

                for(j = 0, i = TROCA_SIZE; i < TROCA_SIZE*2; i++, j++)
                {
                    out_array[j] = buffer[i]; // monto o vetor de retorno pro cara da direita com a parte alta do buffer
                }

                MPI_Send(&out_array, TROCA_SIZE, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD); // devolvo parte alta do buffer ordenado pro vizinho da direita
            }            
        }

        if(my_rank > 0) // não sou o 0, tenho vizinhos à esquerda
        {
            MPI_Recv(&received_array, TROCA_SIZE, MPI_INT, my_rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo de volta do vizinho da esquerda
            for(i = 0; i < TROCA_SIZE; i++)
            {
                vetor[i] = received_array[i]; // boto os que recebi no lugar dos meus menores
            }
        }
        
    }

    #ifdef DEBUG
    printf("\nProcesso %d - Vetor Ordenado: ", my_rank);
    for (i=0 ; i<meu_tam; i++) // imprimo array ordenado
    {
        printf("[%03d] ", vetor[i]);
    }
    printf("\n");
    #endif

    if(my_rank == 0) 
    {
        t2 = MPI_Wtime(); // capturo o tempo de término
        printf("\nTime: %lf seconds\n\n", t2-t1);
    }

    MPI_Finalize();

    return 0; // fim
}
