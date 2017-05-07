#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <cuda.h>

void getmaxcu(unsigned int * number, unsigned int * maximum, int size);

int main(int argc, char * argv[]) {
    unsigned int size;
    unsigned int loop_index;
    unsigned int * number_arr;

    if (argc != 2) {
        printf("Not enough command line arguments. Try again.\n");
        exit(1);
    }

    size = atoi(argv[1]);

    number_arr = (unsigned int *) malloc(sizeof(unsigned int) * size);

    if (!number_arr) {
        printf("Something went wrong when allocating space for number_arr. Try again.\n");
        exit(1);
    }

    srand(time(NULL));

    for (loop_index = 0; loop_index < size; loop_index += 1) {
        number_arr[loop_index] = rand() % size;
    }

    cudaDeviceProp device_properties;
    cudaGetDeviceProperties(&device_properties, 0);

    int threads_per_block = device_properties.maxThreadsPerBlock;
    int num_threads;

    if (threads_per_block > size) {
        num_threads = size;
    }
    else {
        num_threads = threads_per_block;
    }

    int mod_size = num_threads;

    if (size % threads_per_block != 0) {
        mod_size = (size / threads_per_block + 1) * threads_per_block;
    }
    else {
        mod_size = size;
    }

    unsigned int * mod_number_arr = (unsigned int *) malloc(sizeof(unsigned int) * mod_size);

    for (loop_index = 0; loop_index < mod_size; loop_index += 1) {
        if (loop_index < size) {
            mod_number_arr[loop_index] = number_arr[loop_index];
        }
        else {
            mod_number_arr[loop_index] = 0;
        }
    }
}

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
