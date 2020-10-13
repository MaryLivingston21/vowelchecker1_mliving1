//
// Created by Mary Livingston on 2020-10-10.
//
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define SEM_NAME1 "/SEM1_mliving1"
#define SEM_NAME2 "/SEM2_mliving1"

#define BUFLEN 128

void *checker(void *param);

typedef struct {
    int numPhrases;
    sem_t *semaphore1;
    sem_t *semaphore2;
    pthread_mutex_t *mutex;
} SyncInfo;

char buffer[BUFLEN];
int allVowelsPresent;


//------------------------------------------------

void *checker(void *param) {

    SyncInfo *data;
    data = (SyncInfo *) param;
    sem_t *sem1, *sem2;
    pthread_mutex_t *mutex;
    int i, n;

    sem1 = data->semaphore1;
    sem2 = data->semaphore2;
    mutex = data->mutex;
    n = data->numPhrases;

    printf("checker: will check %d phrases\n", n);

    //printf("Checker: waiting for sem1 to start\n");
    sem_wait(sem1);
    //printf("Checker: started \n");


    for (i = 0; i<n; ++i) {
        char localBuffer[BUFLEN];


        //printf("I am checker, locking the mutex\n");
        pthread_mutex_lock(mutex);
        //printf("Checker: copy buffer into local buffer\n");
        strcpy(localBuffer, buffer);
        //printf("I am checker, unlocking the mutex\n");
        pthread_mutex_unlock(mutex);

        pthread_mutex_lock(mutex);
        allVowelsPresent = 0;
        pthread_mutex_unlock(mutex);

        if (strchr(localBuffer, 'a') != NULL) {
            if (strchr(localBuffer, 'e') != NULL){
                if (strchr(localBuffer, 'i') != NULL){
                    if (strchr(localBuffer, 'o') != NULL){
                        if (strchr(localBuffer, 'u') != NULL){
                            pthread_mutex_lock(mutex);
                            allVowelsPresent = 1;
                            pthread_mutex_unlock(mutex);
                            //printf(localBuffer);
                            //printf(" All vowels present \n");
                        }
                    }
                }
            }
        }
        printf("checked ’%s’: result is %d\n", localBuffer, allVowelsPresent);
        sem_post(sem2);
        //printf("Checker: waiting for sem1 to continue\n");
        sem_wait(sem1);
    }
    pthread_exit(0);
}

int main() {
    SyncInfo syncInfo;
    syncInfo.numPhrases = 8;
    sem_t *sem1, *sem2;
    pthread_mutex_t mutex;
    pthread_t tidOne;
    int i;
    char *phrases[] = { "educated", "educated cat", "educated lion", "serious person", "serious panda", "curious student", "curious art student", "obnoxious web developer"};

    sem1 = sem_open(SEM_NAME1, O_CREAT | O_EXCL, 0666, 0);
    if (sem1 == SEM_FAILED) {
        printf("sem1_open() failed\n");
        sem_unlink(SEM_NAME1);
        return(8);
    }

    sem2 = sem_open(SEM_NAME2, O_CREAT | O_EXCL, 0666, 0);
    if (sem2 == SEM_FAILED) {
        printf("sem2_open() failed\n");
        sem_unlink(SEM_NAME2);
        return(8);
    }

    pthread_mutex_init(&mutex, NULL);

    syncInfo.semaphore1 = sem1;
    syncInfo.semaphore2 = sem2;
    syncInfo.mutex = &mutex;

    // pthread_create() to create the second thread; pass it a reference to syncInfo
    pthread_create(&tidOne, NULL, checker, &syncInfo);


    for (i=0; i<syncInfo.numPhrases; ++i) {


        //printf("I am checker, locking the mutex\n");
        pthread_mutex_lock(&mutex);

        //write phrase into global buffer
        //printf("Main, adding phrase to buffer\n");
        strcpy(buffer, phrases[i]);

        //printf("I am checker, unlocking the mutex\n");
        pthread_mutex_unlock(&mutex);

        //printf("Main: sem1 posted\n");
        sem_post(sem1);

        //printf("Main: waiting for sem2\n");
        sem_wait(sem2);

        //print results
        printf("result of checking ’%s’: %d\n", phrases[i], allVowelsPresent);
    }

    //printf("Main: sem1 posted\n");
    sem_post(sem1);

    //wait for thread to terminate
    pthread_join(tidOne, NULL);

    sem_close(sem1);
    sem_unlink(SEM_NAME1);
    sem_close(sem2);
    sem_unlink(SEM_NAME2);

    return 0;
}
