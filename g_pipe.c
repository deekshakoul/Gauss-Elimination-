#include<stdio.h>
#include<stdlib.h>
#include<omp.h>


struct record_s{
	int val;
	int prod;
	struct record_s* next_p;
};

struct buf_list{
	struct record_s* head_p;
	struct record_s* tail_p;
};

struct buf_list *buff;
int *producers_done;

void Enqueue(int rank, struct record_s* rec_p){
	if(buff[rank].tail_p == NULL){
		buff[rank].head_p = rec_p;
		buff[rank].tail_p = rec_p;
	}
	else{
		buff[rank].tail_p->next_p = rec_p;
		buff[rank].tail_p = rec_p;
	}
}

struct record_s *Create_record(int rank, int data){
	struct record_s *new = (struct record_s*)malloc(sizeof(struct record_s));
	new -> val = data;
	new -> prod = rank;
	new -> next_p = NULL;
}

void Put(int rank, int data){
	struct record_s *rec_p;

	rec_p = Create_record(rank, data);

	#pragma omp critical(queue)
	{
		Enqueue(rank, rec_p);
	}
	#pragma omp critical(done)

	producers_done[rank]++;
}

struct record_s* Dequeue(int rank){
	struct record_s *rec_p;

	if(buff[rank].head_p == NULL)
		return NULL;
	else{
		if(buff[rank].head_p == buff[rank].tail_p){
			rec_p = buff[rank].head_p;
			buff[rank].head_p = buff[rank].tail_p = NULL;		
		}
		else{
			rec_p = buff[rank].head_p;				

			buff[rank].head_p = buff[rank].head_p -> next_p;
		}
		return rec_p;
	}
}

int Get(int rank){
	struct record_s *rec_p;
	int data;

	while(producers_done[rank] < 1 || buff[rank].head_p != NULL){
		#pragma omp critical(queue)
		{
			rec_p = Dequeue(rank);
		}	
		if(rec_p != NULL){
			data = rec_p -> val;
			free(rec_p);
			return data;
		}
	}
}


int main(int argc, char *argv[]){

	int n = atoi(argv[1]);	//Number of unknowns
	int p = atoi(argv[2]);	//Number of threads
	
	int i, j, k, row;
	double **a =  (double**)malloc(n * sizeof(double*));
	buff = (struct buf_list*)malloc((p + 1) * sizeof(struct buf_list));	
	producers_done = (int*)malloc((p + 1) * sizeof(int));
	double cons = 0.0;
	for(i = 0; i < p + 1; i++){
		buff[i].head_p = NULL;
		buff[i].tail_p = NULL;
		producers_done[i] = 0;
	}

	for(i = 0; i < n; i++){
		a[i] = (double*)malloc((n + 1) * sizeof(double));
		for(j = 0; j < n; j++){
			a[i][j] = i < j? 1: -1;
		} 
		a[i][n] = rand()%50;
	}


	omp_set_num_threads(p);

	double start = omp_get_wtime();
	
	#pragma omp parallel private(k, i, j, row,cons) shared(a)
	{
		int rank = omp_get_thread_num();
		int size = n / p; ///n should be multiple of p

		if(rank != 0){
			for(i = 0; i < rank * size; i++){
				row = Get(rank);
				Put(rank + 1, row);
				for(j = rank * size; j < (rank + 1) * size; j++){
					cons  = a[j][row]/a[row][row];
					for(k = row + 1; k < n + 1; k++){
						a[j][k] -= cons * a[row][k];
					}
				}
			}
		}
		for(i = rank * size; i <  (rank + 1) * size; i++){
			Put(rank + 1, i);
			for(j = i + 1; j < (rank + 1) * size; j++){
			
				cons = a[j][i]/a[i][i];
				for(k = i + 1; k < n + 1; k++){
					a[j][k] -= cons * a[i][k]; 					
				}
			
			}
		
		}
		
	
	}
	double time = omp_get_wtime();

	for(i = n-1; i >= 0; i--){
		x[i] =a[i][n];
		double cons = 0;
		for(j = i + 1; j < n; j++){
			cons += a[i][j] * x[j];
		}	
		x[i] -= cons;
		x[i] /= a[i][i];	
	}
	double end = omp_get_wtime();
	
	printf("%lf %lf",end - time, time - start);

}