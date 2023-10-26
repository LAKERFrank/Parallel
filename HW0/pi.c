#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int number_of_toss=1000000;

int main(){
    srand(time(NULL)) ;

    int number_in_circle=0;
    double x,y;
    double distance;
    double pi;

    for(int i=0;i<number_of_toss;i++){
        x = (double)rand()/(RAND_MAX)*2-1;
        //printf("x= %f\n",x);
        y = (double)rand()/(RAND_MAX)*2-1;
        // printf("y= %f\n",y);
        distance=x*x+y*y;
        if(distance<=1){
            number_in_circle++;
        }
    }
    pi=4*(double)number_in_circle/(double)number_of_toss;
    printf("pi= %f\n",pi);
}