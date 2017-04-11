#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

int main(int argc char * argv[]) {
    int N = strtol(argv[1], NULL, 10);
    int t = strtol(argv[2], NULL, 10);

    if (!N) {
        printf("Please enter a number greater than 0.");
        exit(1);
    }
}
