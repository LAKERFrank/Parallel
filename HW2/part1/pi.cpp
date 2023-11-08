#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <mutex>
#include <sstream>

using namespace std;

int threadNum;
unsigned int seed=1;
long long int validSum=0;
long long int tossesNum;
pthread_mutex_t pi_mutex;

uint64_t shuffle_table[4]={rand_r(&seed),rand_r(&seed),rand_r(&seed),rand_r(&seed)};
// The actual algorithm
uint64_t next(void)
{
    uint64_t s1 = shuffle_table[0];
    uint64_t s0 = shuffle_table[1];
    uint64_t result = s0 + s1;
    shuffle_table[0] = s0;
    s1 ^= s1 << 23;
    shuffle_table[1] = s1 ^ s0 ^ (s1 >> 18) ^ (s0 >> 5);
    return result;
}

void *count_valid(void*){

    double x,y,distance;
    long long int averageWork = tossesNum / threadNum;
    long long int threadValidNum=0;

    for(int i=0;i<averageWork;i++){
        x = (double)(next() %  (RAND_MAX)) / (RAND_MAX);
        y = (double)(next() %  (RAND_MAX)) / (RAND_MAX);
        distance = x*x + y*y;
        if(distance<=1){
            threadValidNum++;
        }
    }
    pthread_mutex_lock(&pi_mutex);
    validSum+=threadValidNum;
    pthread_mutex_unlock(&pi_mutex);
    return NULL;
}


int main(int argc, char *argv[]){

    (void)argc;

    istringstream var1(argv[1]);
    istringstream var2(argv[2]);

    var1 >> threadNum;
    var2 >> tossesNum;

    double pi;

    pthread_t thread[threadNum];

    for(int t=0;t<threadNum;t++){
        pthread_create(&thread[t],NULL,count_valid,NULL);
    }
    for(int t=0;t<threadNum;t++){
        pthread_join(thread[t],NULL);
    }
    pi=(double)(4*validSum)/(double)tossesNum;
    cout << pi << endl;
}