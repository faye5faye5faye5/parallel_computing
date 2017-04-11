#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void find_primes(int n, int t);

int main(int argc char * argv[]) {
    int N = strtol(argv[1], NULL, 10);
    int t = strtol(argv[2], NULL, 10);

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
    int prime_num_arr[n];
}
