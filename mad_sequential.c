#include<stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char **argv){
    clock_t start;
    double* tinyNumbers;
    int size = 0;
    int index = 0;

	FILE *file_ptr;
	file_ptr = fopen(argv[1], "r");

	if (file_ptr == NULL)
        printf ("Cannot open %s!\n",argv[1]);
    else
    {
        double number;
        while(!feof(file_ptr))
    	{
    	  fscanf(file_ptr, "%lf", &number);
    	  size++;
		}
		size--;
		fclose(file_ptr);
		file_ptr = fopen(argv[1], "r");
		tinyNumbers = (double*)malloc(size*sizeof(double));
    	while(!feof(file_ptr))
    	{
			fscanf(file_ptr, "%lf", &tinyNumbers[index]);
			index++;
		}
		printf("Program is reading %s\n",argv[1]);
	}

	double sum = 0.0;
	for(int i=0; i<size; i++){ //calculate total sum
        sum += tinyNumbers[i];
	}

	double mean =(double)(sum /(double)size) ; //calculate mean

	double deviationSum = 0.0;
	for(int i=0; i<size; i++){ //calculate sum of deviations
        deviationSum += fabs(mean - tinyNumbers[i]);
	}
	double mad = deviationSum / (double)size; //calculate mad

	printf("Mad is %.2lf\n",mad);

	double maxNum, minNum = tinyNumbers[0];
	for(int i =0; i< size; i++){ //find max and min values of the array
        if(tinyNumbers[i] <= minNum){
            minNum = tinyNumbers[i];
        }
        else if(tinyNumbers[i] >= maxNum){
            maxNum = tinyNumbers[i];
        }
	}

	double range = maxNum - minNum; //calculate the range of array
	printf("Range is %.2lf\n",range);

        clock_t finish = clock();
	double time = fabsf((double)finish-start)/CLOCKS_PER_SEC;
        printf("Execution time for Range and MAD algorithm is %f seconds.\n",time);

    return 0;

}
