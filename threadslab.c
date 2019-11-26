#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/wait.h>
#include <dirent.h>
#include <stdbool.h> 



static volatile int counter=0;

static volatile int max=0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int turn;
bool flag[2];


 void *Producer(void * args){

    for( int i=0;i<50;i++){
        //printf("%d\n",counter);
        pthread_mutex_lock(&lock);
        counter++;
        max++;
        pthread_mutex_unlock(&lock);

    }

    return NULL;
}


void *Consumer2(void * args){

    char * theConsumser;
    theConsumser=malloc(50);
    theConsumser=(char *) args;

    while(counter >0 || max <50){
         flag[0] = true;
         turn=1;
         while(flag[1]==true&& turn == 0);   
        
        if(counter>0){

        pthread_mutex_lock(&lock);
        counter--;
        pthread_mutex_unlock(&lock);

        printf("%s consumed 1 product\n\n",theConsumser );

        }   

        flag[0]=false;

    }

    return NULL;
}

void *Consumer(void * args){

    char * theConsumser;
    theConsumser=malloc(50);
    theConsumser=(char *) args;
        

    while(counter >0 || max<50 ){
        flag[1] = true;
        turn=0;
        while(flag[0]==true && turn == 1);

        if (counter>0){
        
        pthread_mutex_lock(&lock);
        counter--;
        pthread_mutex_unlock(&lock);

        printf("%s consumed 1 product\n",theConsumser );        
        }

        flag[1]=false;

}

    return NULL;
}

 int main(){
    
    char *consumer1= "consumer 1";
    char *consumer2= "consumer 2";

    pthread_t p1,p2,p3;

    if (pthread_create(&p1,NULL,Producer,NULL)){
            printf("ERROR\n");
    }
    if( pthread_create(&p2,NULL,Consumer,(void*) consumer1)){
        printf("ERROR\n");
    }
    if(pthread_create(&p3,NULL,Consumer2,(void *) consumer2)){
        printf("ERROR\n");
    }
        
    pthread_join(p1,NULL);
    pthread_join(p2,NULL);
    pthread_join(p3,NULL);

    
    pthread_mutex_destroy(&lock);
    return 0;
 }
