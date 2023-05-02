#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "a2_helper.h"
#include <sys/wait.h>
#include <semaphore.h>


#define P4_THREAD_NUM 4

sem_t semaphore1;
sem_t semaphore2;


void *routine(void* arg)
{
    int *id= (int*) arg;
   if(*id == 1)
   {
    sem_wait(&semaphore1);
    info(BEGIN,4,*id);

   }
   else if(*id == 4)
   {
    info(BEGIN,4,*id);
    sem_post(&semaphore1);
    
   }
   else
   {
    info(BEGIN,4,*id);
   }


   if(*id == 4)
   {
    sem_wait(&semaphore2);
    info(END,4,*id);

   }
   else if(*id == 1)
   {
    info(END,4,*id);
    sem_post(&semaphore2);
    
   }
   else
   {
    info(END,4,*id);
   }
    return NULL;

}

int main() {
    int pid1, pid2, pid3, pid4,pid5,pid7,pid8,pid9;


    init();
    

//vezi video threads in c
//vezi video semaphores in c
//vezi video processes in c


    info(BEGIN,1,0);
    pid1 = fork();
    

    if(pid1 == 0) //child p2
    {
        
        //sunt in process 2
        info(BEGIN,2,0);
        pid2 = fork();
        if(pid2==0)
        {
            //process p4
            info(BEGIN,4,0);


            //TASK 2 AICI 

            sem_init(&semaphore1,0,0);
             sem_init(&semaphore2,0,0);

pthread_t threads[P4_THREAD_NUM];
            for(int i=0;i<P4_THREAD_NUM;i++)
            {
                int* id = malloc(sizeof(int));
               *id =i+1;
             if(pthread_create(&threads[i],NULL,&routine,id)!=0)
                perror("Thread failed to be created");
            }

            for(int i=0;i<P4_THREAD_NUM;i++)
            {
                if(pthread_join(threads[i],NULL)!=0)
                    perror("Threads failed to join");
            }



            
            info(END,4,0);
        }
        else
        {
            //in p2
            pid5 = fork();
            if(pid5 == 0)
            {
                //in process p6
                info(BEGIN,6,0);

                info(END,6,0);

            }
            else
            {
                wait(NULL);
                wait(NULL);
                info(END,2,0);
            }
        }


        

    }
    else
    {
        // parinte  -> adica process 1
        
        pid3 = fork();
        if(pid3 == 0) // sunt in process p3
        {
            info(BEGIN,3,0);
            pid7 = fork();
            if(pid7 == 0)
            {
                //is in p7
                info(BEGIN,7,0);

                info(END,7,0);

            }
            else
            {
                //in p3 
                pid8 = fork();
                if(pid8 == 0)
                {
                    //in p8
                    info(BEGIN,8,0);
                    pid9 = fork();
                    if(pid9 == 0)
                    {
                        //in process p9
                        info(BEGIN,9,0);



                        info(END,9,0);
                    }
                    else
                    {
                        wait(NULL);
                        info(END,8,0);
                    }

                }
                else
                {
                    wait(NULL);
                    wait(NULL);
                    info(END,3,0);
                }

            }

        }
        else
        {
            //tot primu proces -> p1
            pid4 = fork();
            if(pid4 == 0)
            {
                //in process 5
                info(BEGIN,5,0);
                info(END,5,0);
            }
            else
            {
                wait(NULL);
                wait(NULL);
                wait(NULL);
                info(END,1,0);
            }
            




        }

    }
    
    return 0;
}
