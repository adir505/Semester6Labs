#include <stdio.h>
#include <cuda.h>

#define N 4 // Matrix dimension (N x N)

// 1a. Each row computed by one thread
__global__ void addMatrixByRow(int *A, int *B, int *C) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (row < N) {
        for (int col = 0; col < N; col++) {
            C[row * N + col] = A[row * N + col] + B[row * N + col];
        }
    }
}

// 1b. Each column computed by one thread
__global__ void addMatrixByCol(int *A, int *B, int *C) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (col < N) {
        for (int row = 0; row < N; row++) {
            C[row * N + col] = A[row * N + col] + B[row * N + col];
        }
    }
}

// 1c. Each element computed by one thread
__global__ void addMatrixByElement(int *A, int *B, int *C) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    
    if (row < N && col < N) {
        C[row * N + col] = A[row * N + col] + B[row * N + col];
    }
}

// Helper function to print a matrix
void printMatrix(int *mat, const char* name) {
    printf("Matrix %s:\n", name);
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d\t", mat[i * N + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int size = N * N * sizeof(int);
    int h_A[N*N], h_B[N*N], h_C[N*N];
    int *d_A, *d_B, *d_C;

    // Initialize host matrices
    for (int i = 0; i < N * N; i++) {
        h_A[i] = i;
        h_B[i] = 1; // Add 1 to every element for easy verification
    }

    // Allocate device memory
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy to device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    printf("--- MATRIX ADDITION ---\n\n");

    // --- 1a: By Row ---
    addMatrixByRow<<<1, N>>>(d_A, d_B, d_C);
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    printMatrix(h_C, "Result (By Row)");

    // --- 1b: By Column ---
    addMatrixByCol<<<1, N>>>(d_A, d_B, d_C);
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    printMatrix(h_C, "Result (By Col)");

    // --- 1c: By Element ---
    dim3 threadsPerBlock(N, N);
    addMatrixByElement<<<1, threadsPerBlock>>>(d_A, d_B, d_C);
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    printMatrix(h_C, "Result (By Element)");

    // Cleanup
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    return 0;
}