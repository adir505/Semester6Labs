#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX 100

int main(int argc, char *argv[]) {
    int rank, size, len, i;
    char str1[MAX], str2[MAX];
    char *local_result, *final_result;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        fprintf(stdout, "Enter first string: ");
        fflush(stdout);
        scanf("%s", str1);

        fprintf(stdout, "Enter second string: ");
        fflush(stdout);
        scanf("%s", str2);

        len = strlen(str1);
        if (len != strlen(str2)) {
            fprintf(stderr, "Strings must be of same length\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
    }

    MPI_Bcast(&len, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(str1, len + 1, MPI_CHAR, 0, MPI_COMM_WORLD);
    MPI_Bcast(str2, len + 1, MPI_CHAR, 0, MPI_COMM_WORLD);

    local_result = calloc(2 * len + 1, sizeof(char));

    for (i = rank; i < len; i += size) {
        local_result[2 * i]     = str1[i];
        local_result[2 * i + 1] = str2[i];
    }

    if (rank == 0)
        final_result = calloc(2 * len + 1, sizeof(char));

    MPI_Reduce(local_result, final_result,
               2 * len + 1, MPI_CHAR,
               MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Resultant string: %s\n", final_result);
        free(final_result);
    }

    free(local_result);
    MPI_Finalize();
    return 0;
}
