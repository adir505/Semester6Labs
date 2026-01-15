#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

long long factorial(int n) {
    long long fact = 1;
    for (int i = 2; i <= n; i++)
        fact *= i;
    return fact;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n;

    if (rank == 0) {
        if (argc < 2) {
            fprintf(stderr, "Usage: mpirun -np <p> %s <n>\n", argv[0]);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        n = atoi(argv[1]);
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    long long local_sum = (rank < n) ? factorial(rank + 1) : 0;

    long long scan_sum;
    MPI_Scan(&local_sum, &scan_sum, 1, MPI_LONG_LONG_INT, MPI_SUM, MPI_COMM_WORLD);

    if (rank < n) {
        printf("Rank %d: %d! = %lld\n", rank, rank + 1, local_sum);
    }

    if (rank == n - 1) {
        printf("Sum of factorials (1! to %d!) = %lld\n", n, scan_sum);
    }

    MPI_Finalize();
    return 0;
}
