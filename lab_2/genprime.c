#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <omp.h>

void find_primes(int n, int t);

int main(int argc, char * argv[]) {
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

    double t_start = 0.0, t_taken;

#   pragma omp parallel num_threads(t) default(none) shared(prime_num_arr, n, sqrt_n, t_start) private(outer_loop_index, inner_loop_index)
{
#   pragma omp for
    for (outer_loop_index = 4; outer_loop_index < n; outer_loop_index += 1) {
        prime_num_arr[outer_loop_index] = 1;
    }

    t_start = omp_get_wtime();

    for (outer_loop_index = 2; outer_loop_index < sqrt_n; outer_loop_index += 1) {

        if (prime_num_arr[outer_loop_index]) {

#           pragma omp for
            for (inner_loop_index = outer_loop_index * 2; inner_loop_index < n; inner_loop_index += outer_loop_index) {
                prime_num_arr[inner_loop_index] = 0;
            }
        }
    }
}
    t_taken = omp_get_wtime() - t_start;

    printf("Time taken for the main part: %f\n", t_taken);

    int length = (int) log10(n) + 1;

    char filename[length + 5];
    snprintf(filename, sizeof(char) * length + 5, "%d.txt", n);

    FILE * output = fopen(filename, "w");

    if (!output) {
        printf("Something went wrong when opening the output file.\n");
        exit(1);
    }

    int prime_num_rank = 1;
    int prev_prime = 2;

    for (outer_loop_index = 0; outer_loop_index < n; outer_loop_index += 1) {

        if (prime_num_arr[outer_loop_index]) {
            fprintf(output, "%d, %d, %d\n", prime_num_rank, outer_loop_index, outer_loop_index - prev_prime);
            prev_prime = outer_loop_index;
            prime_num_rank += 1;
        }
    }

    fclose(output);
}
