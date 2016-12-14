#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]){
	
	//int p = atoi(argv[1]); 
	int N = atoi(argv[1]); //no. of threads
	int M = N+1; //no. of equations
	double **a = (double**)malloc(N* sizeof(double*));
	 //printf("what happen in arrray allocation\n");
	int i, j, k;
	
	for(i = 0; i < N; i++)
		a[i] = (double*)malloc(M * sizeof(double));
	//printf("what happen in arrray COLUMN allocation\n");
	////initialize the matrix with coefficients
	for(i = 0; i < N; i++){
		for(j = 0; j < N; j++){
			a[i][j] = (i < j)? -1 : 1;
		}
		//a[i][i] += 1;
		a[i][N] = rand();
	}
	
	double *x = (double*)calloc(N, sizeof(double));
	double cons=0.0;
	//printf("what happen does problem have x - allocation\n");
	double time = omp_get_wtime();
	
	for(i = 0; i < N - 1; i++){
		for(j = i + 1; j < N; j++){
			//printf("is second loop div method\n");
			 cons = a[j][i]/a[i][i];
			a[j][i] = 0; 
			for(k = i + 1; k < N + 1; k++){ 
				//printf("In Parallel with thread number %d\n",omp_get_thread_num);
				a[j][k] = a[j][k] - cons * a[i][k];
 			}
		}
	}
	//printf("OUT Parallel\n");
	for(i = N-1; i >= 0; i--){
		x[i] =a[i][N];
		double cons = 0;
		for(j = i + 1; j < N; j++){
			cons += a[i][j] * x[j];
		}	
		x[i] -= cons;
		x[i] /= a[i][i];	
	}
	
	double end = omp_get_wtime();
	
	printf("%lf", end - time);
}
