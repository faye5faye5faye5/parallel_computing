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
    int outer_loop_index;
    int inner_loop_index;

    int prime_num_arr[n];

    prime_num_arr[0] = 0;
    prime_num_arr[1] = 0;
    prime_num_arr[2] = 1;
    prime_num_arr[3] = 1;

    double t_start = 0.0, t_end = 0.0, t_taken;

#   pragma omp parallel num_threads(t) default(none) shared(prime_num_arr, n) private(loop_index, inner_loop_index)
#   pragma omp for
    for (outer_loop_index = 4; outer_loop_index < n; outer_loop_index += 1) {
        prime_num_arr[outer_loop_index] = 0;
    }

    for (outer_loop_index = 2; outer_loop_index < sqrt_n; outer_loop_index += 1) {

        if (prime_num_arr[outer_loop_index]) {

#           pragma omp for
            for (inner_loop_index = outer_loop_index * 2; inner_loop_index < n; inner_loop_index += outer_loop_index) {
                prime_num_arr[inner_loop_index] = 0;
            }
        }
    }
}
