#include <stdio.h>
#include <mpi.h> 

int main(){

    MPI_Request requests[4]; // Array per le richieste di invio/ricezione
    int send_left , send_right, recv_right, recv_left, comm_size, rank;

    MPI_Init(NULL, NULL); // Inizializza l'ambiente MPI
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // Ottieni il numero totale di processi
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); // Ottieni il rank del processo corrente

    // Imposta i valori da inviare
    send_right = rank;  // Valore da inviare a destra (al processo successivo)
    send_left = rank;   // Valore da inviare a sinistra (al processo precedente)

    // Avvia la comunicazione non bloccante
    // Invio a destra (al processo successivo)
    MPI_Isend(&send_right, 1, MPI_INT, (rank + 1) % comm_size, 0, MPI_COMM_WORLD, &requests[0]);
    
    // Invio a sinistra (al processo precedente)
    MPI_Isend(&send_left, 1, MPI_INT, (rank - 1 + comm_size) % comm_size, 0, MPI_COMM_WORLD, &requests[1]);

    // Ricezione da sinistra (dal processo precedente)
    MPI_Irecv(&recv_left, 1, MPI_INT, (rank - 1 + comm_size) % comm_size, 0, MPI_COMM_WORLD, &requests[2]);

    // Ricezione da destra (dal processo successivo)
    MPI_Irecv(&recv_right, 1, MPI_INT, (rank + 1) % comm_size, 0, MPI_COMM_WORLD, &requests[3]);

    // Aspetta che tutte le operazioni siano completate
    MPI_Waitall(4, requests, MPI_STATUSES_IGNORE);

    // Stampa i valori ricevuti
    printf("Processo %d ha ricevuto %d da sinistra e %d da destra\n", rank, recv_left, recv_right);

    MPI_Finalize(); // Termina l'ambiente MPI
    return 0;
}

/*
MPI_Send Type:

    MPI_Send: blocking send, it may use one of the modes

        Synchronous (MPI_Ssend, send when the recv is ready to accept the data)

        Buffered (MPI_Bsend, send data buffered, put it onto the network, and the receiver will start filling its receive buffer)

        Ready (MPI_Rsend, start sending data with the assumption that the receive is already listening for it), you need to be sure that the receive has already been posted.

    MPI_Send usually will pick Ssend or Bsend depending on the datasize to be sent.

    These are blocking and only return once the data has been sent off.

    MPI_Isend: non-blocking send, returns immediately (hence the I). 
        You get a request handle and need to wait on the communication operation to complete at some point. 
        Again the different modes may be used in combination with the nonblocking (so you get MPI_Ibsend, 
        MPI_Issend and MPI_Irsend). 

Check for completion :
    Waitall
    Waitany
    Testany
    Testany

MPI_Reduce:
    Consente di eseguire operazioni di riduzione in modo efficiente, centralizzando il risultato
    in un unico processo.

*/