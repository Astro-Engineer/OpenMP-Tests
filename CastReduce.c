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
    //process 0 reads the numbers.txt
    if (rank == 0) {
        FILE *file = fopen("number.txt", "r");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            //read numbers.txt into numbers array
            fscanf(file, "%d\n", &numbers[i]); 
        }
        fclose(file);
    }
    //send array to all processors
    MPI_Bcast(numbers, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    //calculate the partial sums for each part of the numbers array
    for (int i = rank * PARTIAL_SIZE; i < (rank + 1) * PARTIAL_SIZE; i++) {
        local_sum += numbers[i];
    }
    //use reduction to consolidate all local sums into global sum
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    //processor 0 prints final
    if (rank == 0) {
        printf("Total sum: %d\n", global_sum);
    }
    free(numbers);
    MPI_Finalize();
    return 0;
}
