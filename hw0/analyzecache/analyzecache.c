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
	int big = 24 * MB;
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

		if(diff > 3.4){
			misspen = diff;
			break;
		}
		powcount++;
		lasttime = currtime;
	}

	int linesize = pow(2, powcount);
	printf("Cache Block/Line Size: %d B\n", linesize);
	/*
	 *
	 *Get Size of cache by reading through increasingly bigger array 
	 *until it doesnt fit in memory and generates a miss.
	 *
	 */
	int l;
	char t;
	char *newBig;
	int y;
	float accTime1;
	int powCheck = 0;
	//flag to keep loop going 1 true 0 false
	int go = 1;
	//newBig = (char*)malloc(128 * MB);
	//memset(newBig, 0, 128 * MB);
	//fill array
	/*for(i = 0; i < 128 * MB; i++){
		if(i % 2 == 0)
			newBig[i] = 'a';
		else
			newBig[i] = 'b';
	}*/
	int sizes[18];
	int jl = 0;
	float times5[18];
	//populate sizes array
	for(i = KB; i <= 128 * MB; i = i * 2){
		sizes[jl] = i;
		//printf("%d \n", jl);
		jl++;
	}
	int o;
	int r;
	char test1, test2;
	int timecount = 0;
	for(i = 0; i < 18; i++){
		o = sizes[i];
		//read cache into memory
		newBig = (char*)malloc(o);
		//printf("array size %d \n", o);
		memset(newBig, 0, o);
		for(r = 0; r < o; r++){
			if(r % 2 == 0)
				newBig[r] = 'a';
			else
				newBig[r] = 'b';
		}
		
		for(r = o; r > o; r = r - 32){
			test1 = newBig[r];
			test2 = newBig[r - 1];
			test1 = test1 + test2;
			
		}
		test1 = test1 - 'b';	
		
		//clock speed of array traversal
		long long start2 = clock_time_1();

		for(r = 0; r < o; r = r + 32){
			test2 = newBig[r];
			test2 = test2 +newBig[r - 1];
		}
		test2 = 'b';
		long long end2 = clock_time_1();
		float totTime2 = ((float)(end2 - start2));
		int numAcc = o / linesize;
		float avgTime4 = totTime2 / numAcc;
		times5[timecount] = avgTime4;
		timecount++;
		free(newBig);
		//printf("Average Access Time %f for %d KB\n", avgTime4, o / KB);	
	}
	//printf("%c %c \n", test1, test2);
	misspen = misspen / CLOCKS_PER_SEC;
	misspen = misspen * 10000;
	float diff1;
	float lasttime1 = times5[3];
	float currtime1;
	int powcheck1 = 0;
	for(j = 4; j < 18; j++){
		currtime1 = times5[j];
		diff1 = currtime1 - lasttime1;
		if(diff1 > 0.5){
			powcheck1 = j - 1;
			break;
		}
	}
	int cachesize = pow(2, powcheck1);
	cachesize = cachesize/8;
	printf("Cache Size: %d KB\n", cachesize);
	printf("Cache Miss Penalty: %f us\n", misspen);
	free(BigArray);
	//free(newBig);
	return 0;
}

long long clock_time_1(){
	struct timespec tp;
	clock_gettime(CLOCK_REALTIME, &tp);
	return (long long)(tp.tv_nsec + (long long)tp.tv_sec * 100000000011);
}
