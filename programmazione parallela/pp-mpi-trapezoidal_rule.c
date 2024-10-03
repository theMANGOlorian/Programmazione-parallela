/*
    [PROGRAMMAZIONE PARALLELA | Trapezoidal Rule]
    calcolo dell'area sotto una funzione usando il metodo dei trapezi con la programmazione
    parallela.
    n rappresenta il numero di trapezi da usare e vengono spartiti per il numero di
    processi paralleli.

    Tecnologia usata per la programmazione parallela: MPI
*/


#include <stdio.h>
#include <mpi.h> 
/* 
    N.B per eseguire il codice C con mpi
    bisogna usare mpicc e non gcc

    per eseguire il codice usare mpiexec -n <numero> ./nome-file (*si piu usare anche mpirun)
*/

double f(double x){
    return x*x; //funzione da integrare (i.e. f(x) = x^2)
}

double Trap(double left, double right, int count, double base_len){
    double estimate,x;
    estimate = ( f(left) + f(right) )/2.0;
    for (int i = 1; i <= count-1; i++){
        x = left + i*base_len;
        estimate += f(x);
    }
    estimate = estimate*base_len;
    return estimate;
}

int main(){

    int my_rank, comm_size, n = 10000000, local_n; // sono i numeri di trapezi da calcolare (piu sono maggiore è preciso il calcolo dell'area)
    double a = 0.0, b = 3.0, h, local_a, local_b; // intervallo [a,b] da integrare
    double local_int, total_int;
    

    MPI_Init(NULL,NULL); // Tells MPI to do all necessary set up
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // numero di processi
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // numero del processo [0 .. p-1]

    h = (b-a)/n; //h è uguale per ogni processo
    local_n = n / comm_size; //numero dei trapezoidi

    local_a = a + my_rank*local_n*h;
    local_b = local_a + local_n * h;
    local_int = Trap(local_a,local_b,local_n,h);

    if (my_rank == 0){
        total_int = local_int;

        for (int source=1; source < comm_size; source++){
            MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total_int += local_int;
        }

        printf("With n = %d trapezoids, our estimate\n",n);
        printf("of the integral from %f to %f = %.15e\n",a,b,total_int);
        
    }else{
        MPI_Send(&local_int,1,MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    
    MPI_Finalize(); // Tells MPI we're done
    return 0;
}

