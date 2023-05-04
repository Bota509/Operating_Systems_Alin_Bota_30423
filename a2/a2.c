#define _XOPEN_SOURCE 600
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "a2_helper.h"
#include <sys/wait.h>
#include <semaphore.h>

#define P4_THREAD_NUM 4
#define P3_THREAD_NUM 45

sem_t semaphore1;
sem_t semaphore2;

sem_t semForEleven1;
sem_t semForEleven2;
sem_t waiting;

sem_t semForBoth1;
sem_t semForBoth2;

sem_t semForBoth3;



void *routine(void *arg)
{
    int *id = (int *)arg;
    if (*id == 1)
    {
        sem_wait(&semaphore1);
        info(BEGIN, 4, *id);
    }
    else if (*id == 4)
    {
        info(BEGIN, 4, *id);
        sem_post(&semaphore1);
    }
    else if (*id == 3)
    {
        // 7.3 s- a terminat si incepe 4.3

        sem_wait(&semForBoth1);
        info(BEGIN, 4, *id);
    }
    else
    {
        info(BEGIN, 4, *id);
    }

    if (*id == 4)
    {
        sem_wait(&semaphore2);
        info(END, 4, *id);
    }
    else if (*id == 1)
    {
        info(END, 4, *id);
        sem_post(&semaphore2);
    }
    else if (*id == 3)
    {
        // 4.3 sa terminat si poate incepe 7.4
        info(END, 4, *id);
        sem_post(&semForBoth2);
    }
    else
    {
        info(END, 4, *id);
    }
    return NULL;
}

void *routine2(void *arg)
{
    int *id = (int *)arg;

    sem_wait(&semForEleven1);

    info(BEGIN, 3, *id);

    if (*id == 11)
    {
        sem_wait(&semForEleven2);
        sem_wait(&semForEleven2);
        sem_wait(&semForEleven2);
        sem_wait(&semForEleven2);
        sem_wait(&semForEleven2);
        info(END, 3, *id);

        sem_post(&semForBoth3);
        sem_post(&semForBoth3);
        sem_post(&semForBoth3);
        sem_post(&semForBoth3);
        sem_post(&semForBoth3);
    }
    else if (*id > 11 && *id <= 16)
    {
        sem_post(&semForEleven2);
        sem_wait(&semForBoth3);
        info(END, 3, *id);
    }
    else
    {
        info(END, 3, *id);
    }

    sem_post(&semForEleven1);

    return NULL;
}

void *routine3(void *arg)
{

    int *id = (int *)arg;

    if (*id == 4)
    {

        // 4.3 sa terminat si poate incepe 7.4

        sem_wait(&semForBoth2);
        info(BEGIN, 7, *id);
    }
    else
    {
        info(BEGIN, 7, *id);
    }

    if (*id == 3)
    {
        // 7.3 s- a terminat si incepe 4.3

        info(END, 7, *id);
        sem_post(&semForBoth1);
    }
    else
    {
        info(END, 7, *id);
    }

    return NULL;
}

