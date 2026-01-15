//Program to send a word and toggle each letter.
#include <mpi.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // We need exactly 2 processes for this to work
    if (world_size != 2) {
        if (world_rank == 0) 
            printf("World size must be exactly 2 for toggle_letter!\n");
        MPI_Finalize();
        return 0;
    }

    const int MAX_LEN = 100;
    char word[MAX_LEN];

    if (world_rank == 0) {
        // Initialize the word to send
        snprintf(word, MAX_LEN, "HelloWorld");
        printf("Rank 0 sending word: %s\n", word);
        MPI_Ssend(word, MAX_LEN, MPI_CHAR, 1, 0, MPI_COMM_WORLD);
    } else if (world_rank == 1) {
        // Receive the word
        MPI_Recv(word, MAX_LEN, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 1 received word: %s\n", word);

        // Toggle each letter's case
        for (int i = 0; word[i] != '\0'; i++) {
            if (word[i] >= 'a' && word[i] <= 'z') {
                word[i] = word[i] - ('a' - 'A'); // Convert to uppercase
            } else if (word[i] >= 'A' && word[i] <= 'Z') {
                word[i] = word[i] + ('a' - 'A'); // Convert to lowercase
            }
        }

        printf("Rank 1 toggled word: %s\n", word);
        MPI_Ssend(word, MAX_LEN, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }

    if (world_rank == 0) {
        // Receive the toggled word
        MPI_Recv(word, MAX_LEN, MPI_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank 0 received toggled word: %s\n", word);
    }

    MPI_Finalize();
    return 0;
}