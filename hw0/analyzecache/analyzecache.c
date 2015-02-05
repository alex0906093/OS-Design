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
	/*
	 *Create 2 loops, outside one should control what power of 2 inside one 
	 *is incrementing by, fill an array with results in order to find where 
	 *cache is no longer being accessed
	 *	 
	 */
	static int power;
	float times[11];
	int timeinsert = 0;
	for(power = 2; power < 4096; power = power * 2){	 
		long long start = clock_time_1();
		for(i = 0; i < big; i = i + power){
			c = BigArray[i];
			char a = c + c;
		}
		long long end = clock_time_1();
		float totaltime1 = ((float)(end - start));
		float avgAcc = totaltime1 / (big / power);
		times[timeinsert] = avgAcc;
		timeinsert++;
		//printf("Average Access Time for loop was %f \n", avgAcc);

	}
	float lasttime = times[0];
	float currtime;
	float misspen;
	int j;
	float diff;
	int powcount = 1;
	for(j = 1; j < 11; j++){
		currtime = times[j];
		diff = 0.0;
		diff = currtime - lasttime;
		
		if(diff > 1.5){
			misspen = diff;
			break;
		}
		powcount++;
		lasttime = currtime;
	}
	
	int linesize = pow(2, powcount);
	printf("Cache Block/Line Size: %d B\n", linesize);
	misspen = misspen / CLOCKS_PER_SEC;
	misspen = misspen * 1000000;
	printf("Cache Miss Penalty: %f us\n", misspen);

	return 0;
}

long long clock_time_1(){
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	return (long long)(tp.tv_nsec + (long long)tp.tv_sec * 100000000011);
}
