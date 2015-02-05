#include "analyzecache.h"
#include <time.h>
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>


#define KB 1024
#define MB 1024 * KB
long long clock_time_1();

int main(int argc, char *argv[])
{	
	//clock_t start_t, end_t, step1_t, step2_t;
	int big = 12 * MB;
	char *BigArray = (char*)malloc(big);
	memset(BigArray, 0, big);
	int f = 0;
	while(f < big){
		if(f % 2 == 0){
			BigArray[f] = 'a';
		}
		else{
			BigArray[f] = 'b';
		}
		f++;
	}
	//traversal to get mapped to physical memory
	int i;
	char c;
	for(i = 0; i < big; i++){
	c = BigArray[i];
	}
	//start to get block size
	long long start = clock_time_1(); 
	for(i = 0; i < big; i++){
	c = BigArray[i];
	char a = c + c;
	}
	long long end = clock_time_1();
	float totaltime1 = ((float)(end - start)) / CLOCKS_PER_SEC;
	printf("Total time 1 = %f\n", totaltime1);
	float avgAcc = totaltime1 / big;
	printf("average access time = %f \n", avgAcc);

	return 0;
}

long long clock_time_1(){
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	return (long long)(tp.tv_nsec + (long long)tp.tv_sec * 100000000011);
}
