#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>  // per rand() e srand()
#include <time.h>    // per time()

/* Esempio: mpirun a.out 1000 */

int main(int argc, char** argv){

    int comm_size, my_rank;
    long long int number_of_toss = atoll(argv[1]);  // Usa atoll per convertire in long long int

    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    srand(time(NULL) + my_rank);  // Assicura che ogni processo abbia un seme diverso

    long long int local_number_of_toss = number_of_toss / comm_size;
    long long int rest = number_of_toss % comm_size;

    if (my_rank < rest) {
        local_number_of_toss += 1;
    }
    printf("Process number %d, number of tosses %lld\n", my_rank, local_number_of_toss);
    
    long long int number_in_circle = 0;  // Usa long long int per il conteggio
    long long int total;  // Usa long long int per il totale
    double x, y, pi_estimated;

    for (long long int t = 0; t < local_number_of_toss; t++) {
        x = (double)rand() / RAND_MAX * 2.0 - 1.0;
        y = (double)rand() / RAND_MAX * 2.0 - 1.0;

        if (x * x + y * y <= 1) {
            number_in_circle++;
        }
    }

    // Riduci il conteggio con MPI_Reduce
    MPI_Reduce(&number_in_circle, &total, 1, MPI_LONG_LONG_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (my_rank == 0) {
        // Calcola pi con un maggior grado di precisione
        pi_estimated = 4.0 * total / ((double)number_of_toss);
        printf("pi : %.15lf\n", pi_estimated);  // Stampa con maggiore precisione
    }
    
    MPI_Finalize();

    return 0;
}
