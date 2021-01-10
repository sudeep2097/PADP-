
/* Write an OpenMP program that computes a simple matrix-matrix multiplication using dynamic memory allocation.
a) Illustrate the correctness of the program.
b) Justify the inference when outer “for” loop is parallelized with and without using the explicit data scope variables.
*/


#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
int main(){

//int r = 1000, c = 1000, i, j, count=0, sum =0, k;

int r, c, i, j, count=0, sum =0, k,thread;

printf("Enter no. of Rows : ");
scanf("%d",&r);
printf("Enter no. of Columns: ");
scanf("%d",&c);

printf("Enter no. of Threads: ");
scanf("%d",&thread);

//dynamically allocate arrays
int **arr1 = (int **)malloc(r * sizeof(int *));
for (i=0; i<r; i++)
arr1[i] = (int *)malloc(c * sizeof(int));
int **arr2 = (int **)malloc(r * sizeof(int *));
for (i=0; i<r; i++)
arr2[i] = (int *)malloc(c * sizeof(int));
int **arr3 = (int **)malloc(r * sizeof(int *));
for (i=0; i<r; i++)
arr3[i] = (int *)malloc(c * sizeof(int));

for(i = 0;i < r; i++)
	for(j = 0;j < c; j++)
		arr1[i][j] = count++;

for(i = 0;i < r; i++)
	for(j = 0;j < c; j++)
		arr2[i][j] = count++;
double x = omp_get_wtime(); //gets current time wrt common reference time in seconds
omp_set_num_threads(thread);
#pragma omp parallel for private(j,k) //if j and k not private then output is incorrect
for(i = 0;i < r; i++)
	for(j = 0;j < c; j++)
		for(k = 0;k < r; k++)
			arr3[i][j] += arr1[i][k] * arr2[k][j];

double y = omp_get_wtime();
printf("\nTime taken : %lf sec\n", y-x);
return 0;
}

  

