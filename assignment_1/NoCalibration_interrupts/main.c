#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>       /* for setitimer */
#include <unistd.h>         /* for pause */
#include <signal.h>         /* for signal */
#include <time.h>

void printTimeStampsToCsv(unsigned long* ms, unsigned long* us, int length, float delta);

unsigned long *s_arr, *us_arr;
int number_of_samples;
float f_delta_time;
unsigned long delta_time;
struct timeval sample;
int counter = 0;


void DoStuff(void) {

    struct timeval deigma;
    gettimeofday(&deigma, NULL);
    us_arr[counter] = deigma.tv_usec;
    s_arr[counter] = deigma.tv_sec;
    counter++;

  struct itimerval it_val;          /* for setting itimer */
  it_val.it_value.tv_sec =     0;   //INTERVAL/1000;
  it_val.it_value.tv_usec =    0;   //(INTERVAL*1000) % 1000000;   
  it_val.it_interval = it_val.it_value;
    if(counter == number_of_samples-1){
        setitimer(ITIMER_PROF, &it_val,NULL);
    }
    
  //printf("Timer went off. %d\n",counter);

}

int main(int argc, char** argv){

    if(argc < 2){
        printf("Not Enough Arguments\n arg1::int number of samples, arg2::float delta time\n");
        return 0;
    }
    number_of_samples = atoi(argv[1]);
    f_delta_time = atof(argv[2]);
    delta_time = 1000*f_delta_time; // tranform seconds to micro seconds for usleep
    printf("Samples: %d\n Delta Time: %f s OR %lu us\n",number_of_samples,f_delta_time, delta_time);
    

    s_arr = (unsigned long*)malloc(number_of_samples*sizeof(unsigned long));
    us_arr = (unsigned long*)malloc(number_of_samples*sizeof(unsigned long));

    struct itimerval it_val;  /* for setting itimer */

    /* Upon SIGALRM, call DoStuff().
    * Set interval timer.  We want frequency in ms, 
    * but the setitimer call needs seconds and useconds. */
    if (signal(SIGALRM, (void (*)(int)) DoStuff) == SIG_ERR) {
        perror("Unable to catch SIGALRM");
        exit(1);
    }
    it_val.it_value.tv_sec =     delta_time/1000;
    it_val.it_value.tv_usec =    (delta_time*1000) % 1000000;   
    it_val.it_interval = it_val.it_value;
    if (setitimer(ITIMER_REAL, &it_val, NULL) == -1) {
        perror("error calling setitimer()");
        exit(1);
    }

    while (counter < number_of_samples) 
        pause();

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