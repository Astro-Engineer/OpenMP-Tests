#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 64
#define PARTIAL_SIZE 16

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int size, rank;
    int *numbers;
    int local_sum = 0;
    int global_sum = 0;
    //arrays for local and global sums
    int *local_numbers = NULL;
    int *global_sums = NULL;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    //set aside memory of numbers array and local numbers array
    numbers = (int *)malloc(sizeof(int) * ARRAY_SIZE);
    local_numbers = (int *)malloc(sizeof(int) * PARTIAL_SIZE);
    //process 0 reads the numbers.txt
    if (rank == 0) {
        FILE *file = fopen("number.txt", "r");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            //read numbers.txt into numbers array
            fscanf(file, "%d\n", &numbers[i]); 
        }
        fclose(file);
    }
    //send parts of array to other processors using scatter
    MPI_Scatter(numbers, PARTIAL_SIZE, MPI_INT, local_numbers, PARTIAL_SIZE, MPI_INT, 0, MPI_COMM_WORLD);
    //calculate local sums using local numbers from scatter
    for (int i = 0; i < PARTIAL_SIZE; i++) {
        local_sum += local_numbers[i];
    }
    //set aside memory for total amount and consolidate local sums using scatter
    if (rank == 0) {
        global_sums = (int *)malloc(sizeof(int) * size);
    }
    MPI_Gather(&local_sum, 1, MPI_INT, global_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);
    //compute total sum from gather
    if (rank == 0) {
        global_sum = 0;
        for (int i = 0; i < size; i++) {
            global_sum += global_sums[i];
        }
        printf("Total sum: %d\n", global_sum);
    }
    free(numbers);
    free(local_numbers);
    free(global_sums);
    MPI_Finalize();
    return 0;
}