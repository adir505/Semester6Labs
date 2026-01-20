#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include <ctype.h>

int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}
int main(int argc, char** argv) {
    int rank, size;
    char *str = NULL;
    int total_non_vowels = 0;

    if (MPI_Init(&argc, &argv) != MPI_SUCCESS) {
        fprintf(stderr, "Error initializing MPI\n");
        return EXIT_FAILURE;
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter a string: ");
        str = (char *)malloc(100 * sizeof(char)); // Initial allocation
        if (fgets(str, 100, stdin) == NULL) {
            fprintf(stderr, "Error reading input\n");
            MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
        }
        str[strcspn(str, "\n")] = 0; // Remove newline character
    }

    // Broadcast the string length to all processes
    int str_length;
    if (rank == 0) {
        str_length = strlen(str);
    }
    MPI_Bcast(&str_length, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the chunk size for each process
    int chunk_size = str_length / size;
    char *local_str = (char *)malloc((chunk_size + 1) * sizeof(char));

    // Scatter the string to all processes
    MPI_Scatter(str, chunk_size, MPI_CHAR, local_str, chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    // No need to null-terminate local_str after MPI_Scatter

    // Each process counts non-vowel characters in its chunk
    int local_non_vowels = 0;
    for (int i = 0; i < chunk_size; i++) {
        if (!is_vowel(local_str[i]) && isalpha(local_str[i])) {
            local_non_vowels++;
        }
    }

    // Reduce to get the total non-vowel count at root
    MPI_Reduce(&local_non_vowels, &total_non_vowels, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Total non-vowel characters: %d\n", total_non_vowels);
    }

    free(local_str);
    if (str) free(str);
    if (MPI_Finalize() != MPI_SUCCESS) {
        fprintf(stderr, "Error finalizing MPI\n");
        return EXIT_FAILURE;
    }
    return 0;
}
