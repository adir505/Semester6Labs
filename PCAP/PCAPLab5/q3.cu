#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cuda_runtime.h>

__global__ void sineKernel(const float *angles, float *sines, int n) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;

    if (i < n) {
        sines[i] = sinf(angles[i]);
    }
}

int main() {
    int N = 1000000;
    size_t size = N * sizeof(float);

    float *h_angles = (float *)malloc(size);
    float *h_sines = (float *)malloc(size);

    for (int i = 0; i < N; i++) {
        h_angles[i] = (float)i * 0.01f;
    }

    float *d_angles = NULL;
    float *d_sines = NULL;

    cudaMalloc((void **)&d_angles, size);
    cudaMalloc((void **)&d_sines, size);

    cudaMemcpy(d_angles, h_angles, size, cudaMemcpyHostToDevice);

    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

    sineKernel<<<blocksPerGrid, threadsPerBlock>>>(d_angles, d_sines, N);

    cudaMemcpy(h_sines, d_sines, size, cudaMemcpyDeviceToHost);

    for (int i = 0; i < 10; i++) {
        if (fabs(sinf(h_angles[i]) - h_sines[i]) > 1e-5) {
            printf("Verification failed at index %d\n", i);
            exit(EXIT_FAILURE);
        }
    }
    printf("Success\n");

    cudaFree(d_angles);
    cudaFree(d_sines);
    free(h_angles);
    free(h_sines);

    return 0;
}