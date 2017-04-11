#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <mpi.h>

/*** Skeleton for Lab 1 ***/

/***** Globals ******/
float *a; /* The coefficients */
float *x;  /* The unknowns */
float *b;  /* The constants */
float *aii;
float err; /* The absolute relative error */
int num = 0;  /* number of unknowns */


/****** Function declarations */
void check_matrix(); /* Check whether the matrix will converge */
void get_input();  /* Read input from file */
int check_error_spread();

/********************************/

int check_error_spread(float * updated_x, int num) {
  int index;
  float current_error;

  for (index = 0; index < num; index += 1) {

    current_error = fabsf((updated_x[index] - x[index]) / updated_x[index]);
    if (current_error > err) {
      return 1;
    }
  }

  return 0;
}

/* Function definitions: functions are ordered alphabetically ****/
/*****************************************************************/

/*
   Conditions for convergence (diagonal dominance):
   1. diagonal element >= sum of all other elements of the row
   2. At least one diagonal element > sum of all other elements of the row
 */
void check_matrix()
{
  int bigger = 0;
int i, j;
float sum = 0;
float aii = 0;


  for(i = 0; i < num; i++){
    sum = 0;
    aii = fabs(a[i*(num + 1)]);


    for(j = 0; j < num; j++){
        if( j != i) sum += fabs(a[i*num + j]);
    }

    if( aii < sum){
        printf("The matrix will not converge\n");
        exit(1);
    }

    if(aii > sum) bigger++;
  }

  if( !bigger ){
    printf("The matrix will not converge\n");
    exit(1);
  }
}


/******************************************************/
/* Read input from file */
/* After this function returns:
 * a[][] will be filled with coefficients and you can access them using a[i][j] for element (i,j)
 * x[] will contain the initial values of x
 * b[] will contain the constants (i.e. the right-hand-side of the equations
 * num will have number of variables
 * err will have the absolute error that you need to reach
 */
 void get_input(char filename[])
  {
    FILE * fp;
     int i ,j;

     fp = fopen(filename, "r");

     if(!fp){
         printf("Cannot open file %s\n", filename);
         exit(1);
     }

     fscanf(fp,"%d ",&num);
     fscanf(fp,"%f ",&err);

     a = (float*)malloc(num * num * sizeof(float));

     if(!a){
         printf("Cannot allocate a!\n");
         exit(1);
     }

     x = (float *) malloc(num * sizeof(float));

     if(!x){
         printf("Cannot allocate x!\n");
         exit(1);
     }

     b = (float *) malloc(num * sizeof(float));

     if( !b){
         printf("Cannot allocate b!\n");
         exit(1);
     }

     aii = (float *) malloc(num * sizeof(float));

     for(i = 0; i < num; i++){
         fscanf(fp,"%f ", &x[i]);
     }

     for(i = 0; i < num ; i++){
         for(j = 0; j < num; j++){
             fscanf(fp,"%f ",&a[i*num + j]);

             if(i == j){
                 aii[i] = a[i*num + j];
             }
         }
         fscanf(fp,"%f ",&b[i]);
     }
     fclose(fp);
  }



/************************************************************/


int main(int argc, char *argv[])
{

 int i;
 int j;
 int index;
 int nit = 0; /* number of iterations */

 if( argc != 2)
 {
   printf("Usage: gsref filename\n");
   exit(1);
 }

 /* Read the input file and fill the global data structure above */
 get_input(argv[1]);

 /* Check for convergence condition */
 /* This function will exit the program if the coffeicient will never converge to
  * the needed absolute error.
  * This is not expected to happen for this programming assignment.
  */
 check_matrix();

 int comm_size;
 int my_rank;

 MPI_Init(&argc, &argv);
 MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
 MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

 int my_num_min = num / comm_size;
 int num_left = num % comm_size;
 int sending_buff[comm_size];
 int receiving_buff[comm_size];
 int displacement_buff[comm_size];
 int displacement = 0;

 for (i = 0; i < comm_size; i += 1) {

   if (i < num_left) {
     sending_buff[i] = my_num_min + 1;
   }
   else {
     sending_buff[i] = my_num_min;
   }

   receiving_buff[i] = sending_buff[i];

   displacement_buff[i] = displacement;

   displacement += sending_buff[i];
 }

 int my_num = (int) ceil((double) num / comm_size);
 int elements_in_a = my_num * num;

 float * my_x = (float *) malloc(sizeof(float) * my_num);
 float * my_a = (float *) malloc(sizeof(float) * my_num * num);
 float * my_b = (float *) malloc(sizeof(float) * my_num);
 float * my_diagonal = (float *) malloc(sizeof(float) * my_num);

 float * current_x = (float *) malloc(sizeof(float) * num);

 MPI_Scatter(a, elements_in_a, MPI_FLOAT, my_a, elements_in_a, MPI_FLOAT, 0, MPI_COMM_WORLD);
 MPI_Scatter(b, my_num, MPI_FLOAT, my_b, my_num, MPI_FLOAT, 0, MPI_COMM_WORLD);
 MPI_Scatter(aii, my_num, MPI_FLOAT, my_diagonal, my_num, MPI_FLOAT, 0, MPI_COMM_WORLD);
 MPI_Scatterv(x, sending_buff, displacement_buff, MPI_FLOAT, my_x, receiving_buff[my_rank], MPI_FLOAT, 0, MPI_COMM_WORLD);

 for (index = 0; index < num; index += 1) {
   current_x[index] = x[index];
 }

 do {

   for (index = 0; index < num; index += 1){
     x[index] = current_x[index];
   }

   for (index = 0; index < sending_buff[my_rank]; index += 1) {

     int tmp_index = index;

     for (j = 0; j < my_rank; j += 1) {
       tmp_index += sending_buff[j];
     }

     my_x[index] = my_b[index];

     for (j = 0; j < tmp_index; j += 1) {
       my_x[index] -= (my_a[index * num + j] * x[j]);
     }

     for (j = tmp_index + 1; j < num; j += 1) {
       my_x[index] -= (my_a[index * num + j] * x[j]);
     }

     my_x[index] /= my_diagonal[index];
   }

   MPI_Allgatherv(my_x, sending_buff[my_rank], MPI_FLOAT, current_x, receiving_buff, displacement_buff, MPI_FLOAT, MPI_COMM_WORLD);

   nit += 1;

 } while(check_error_spread(current_x, num));


if (my_rank == 0) {
   /* Writing to the stdout */
   /* Keep that same format */
   for( i = 0; i < num; i++) {
     printf("%f\n", x[i]);
   }

   printf("total number of iterations: %d\n", nit);

   free(a);
   free(b);
   free(aii);
   free(x);
 }

 free(my_a);
 free(my_b);
 free(my_diagonal);
 free(my_x);
 free(current_x);

 MPI_Barrier(MPI_COMM_WORLD);
 MPI_Finalize();

 return 0;

}
