
#include <stdio.h>
#include <mpi.h> 
/* 
    N.B per eseguire il codice C con mpi
    bisogna usare mpicc e non gcc

    per eseguire il codice usare mpiexec -n <numero> ./nome-file
*/

int main(){

    int comm_size, my_rank;
    char text[1024];

    MPI_Init(NULL,NULL); // Tells MPI to do all necessary set up


    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // numero di processi
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // numero del processo [0 .. p-1]

    if (my_rank == 0){
        MPI_Status status;
        printf("Hello world from %d out of %d\n", my_rank, comm_size);

        for (int i=1; i < comm_size; i++){

            int error_code = MPI_Recv(text, 1024, MPI_CHAR, i, 0, MPI_COMM_WORLD, &status); // riceve <text> dal processo i
            if (error_code != MPI_SUCCESS){
                printf("Error MPI rilevato nello status\n");
            }
            printf("%s , source: %d , tag: %d\n", text,status.MPI_SOURCE,status.MPI_TAG);
        }
        
    }else{
        sprintf(text,"Hello world from %d out of %d", my_rank, comm_size);
        MPI_Send(text,1024,MPI_CHAR, 0, 0, MPI_COMM_WORLD); // invia <text> al processo p=0
    }

    
    MPI_Finalize(); // Tells MPI we're done
    return 0;
}


/*
N.B.
MPI_Send(buffer,buffer_size,data_type,dst_process,tag,communicator)
MPI_Recv(buffer,buffer_size,data_type,src_process,tag_filter,communicator,status)

MPI_Get_count(&status,MPI_INT,&count);
*/
