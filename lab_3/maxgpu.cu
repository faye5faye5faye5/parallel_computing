#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <cuda.h>

__global__
void getmaxcu(unsigned int * number, unsigned int * maximum, int size) {
    extern __shared__ unsigned int data[];

    unsigned int t_id = threadIdx.x;
    unsigned int index = blockIdx.x * (blockDim.x) + threadIdx.x;

    data[t_id] = num[index];

    if (index >= size) {
        data[t_id] = 0;
    }
}
