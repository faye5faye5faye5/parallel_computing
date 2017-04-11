#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

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
}