int main()
{
    int pid1, pid2, pid3, pid4, pid5, pid7, pid8, pid9;

    init();

    sem_init(&semForBoth1, 0, 1);
    sem_init(&semForBoth2, 0, 1);

    info(BEGIN, 1, 0);
    pid1 = fork();

    if (pid1 == 0) // child p2
    {

        // sunt in process 2
        info(BEGIN, 2, 0);
        pid2 = fork();
        if (pid2 == 0)
        {
            // process p4
            info(BEGIN, 4, 0);

            // TASK 2 AICI

            sem_init(&semaphore1, 0, 0);
            sem_init(&semaphore2, 0, 0);

            pthread_t threads[P4_THREAD_NUM];
            for (int i = 0; i < P4_THREAD_NUM; i++)
            {
                int *id = malloc(sizeof(int));
                *id = i + 1;
                if (pthread_create(&threads[i], NULL, &routine, id) != 0)
                    perror("Thread failed to be created");
            }

            for (int i = 0; i < P4_THREAD_NUM; i++)
            {
                if (pthread_join(threads[i], NULL) != 0)
                    perror("Threads failed to join");
            }

            info(END, 4, 0);
        }
        else
        {
            // in p2
            pid5 = fork();
            if (pid5 == 0)
            {
                // in process p6
                info(BEGIN, 6, 0);

                info(END, 6, 0);
            }
            else
            {
                wait(NULL);
                wait(NULL);
                info(END, 2, 0);
            }
        }
    }
    else
    {
        // parinte  -> adica process 1

        pid3 = fork();
        if (pid3 == 0) // sunt in process p3
        {
            info(BEGIN, 3, 0);

            pid7 = fork();
            if (pid7 == 0)
            {
                // is in p7
                info(BEGIN, 7, 0);
                // TASK 4

                pthread_t threadss[P4_THREAD_NUM];

                for (int i = 0; i < P4_THREAD_NUM; i++)
                {
                    int *id = malloc(sizeof(int));
                    *id = i + 1;
                    if (pthread_create(&threadss[i], NULL, &routine3, id) != 0)
                        perror("Thread failed to be created");
                }

                for (int i = 0; i < P4_THREAD_NUM; i++)
                {
                    if (pthread_join(threadss[i], NULL) != 0)
                        perror("Threads failed to join");
                }

                info(END, 7, 0);

            }
            else
            {
                // in p3
                pid8 = fork();
                if (pid8 == 0)
                {
                    // in p8
                    info(BEGIN, 8, 0);
                    pid9 = fork();
                    if (pid9 == 0)
                    {
                        // in process p9
                        info(BEGIN, 9, 0);

                        info(END, 9, 0);
                    }
                    else
                    {
                        wait(NULL);
                        info(END, 8, 0);
                    }
                }
                else
                {

                    // Task 3 aici
                    sem_init(&semForEleven1, 0, 6);
                    sem_init(&semForEleven2, 0, 0);
                    sem_init(&semForBoth3, 0, 0);

                    pthread_t threads[P3_THREAD_NUM];
                    for (int i = 10; i < 16; i++)
                    {
                        int *id = malloc(sizeof(int));
                        *id = i + 1;
                        if (pthread_create(&threads[i], NULL, &routine2, id) != 0)
                            perror("Thread failed to be created");
                    }

                    for (int i = 10; i < 16; i++)
                    {
                        if (pthread_join(threads[i], NULL) != 0)
                            perror("Threads failed to join");
                    }

                    for (int i = 0; i < 10; i++)
                    {
                        int *id = malloc(sizeof(int));
                        *id = i + 1;
                        if (pthread_create(&threads[i], NULL, &routine2, id) != 0)
                            perror("Thread failed to be created");
                    }

                    for (int i = 0; i < 10; i++)
                    {
                        if (pthread_join(threads[i], NULL) != 0)
                            perror("Threads failed to join");
                    }

                    for (int i = 16; i < P3_THREAD_NUM; i++)
                    {
                        int *id = malloc(sizeof(int));
                        *id = i + 1;
                        if (pthread_create(&threads[i], NULL, &routine2, id) != 0)
                            perror("Thread failed to be created");
                    }

                    for (int i = 16; i < P3_THREAD_NUM; i++)
                    {
                        if (pthread_join(threads[i], NULL) != 0)
                            perror("Threads failed to join");
                    }

                    wait(NULL);
                    wait(NULL);
                    info(END, 3, 0);
                }
            }
        }
        else
        {
            // tot primu proces -> p1
            pid4 = fork();
            if (pid4 == 0)
            {
                // in process 5
                info(BEGIN, 5, 0);
                info(END, 5, 0);
            }
            else
            {
                wait(NULL);
                wait(NULL);
                wait(NULL);
                info(END, 1, 0);
            }
        }
    }

    return 0;
}
