//Program reads 3X3 matrix. Element is entered by user to be searched, in root using MPI. Count of total occurences of the element has to be found, using 3 processes.
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != 3) {
        if (rank == 0) {
            fprintf(stderr, "This program requires exactly 3 processes.\n");
        }
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    int matrix[3][3];
    int element_to_search;
    int local_count = 0;

    if (rank == 0) {
        printf("Enter elements of 3x3 matrix:\n");
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                scanf("%d", &matrix[i][j]);
            }
        }
        printf("Enter element to search: ");
        scanf("%d", &element_to_search);
    }

    MPI_Bcast(&element_to_search, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(matrix, 3, MPI_INT, matrix[rank], 3, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process searches its assigned row
    for (int j = 0; j < 3; j++) {
        if (matrix[rank][j] == element_to_search) {
            local_count++;
        }
    }

    int total_count;
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total occurrences of %d in the matrix: %d\n", element_to_search, total_count);
    }

    MPI_Finalize();
    return 0;
}