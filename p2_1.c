#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 64
#define PARTIAL_SIZE 16

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int size, rank;
    int *numbers;
    //initialize local and global sums for each processor
    int local_sum = 0;
    int global_sum = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //set aside memory of numbers array
    numbers = (int *)malloc(sizeof(int) * ARRAY_SIZE);
    //open file and read numbers on each
    FILE *file = fopen("number.txt", "r");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        //read numbers.txt into numbers array
        fscanf(file, "%d\n", &numbers[i]); 
    }
    fclose(file);
    //calculate the partial sums for each part of the numbers array
    for (int i = rank * PARTIAL_SIZE; i < (rank + 1) * PARTIAL_SIZE; i++) {
        local_sum += numbers[i];
    }
    //if the processor is 0, sum onto the total sum
    if (rank == 0) {
        int received_sum;
        for (int sender = 1; sender < size; sender++) {
            MPI_Recv(&received_sum, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += received_sum;
        }
        //add its own sum to the global
        global_sum += local_sum; 
        printf("Total sum: %d\n", global_sum);
    } else {
        //send partial sum to processor 0
        MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    free(numbers);
    MPI_Finalize();
    return 0;
}