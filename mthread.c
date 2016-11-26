#include <time.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
double * data_set;
double * query_points;
int * results;

int num_points;
int dimension;
int num_querys;
int query_dim;
int num_threads=1;
int jobs_per_thread;
int jobs_last_thread =0;
int count;
int test = 0;
pthread_mutex_t ct_mutex,temp;
void usage(char *prog) 
{
    fprintf(stderr, "Usage: %s datafile queryfile outputfile number_threads\n", prog);
    exit(1);
}
//return index of closest point
void * find_cps(void * res_ptr){	
int * ct = (int *)res_ptr;
int myct;
pthread_mutex_lock(&ct_mutex);
myct = *ct;
*ct = *ct+1;
pthread_mutex_unlock(&ct_mutex);
if(myct==num_threads-1){

	 if(jobs_per_thread*num_threads<num_querys){
	 //	printf("\nhere\n\n\n ");
  jobs_last_thread =  num_querys - ((jobs_per_thread)*(num_threads-1));
  }
  else{
  	jobs_last_thread = jobs_per_thread;
  }
  printf(" Last threads jobs %i",jobs_last_thread);
}


	for(int x = 0; (myct != num_threads-1 && x<jobs_per_thread) || (myct == num_threads-1 && x<jobs_last_thread);x++){
		//read query into mem
		int indx = 0;
		double min = -1;
		for(int y=0;y<num_points;y++){
		double sum = 0;
			for(int z=0;z<query_dim;z++){
			/* *(this_query + y)*/

			 sum += pow( (*(query_points + jobs_per_thread*myct*query_dim  + x*query_dim + z) - *(data_set + y*dimension + z) ),2 );
	
			sum = sqrt(sum);
			//printf("Point: %i Distance: %f current min: %f\n",x,sum,min);
			if( min < 0 ){
				min = sum;
				indx = y;//maybe +1

			}
			else if(sum<min){
				min = sum;
				indx = y;//maybe +1
			}
		}
	}
		*(results + myct*jobs_per_thread + x) = indx+1;
		//can see the threads working 
		printf("Res %i Thread %i\n ",*(results + myct*jobs_per_thread + x),myct);
	
 }





return NULL;

}


int main(int argc, char *argv[]){
count =0;
if(argc != 5){
	usage(argv[0]);
}
char *dataFile = argv[1];
char *queryFile = argv[2];
char *outFile = argv[3];
num_threads = atoi(argv[4]);

int i,o =0;
FILE * fp;
time_t start,end;

fp = fopen(dataFile, "r");
if (fp == NULL) {
	fprintf(stderr,"Error: Cannot open data file %s\n",dataFile);
	exit(1);
}
//get dimension/number of points to allocate appropriate space
fscanf(fp,"%d",&num_points);
fscanf(fp,"%d",&dimension);
data_set = (double *)malloc(sizeof(double)*num_points*dimension+2);

//read dataset into global memory.
for(int x = 0;x<num_points*dimension;x++){
	fscanf(fp,"%lf",(data_set+x));
}


FILE * fpq;
fpq = fopen(queryFile, "r");
if (fpq == NULL) {
	fprintf(stderr,"Error: Cannot open query file %s\n",queryFile);
	exit(1);
}
fscanf(fpq,"%d",&num_querys);
fscanf(fpq,"%d",&query_dim);
if(num_threads>num_querys)
	num_threads = num_querys;

 jobs_per_thread = num_querys/num_threads;




//threads write their results here
results = (int *)malloc(num_querys*sizeof(int)+2);
//allocate space to bring all querys into memory (not sure if this is a good idea or not)
query_points =  (double *)malloc(sizeof(double)*num_querys*query_dim+2);
//Need to iterate through each query point - storing in memory
for(int x = 0;x<query_dim*num_querys;x++){
	fscanf(fpq,"%lf",(query_points+x));
}
time(&start);
pthread_t threads[num_threads];
void * rc[num_threads];
for(int x = 0; x<num_threads;x++){
	if(pthread_create(&threads[x], NULL,find_cps,&count)){
		exit(1);
	}

}

for(int y=0;y<num_threads;y++){
	if(pthread_join(threads[y],&rc[y])){
		exit(1);
	}
}
time(&end);
for(int x =0;x<num_querys;x++){
	printf("(%i), Query# %i\n", *(results+x),x );
}
	
	double dif = difftime(end,start);
	printf ("Elapsed time is %.01f seconds. \n", dif);


	free(data_set);
	free(query_points);
	free(results);
	return 0;
}