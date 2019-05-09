#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

void printTimeStampsToCsv(unsigned long* ms, unsigned long* us, int length, float delta);

void readCPUusage();

int main(int argc, char** argv){


    /* ----- Parsing Arguments ----- */
    if(argc < 2){
        printf("Not Enough Arguments\n arg1::int number of samples, arg2::float delta time\n");
        return 0;
    }
    int number_of_samples = atoi(argv[1]);
    float f_delta_time = atof(argv[2]);
    unsigned long delta_time = 1000*1000*f_delta_time; // tranform seconds to micro seconds for usleep
    printf("Samples: %d\n Delta Time: %f s OR %lu us\n",number_of_samples,f_delta_time, delta_time);
    /* ------------------------------*/


    struct timeval sample,start,end;

    unsigned long *s_arr, *us_arr;
    s_arr = (unsigned long*)malloc(number_of_samples*sizeof(unsigned long));
    us_arr = (unsigned long*)malloc(number_of_samples*sizeof(unsigned long));


    readCPUusage();
    gettimeofday(&start,NULL);
    // Main Loop
    delta_time = delta_time - 300;
    for(int i = 0; i < number_of_samples; i++){
        gettimeofday(&sample,NULL);
        s_arr[i] = sample.tv_sec;
        us_arr[i] = sample.tv_usec;
        usleep(delta_time);
        if(i%700 == 0)
            printf("Sampled %d\n",i);
    }
    // End of Main Loop
    gettimeofday(&end,NULL);
    readCPUusage();


    // Calculate Total Time Elapsed
    unsigned long difference_us;
    unsigned long difference_s = end.tv_sec - start.tv_sec;
    if(end.tv_usec >= start.tv_usec){
        difference_us = end.tv_usec - start.tv_usec; 
    }else{
        difference_us = start.tv_usec - end.tv_usec;
        difference_us = 1000000 - difference_us;
        difference_s++;
    }
    printf("Total Time = %lu s, %lu us\n",difference_s,difference_us);

    // Calculate the time between samples
    for(int i = number_of_samples-1; i > 0; i--){
        unsigned long difference_us;
        unsigned long difference_s = s_arr[i] - s_arr[i-1];
        if(us_arr[i] >= us_arr[i-1]){
            difference_us = us_arr[i] - us_arr[i-1]; 
        }else{
            difference_us = us_arr[i-1] - us_arr[i];
            difference_us = 1000000 - difference_us;
            difference_s++;
        }
        s_arr[i] = difference_s;
        us_arr[i] = difference_us;
    }

    // Print intervals to CSV
    printTimeStampsToCsv(s_arr,us_arr,number_of_samples,f_delta_time);


    free(s_arr);
    free(us_arr);
    return 0;
}

void printTimeStampsToCsv(unsigned long* ms, unsigned long* us, int length, float delta){
    FILE* f = fopen("timeStamps.csv","w");
    fprintf(f, "%d, %f\n",length,delta);
    for(int i = 0; i < length; i++){
        fprintf(f,"%lu, %lu\n",ms[i],us[i]);
    }

    fclose(f);
}
void readCPUusage(){
    FILE* f = fopen("/proc/stat", "r");
    char* line = malloc(512*sizeof(char));
    unsigned long buffsize = 512;
    getline(&line,&buffsize,f);
    printf("USAGE: %s\n",line);
}