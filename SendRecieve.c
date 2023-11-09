#include <stdio.h>
#include <mpi.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int size = 4;
    int rank, Msg;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    //set parent processor with inital value
    if (rank == 0) {
        Msg = 451;
        printf("Process 0: Initially Msg = %d\n", Msg);
    } else {
        //recieve from previous processor and then increment
        MPI_Recv(&Msg, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        Msg++;
        printf("Process %d: Msg = %d\n", rank, Msg);
    }
    //if the processor is the last one, send to 0 that the "final" msg was sent
    if (rank == size - 1) {
        printf("Process 0: Received Msg = %d. Done!\n", Msg);
    //if not last one, send to the next processor
    } else {
        MPI_Send(&Msg, 1, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}
