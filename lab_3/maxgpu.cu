#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <cuda.h>

void getmaxcu(unsigned int * number, unsigned int * maximum, int size);

__global__
void getmaxcu(unsigned int * number, unsigned int * maximum, int size) {
    extern __shared__ unsigned int data[];

    unsigned int t_id = threadIdx.x;
    unsigned int index = blockIdx.x * (blockDim.x) + threadIdx.x;

    data[t_id] = num[index];

    if (index >= size) {
        data[t_id] = 0;
    }

    __syncthreads();

    int loop_index;
    for (loop_index = blockDim.x / 2; loop_index > 0; loop_index >>= 1) {

        __syncthreads();

        if (t_id < loop_index) {
            if (data[t_id] < data[t_id + loop_index]) {
                data[t_id] = data[t_id + loop_index];
            }
        }
    }

    if (t_id == 0) {
        maximum[blockIdx.x] = data[0];
    }
}
