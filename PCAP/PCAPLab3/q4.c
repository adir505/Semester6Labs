//Program reads 2 strings of same length in root process. Using N processes including root process,
//alternate each character of both strings and store in result string. Print result string in root process.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#define MAX 100

int main(int argc, char *argv[]) {
    int rank, size, len, i;
    char str1[MAX], str2[MAX], result[MAX * 2];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Read two strings from user
        printf("Enter first string: ");
        scanf("%s", str1);
        printf("Enter second string: ");
        scanf("%s", str2);

        len = strlen(str1);
        if (len != strlen(str2)) {
            printf("Strings must be of the same length.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    // Broadcast length of strings to all processes
    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Broadcast both strings to all processes
    MPI_Bcast(str1, MAX, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(str2, MAX, MPI_CHAR, 0, MPI_COMM_WORLD);

    // Each process computes its part of the result
    for (i = rank; i < len; i += size) {
        result[2 * i] = str1[i];
        result[2 * i + 1] = str2[i];
    }

    // Gather results at root process
    MPI_Gather(&result[rank * 2], (len / size) * 2 + (rank < len % size ? 2 : 0), MPI_CHAR,
               result, (len / size) * 2 + (rank < len % size ? 2 : 0), MPI_CHAR,
               0, MPI_COMM_WORLD);

    if (rank == 0) {
        result[2 * len] = '\0'; // Null-terminate the result string
        printf("Resultant string: %s\n", result);
    }

    MPI_Finalize();
    return 0;
}