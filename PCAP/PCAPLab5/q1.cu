#include <stdio.h>
#include <cuda_runtime.h>

#define N 512

__global__ void vectorAdd(const int *a, const int *b, int *c, int n) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;

    if (i < n) {
        c[i] = a[i] + b[i];
    }
}

void checkResult(int *host_c, int expected_val) {
    for (int i = 0; i < N; i++) {
        if (host_c[i] != expected_val) {
            printf("Error at index %d: Expected %d, Got %d\n", i, expected_val, host_c[i]);
            return;
        }
    }
    printf("Success!\n");
}

int main() {
    int *h_a, *h_b, *h_c;
    int *d_a, *d_b, *d_c;
    size_t size = N * sizeof(int);

    h_a = (int *)malloc(size);
    h_b = (int *)malloc(size);
    h_c = (int *)malloc(size);

    for (int i = 0; i < N; i++) {
        h_a[i] = 1;
        h_b[i] = 2;
    }

    cudaMalloc((void **)&d_a, size);
    cudaMalloc((void **)&d_b, size);
    cudaMalloc((void **)&d_c, size);

    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    printf("Scenario A: Launching 1 Block with %d Threads...\n", N);
    
    vectorAdd<<<1, N>>>(d_a, d_b, d_c, N);
    
    cudaDeviceSynchronize(); // Wait for GPU to finish
    
    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);
    checkResult(h_c, 3);

    cudaMemset(d_c, 0, size);

    int blockSize = 256;
    int numBlocks = (N + blockSize - 1) / blockSize;

    printf("Scenario B: Launching %d Blocks with %d Threads each...\n", numBlocks, blockSize);

    vectorAdd<<<numBlocks, blockSize>>>(d_a, d_b, d_c, N);
    
    cudaDeviceSynchronize();

    cudaMemcpy(h_c, d_c, size, cudaMemcpyDeviceToHost);
    checkResult(h_c, 3);

    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
    free(h_a); free(h_b); free(h_c);

    return 0;
}