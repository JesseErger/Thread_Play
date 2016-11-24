#include <time.h>
#include <stdio.h>
#include <stdlib.h>
double * data_set;
void usage(char *prog) 
{
    fprintf(stderr, "Usage: %s datafile queryfile outputfile number_threads\n", prog);
    exit(1);
}
//return index of closest point
int shortest_distance(double * q_point){

}


int main(int argc, char *argv[]){

if(argc != 5){
	usage(argv[0]);
}
char *dataFile = argv[1];
char *queryFile = argv[2];
char *outFile = argv[3];
int num_threads = atoi(argv[4]);
int num_points;
int dimension;
int i,o =0;
FILE * fp;
time_t start,end;
time(&start);
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
//printf("%f",*(data_set+(num_points*dimension)-1)); <-- last point
int num_querys;
//
int query_dim;
int c;	


	//code here
	time(&end);
	double dif = difftime(end,start);
	printf ("Elapsed time is %.01f seconds. \n", dif);
	free(data_set);
	return 0;

}