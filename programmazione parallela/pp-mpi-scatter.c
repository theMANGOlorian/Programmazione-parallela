#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>  // for malloc and rand
#include <time.h>    // for seeding rand with time

// Funzione per creare un vettore di numeri casuali
int* create_random_vector(int n,int seed) {
    // Allocazione dinamica del vettore
    int* vector = (int*)malloc(n * sizeof(int));
    
    if (vector == NULL) {
        return NULL; // Ritorna NULL se l'allocazione fallisce
    }

    // Seed per il generatore di numeri casuali
    srand(time(NULL) + seed); // Aggiungi n per differenziare i vettori generati

    // Popola il vettore con numeri casuali
    for (int i = 0; i < n; i++) {
        vector[i] = rand() % 100;  // Genera numeri casuali tra 0 e 99
    }

    return vector; // Restituisce il puntatore al vettore
}

// Funzione per stampare il vettore
void print_vector(int* vector, int n) {
    if (vector == NULL) {
        printf("Il vettore è vuoto (NULL).\n");
        return;
    }

    printf("Vettore: [");
    for (int i = 0; i < n; i++) {
        printf("%d", vector[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]\n");
}

int main(int argc, char** argv) {
    int rank, size;
    int *A = NULL, *B = NULL;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (argc < 2) {
        if (rank == 0) printf("Errore: specificare la dimensione del vettore.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int n = atoi(argv[1]);  // Dimensione totale del vettore

    // Verifica che n sia divisibile tra i processi
    if (n % size != 0) {
        if (rank == 0) printf("Errore: la dimensione del vettore non è divisibile per il numero di processi.\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int local_n = n / size;  // Dimensione del blocco per ogni processo

    // Il processo 0 crea i vettori
    if (rank == 0) {
        A = create_random_vector(n,600000);  // Vettore A
        B = create_random_vector(n,700);  // Vettore B
        printf("Vettore A e B generati dal processo root (rank 0):\n");
        print_vector(A, n);
        print_vector(B, n);
    }

    // Ogni processo ha un proprio spazio per i sottovettori di A e B
    int* local_A = (int*)malloc(local_n * sizeof(int));
    int* local_B = (int*)malloc(local_n * sizeof(int));

    // Distribuzione dei vettori A e B tra i processi
    MPI_Scatter(A, local_n, MPI_INT, local_A, local_n, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(B, local_n, MPI_INT, local_B, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    printf("rank %d A: ",rank);
    print_vector(local_A, local_n);
    printf("rank %d B: ",rank);
    print_vector(local_B, local_n);

    // Libera la memoria allocata
    free(local_A);
    free(local_B);
    if (rank == 0) {
        free(A);
        free(B);
    }

    MPI_Finalize();
    return 0;
}
