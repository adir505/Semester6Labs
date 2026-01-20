//MPI program to read a string and find the number of non vowel charecters.
//Uses N processes and strlen is divisible by N.
//Root prints number found by all processes and total non vowel charecters.
#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include <ctype.h>

int is_vowel(char c) {
    c = tolower(c);
    return (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u');
}
int main(int argc, char** argv) {
    int rank, size;
    char str[100];
    int total_non_vowels = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        printf("Enter a string: ");
        fgets(str, sizeof(str), stdin);
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
    char local_str[chunk_size + 1];

    // Scatter the string to all processes
    MPI_Scatter(str, chunk_size, MPI_CHAR, local_str, chunk_size, MPI_CHAR, 0, MPI_COMM_WORLD);
    local_str[chunk_size] = '\0'; // Null-terminate the local string

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

    MPI_Finalize();
    return 0;
}