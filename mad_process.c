#include<sys/types.h>
#include<sys/wait.h>
#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>


int main(int argc, char **argv) {

    //file_desc[0] for reading
    //file_desc[1] for writing

     clock_t start = clock();


	int file_desc[2];
	int file_desc2[2];
	int file_desc3[2];
	int file_desc4[2];

    int size = 0;
    double* tinyNumbers;
	if (pipe(file_desc) == -1 || pipe(file_desc2) == -1 || pipe(file_desc3) == -1 || pipe(file_desc4) == -1){
        printf("ERROR OCCURED!!!\n");
        return 1;
	}

	int index = 0;

	FILE *file_ptr;
	file_ptr = fopen(argv[1], "r");
	//file_ptr = fopen("large.txt", "r");

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
		//file_ptr = fopen("large.txt", "r");
		tinyNumbers = (double*)malloc(size*sizeof(double));
    	while(!feof(file_ptr))
    	{
			fscanf(file_ptr, "%lf", &tinyNumbers[index]);
			index++;
		}
		printf("Program is reading %s\n",argv[1]);
		//printf("Program is reading large.txt\n");
	}

    double mad = 0.0;
    double minValue, maxValue;
    double firstDevSumBuffer;
    double firstSumBuffer;
    double meanBuffer;
    double minMaxBuffer[2];

    pid_t pid;
	pid = fork();

	if(pid < 0){
		fprintf(stderr, "Fork Failed\n");
		return 1;
	}
	else if(pid == 0){//child
		close(file_desc[0]); //close read
		close(file_desc2[1]); //close write
		close(file_desc3[0]);
		close(file_desc4[1]);

        double minMax[2];//minMax[0] for min value, minMax[1] for max value
        double firstHalf[size/2];
        double first_deviations[size/2];
        double firstSum = 0.0;
        double firstDevSum = 0.0;
		printf("The child process is created\n");

        for(int i = 0; i < size/2; i++){ //divide the array
	    	firstHalf[i] = tinyNumbers[i];
	    	firstSum += firstHalf[i];
		}

		maxValue= firstHalf[0];
		minValue= firstHalf[0];

		for(int j = 0; j < size/2; j++){ //find min and max values of first half
			if(firstHalf[j]>maxValue){
                maxValue = firstHalf[j];
	    	}
	    	else if(firstHalf[j] < minValue){
                minValue = firstHalf[j];
	    	}
		}

		minMax[0]=minValue;
		minMax[1]=maxValue;

		write(file_desc[1], &firstSum, sizeof(double));
	    close(file_desc[1]);

	    read(file_desc2[0], &meanBuffer, sizeof(double) );
		close(file_desc2[0]);

		read(file_desc4[0], &minMaxBuffer, sizeof(minMaxBuffer) );
		close(file_desc4[0]);

        if(minMaxBuffer[0] < minMax[0])
            minValue = minMaxBuffer[0];
        if(minMaxBuffer[1]>minMax[1])
            maxValue = minMaxBuffer[1];

		printf("Range is %.1lf\n", maxValue - minValue);

		for(int i = 0; i < size/2; i++){ //calculating the deviations
	    	first_deviations[i] = fabs(firstHalf[i] - meanBuffer);
	    	firstDevSum += first_deviations[i];
		}

	    write(file_desc3[1], &firstDevSum, sizeof(double));
	    close(file_desc3[1]);
            exit(0);
	}


	else { //parent
        close(file_desc[1]);
        close(file_desc2[0]);
        close(file_desc3[1]);
        close(file_desc4[0]);

        double minMax2[2]; //minMax[0] for min value, minMax[1] for max value
        double secondHalf[size/2];
        double second_deviations[size/2];
        double secondSum = 0.0;
        double mean = 0.0;
        double secondDevSum = 0.0;
		read(file_desc[0], &firstSumBuffer, sizeof(double) );
		close(file_desc[0]);

		int count=0;
	    for(int i = size/2; i < size; i++){
	    	secondHalf[count] = tinyNumbers[i];
	    	secondSum += secondHalf[count];
	    	count++;
		}

	    maxValue = secondHalf[0];
		minValue = secondHalf[0];
		for(int j = 0; j < size/2; j++){
			if(secondHalf[j] > maxValue){
                maxValue = secondHalf[j];
	    	}
	    	else if(secondHalf[j] < minValue){
                minValue = secondHalf[j];
	    	}
		}

		minMax2[0]=minValue;
		minMax2[1]=maxValue;

		write(file_desc4[1], &minMax2, sizeof(minMax2) );
		close(file_desc4[1]);

        mean = (double )((secondSum + firstSumBuffer)/(double)size); //calculating the mean of data set

        write(file_desc2[1], &mean, sizeof(double) );
		close(file_desc2[1]);

        read(file_desc3[0], &firstDevSumBuffer, sizeof(double) );
		close(file_desc3[0]);

		for(int i = 0; i < size/2; i++){
	    	second_deviations[i] = fabs(secondHalf[i] - mean);
	    	secondDevSum += second_deviations[i];
		}

		double totalDevSum = firstDevSumBuffer + secondDevSum;

		mad = totalDevSum / (double)size;

		printf("MAD is %.2lf\n",mad);


	}


        clock_t finish = clock();
	double time = ((double)finish-start)/CLOCKS_PER_SEC;
        printf("Execution time for Range and MAD algorithm is %f seconds.\n",time);



	return 0;
}
