
// gcc p1 -fopenmp p1.c
// ./p1

#include<omp.h>
#include<stdio.h>
#include<stdlib.h>
# include <math.h>
#define SEED 35791246

void main()
{
	int n = 0, i, count = 0,t;
	double z, pi, x,y;


	printf("Enter the number of iterations used to estimate pi: ");
	scanf("%d", &n);
	printf("Enter the number of threads: ");
	scanf("%d", &t);
	srand(SEED);
	double t1 = omp_get_wtime();
	omp_set_num_threads(t);
	#pragma omp parallel for private(x,y,z) reduction( +: count)

	for ( i=0; i<n; i++)
	{
		x = (double)rand()/RAND_MAX;
		y = (double)rand()/RAND_MAX;
		z = x*x+y*y;
		if (z<=1) count++;
	}
	double t2 = omp_get_wtime();
	printf(" time taken : %lf sec\n", t2-t1);
	pi=(double)count/n*4;

	printf("# of trials= %d \n  # of threads= %d \n estimate of pi is %g\n",n,t,pi);
}

//
