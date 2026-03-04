#include <stdio.h>
#include <cuda.h>

#define N 4 // Matrix dimension (N x N)

// 2a. Each row computed by one thread
__global__ void mulMatrixByRow(int *A, int *B, int *C) {
    int row = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (row < N) {
        for (int col = 0; col < N; col++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[row * N + k] * B[k * N + col];
            }
            C[row * N + col] = sum;
        }
    }
}

// 2b. Each column computed by one thread
__global__ void mulMatrixByCol(int *A, int *B, int *C) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (col < N) {
        for (int row = 0; row < N; row++) {
            int sum = 0;
            for (int k = 0; k < N; k++) {
                sum += A[row * N + k] * B[k * N + col];
            }
            C[row * N + col] = sum;
        }
    }
}

// 2c. Each element computed by one thread
__global__ void mulMatrixByElement(int *A, int *B, int *C) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    
    if (row < N && col < N) {
        int sum = 0;
        for (int k = 0; k < N; k++) {
            sum += A[row * N + k] * B[k * N + col];
        }
        C[row * N + col] = sum;
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

    // Initialize host matrices (Simple identity-like test)
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            h_A[i * N + j] = i + j; 
            h_B[i * N + j] = (i == j) ? 1 : 0; // Identity matrix
        }
    }

    // Allocate device memory
    cudaMalloc((void**)&d_A, size);
    cudaMalloc((void**)&d_B, size);
    cudaMalloc((void**)&d_C, size);

    // Copy to device
    cudaMemcpy(d_A, h_A, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_B, h_B, size, cudaMemcpyHostToDevice);

    printf("--- MATRIX MULTIPLICATION ---\n\n");

    // --- 2a: By Row ---
    mulMatrixByRow<<<1, N>>>(d_A, d_B, d_C);
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    printMatrix(h_C, "Result (By Row)");

    // --- 2b: By Column ---
    mulMatrixByCol<<<1, N>>>(d_A, d_B, d_C);
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    printMatrix(h_C, "Result (By Col)");

    // --- 2c: By Element ---
    dim3 threadsPerBlock(N, N);
    mulMatrixByElement<<<1, threadsPerBlock>>>(d_A, d_B, d_C);
    cudaMemcpy(h_C, d_C, size, cudaMemcpyDeviceToHost);
    printMatrix(h_C, "Result (By Element)");

    // Cleanup
    cudaFree(d_A); cudaFree(d_B); cudaFree(d_C);
    return 0;
}