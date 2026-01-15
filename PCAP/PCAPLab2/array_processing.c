#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv){
    int rank, size,i;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank==0){
        // Initialize array in r
        int N=size,bytes=sizeof(int),total_bytes=bytes+MPI_BSEND_OVERHEAD;
        int arr[N];
        void* buffer=malloc(total_bytes);
        MPI_Buffer_attach(buffer, total_bytes);
        for(i=0;i<N;i++){
            arr[i]=i;
            MPI_Bsend(&arr[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else{
        // Receive array from rank 0
        MPI_Recv(&i, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if(rank%2==0) i*=i;
        else i*=i*i;
        printf("Rank %d processed value: %d\n", rank, i);
    }
    MPI_Finalize();
    return 0;
}