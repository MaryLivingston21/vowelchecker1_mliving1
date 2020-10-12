//
// Created by Mary Livingston on 2020-10-10.
//

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFLEN 128

void *checker(void *param);

typedef struct {
    int numPhrases;
} SyncInfo;

char buffer[BUFLEN];
int allVowelsPresent;

//------------------------------------------------

void *checker(void *param) {

    SyncInfo *data;
    data = (SyncInfo *) param;

    int i, n;
    n = data->numPhrases;

    for (i = 0; i<n; ++i) {
        char localBuffer[BUFLEN];
        strcpy(localBuffer, buffer);

        allVowelsPresent = 0;
        if (strchr(localBuffer, 'a') != NULL) {
            if (strchr(localBuffer, 'e') != NULL){
                if (strchr(localBuffer, 'i') != NULL){
                    if (strchr(localBuffer, 'o') != NULL){
                        if (strchr(localBuffer, 'u') != NULL){
                            allVowelsPresent = 1;
                            //printf("All vowels present");
                        }
                    }
                }
            }
        }
    }
    pthread_exit(0);
}

int main() {
    SyncInfo syncInfo;
    syncInfo.numPhrases = 8;
    pthread_t tidOne;
    int i;

    char *phrases[] = { "educated", "educated cat", "educated lion", "serious person", "serious panda", "curious student", "curious art student", "obnoxious web developer"};


    // pthread_create() to create the second thread; pass it a reference to syncInfo
    pthread_create(&tidOne, NULL, checker, &syncInfo);

    for (i=0; i<syncInfo.numPhrases; ++i) {
        //write phrase into global buffer
        strcpy(buffer, phrases[i]);
        //print results
        printf("result of checking ’%s’: %d\n", phrases[i], allVowelsPresent);
    }

    //wait for thread to terminate
    pthread_join(tidOne, NULL);

}
