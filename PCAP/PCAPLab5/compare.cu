#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <cuda_runtime.h>

#define N 1000000  // Define data size

// --------------------------------------------------------
// 1. CPU IMPLEMENTATION
// --------------------------------------------------------
void host_function(float *a, float *b, float *c, int n) {
    // [INSERT CPU LOGIC HERE]
    // Example:
    for(int i = 0; i < n; i++) {
        c[i] = a[i] + b[i];
    }
}

// --------------------------------------------------------
// 2. GPU KERNEL
// --------------------------------------------------------
__global__ void device_kernel(float *a, float *b, float *c, int n) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i < n) {
        // [INSERT GPU LOGIC HERE]
        // Example:
        c[i] = a[i] + b[i];
    }
}

int main() {
    size_t size = N * sizeof(float);

    // Allocation and Initialization
    float *h_a = (float*)malloc(size);
    float *h_b = (float*)malloc(size);
    float *h_cpu_c = (float*)malloc(size); // Result from CPU
    float *h_gpu_c = (float*)malloc(size); // Result from GPU

    // Fill arrays
    for(int i=0; i<N; i++) {
        h_a[i] = 1.0f; 
        h_b[i] = 2.0f;
    }

    // GPU Memory Setup
    float *d_a, *d_b, *d_c;
    cudaMalloc(&d_a, size);
    cudaMalloc(&d_b, size);
    cudaMalloc(&d_c, size);

    cudaMemcpy(d_a, h_a, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_b, h_b, size, cudaMemcpyHostToDevice);

    // ----------------------------------------------------
    // MEASURE HOST (CPU) TIME
    // ----------------------------------------------------
    clock_t cpu_start = clock();
    
    host_function(h_a, h_b, h_cpu_c, N);
    
    clock_t cpu_end = clock();
    double cpu_ms = 1000.0 * (double)(cpu_end - cpu_start) / CLOCKS_PER_SEC;

    printf("Host (CPU) Time:   %f ms\n", cpu_ms);

    // ----------------------------------------------------
    // MEASURE DEVICE (GPU) TIME
    // ----------------------------------------------------
    // Set up CUDA Events
    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

    // Record Start
    cudaEventRecord(start);

    // Launch Kernel
    device_kernel<<<blocksPerGrid, threadsPerBlock>>>(d_a, d_b, d_c, N);

    // Record Stop
    cudaEventRecord(stop);
    cudaEventSynchronize(stop); // Wait for GPU to finish

    float gpu_ms = 0;
    cudaEventElapsedTime(&gpu_ms, start, stop);

    printf("Device (GPU) Time: %f ms\n", gpu_ms);
    printf("Speedup:           %.2fx\n", cpu_ms / gpu_ms);

    // ----------------------------------------------------
    // CLEANUP
    // ----------------------------------------------------
    cudaMemcpy(h_gpu_c, d_c, size, cudaMemcpyDeviceToHost);
    
    // Optional: Verify results match
    // verify_results(h_cpu_c, h_gpu_c, N);

    cudaFree(d_a); cudaFree(d_b); cudaFree(d_c);
    free(h_a); free(h_b); free(h_cpu_c); free(h_gpu_c);
    cudaEventDestroy(start);
    cudaEventDestroy(stop);

    return 0;
}