#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void find_primes(int n, int t);

int main(int argc char * argv[]) {
    int N = atoi(argv[1]);
    int t = atoi(argv[2]);

    if (!N) {
        printf("Please enter a number greater than 0 for N.");
        exit(1);
    }
    if (!t) {
        printf("Please enter a number greater than 0 for t.");
        exit(1);
    }
    if (N <= 1 || t <= 0) {
        printf("Please enter valid values for N or t.");
        exit(1);
    }

    find_primes(N, t);

    return 0;
}

void find_primes(int n, int t) {
    int sqrt_n = (int) sqrt(n);
    int check_next = 2;
    int loop_index;

    int prime_num_arr[n];
}
