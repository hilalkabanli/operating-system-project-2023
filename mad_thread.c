#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
#include <pthread.h>

#define MAX 450000
//the size of the array will be changed
//max will not use in the main part

int max_thread;
float arr[MAX] = {0};
float data[MAX] = {0};
float deviationSum[MAX] = {0};
float absSum[MAX]={0};
float mean=0;
int size = 0;
float range;
char file_name;
float maxNum;
float minNum;

int counter = 0;
int counter2 = 0;
pthread_mutex_t lock;

float Mean(float arr[], int n)
{
    float sum = 0;
    int i;
    for (i = 0; i < n; i++)
        sum = sum + arr[i];
    return sum / n;
}

void* MeanAbsoluteDeviation(void* arg)
{
	pthread_mutex_lock(&lock);
	// Calculate the sum of absolute
    int thread_part =counter++;
    pthread_mutex_unlock(&lock);
   // deviation about mean.
	int i;
    for (i = thread_part * (size / max_thread); i < (thread_part + 1) * (size / max_thread); i++){
        absSum[thread_part] = absSum[thread_part] + fabs(mean - data[i]);
	}
 	// Return mean absolute deviation about mean.

}

void* Range(void* arg)
{
	pthread_mutex_lock(&lock);
	int thread_part2 = counter2++;
	pthread_mutex_unlock(&lock);

	int i;
	for(i = thread_part2 * (size / max_thread); i < (thread_part2 + 1) * (size / max_thread); i++){ //find max and min values of the array
        if(data[i] < minNum){
            minNum = data[i];

        }
        else if(data[i] > maxNum){
            maxNum = data[i];
        }

	}

        range=maxNum-minNum;

}

int main(int argc, char *argv[])
{

	max_thread = atoi(argv[2]);
	clock_t begin = clock();
	FILE *file;
    int line=0;
    file = fopen(argv[1], "r");
    if(!file){

        printf ("Cannot open %s!\n",argv[1]);
    }
    else{
        while(!feof(file)){
            fscanf(file, "%f", &data[line]);
            line++;
		}
        line--;
	}

    fclose(file);
	printf("Program is reading %s\n",argv[1]);


	size = line;

    mean = Mean(data, size);

    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init failed\n");
        return 1;
    }

    pthread_t threads[max_thread];

    int i;
    for (i = 0; i < max_thread; i++)
        pthread_create(&threads[i], NULL, MeanAbsoluteDeviation, (void*)NULL);

    printf("%d threads are created\n", max_thread);

    for (i = 0; i < max_thread; i++)
        pthread_join(threads[i], NULL);
    // adding sum of all 4 parts
       float total_mean=0.0;

    for (i = 0; i < max_thread; i++){
        total_mean += absSum[i];

 	}
    printf("MAD is %.2f\n", total_mean/size);

    pthread_mutex_destroy(&lock);


	minNum=data[0];
	maxNum=data[0];
    pthread_t thread2[max_thread];

    for (i = 0; i < max_thread; i++)
        pthread_create(&thread2[i], NULL, Range, (void*)NULL);
    for (i = 0; i < max_thread; i++)
        pthread_join(thread2[i], NULL);
    printf("Range is %.1f\n",range);
    clock_t end = clock();

    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("Execution time for Range and MAD algorithm is %f seconds\n", time_spent);
    return 0;

}
