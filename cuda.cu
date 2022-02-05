#include <cstdlib>
#include <cassert>
#include <iostream>

using namespace std;

__global__ void matrixMul(int *a, int *b, int N, int M)
{
    // Calculate the global row and column for each thread
    int row = blockIdx.y * blockDim.y + threadIdx.y;
    int col = blockIdx.x * blockDim.x + threadIdx.x;

    // Boundary check for our matrix
    if(row < N && col < M)
    {
        // Accumulate a partial result
        int tmp = 0;
        for(int i = 0; i < N; i++)
        {
            //tmp += a[row * N + i] * b[i * N + col];
            tmp += a[i * M + col];
        }

        // Write back the result
        b[col] = tmp;
    }

    int max = 0;
    int index = 0;
    int max_index = 0;

    for (int k ; k<M;k++)
    {
        if (b[i]>max)
        {
            max = b[i];
            index = i;
        }
    }

    max_index = index;

}
// Initializes a square matrix with random numbers between 0-100
void init_matrix(int *m, int N, int M)
{
    for(int i = 0; i < N * M; i++)
    {
        m[i] = rand() % 100;

    }
}

// Verify the result on the CPU
void verify_result(int *a, int *b, int *c, int N){
    int tmp;
    // For every row...
    for(int i = 0; i < N; i++){
        // For every col...
        for(int j = 0; j < N; j++){
            // For every element in the row-col pair
            tmp = 0;
            for(int k = 0; k < N; k++){
                tmp += a[i * N + k] * b[k * N + j];
            }
            
            // Check each result
            assert(tmp == c[i * N + j]);
        }
    }
}

int main(){
    // Set our square matrix dimension (2^10 x 2^10 default) 
    int N = 1000;
    int M = 100;
    size_t bytes = N * M * sizeof(int);

    // Allocate memory for our matrices
    int *a, *b;
    cudaMallocManaged(&a, bytes);
    cudaMallocManaged(&b, bytes);
    

    // Initialize our matrices
    init_matrix(a, N, M);
    init_matrix(b, 0, M);

    // Set our CTA and Grid dimensions
    int threads = 16;
    int blocks = (N + threads - 1) / threads;

    // Setup our kernel launch parameters
    dim3 THREADS(threads, threads);
    dim3 BLOCKS(blocks, blocks);

    // Launch our kernel
    matrixMul<<<BLOCKS, THREADS>>>(a, b, N, M);
    cudaDeviceSynchronize();

    // Verify the result
    verify_result(a, b, N, M);

    cout << "PROGRAM COMPLETED SUCCESSFULLY!" << endl;
    
    // Free allocated memory
    cudaFree(a);
    cudaFree(b);
 
    return 0;
}
