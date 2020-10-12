////
//// Created by Jason D Hibbeler
////
//#include <stdio.h>
//#include <stdlib.h>
//#include <pthread.h>
//
//void *runner(void *param);
//void *boss(void *param);
//
//main() {
//    pthread_mutex_t mutex;
//    pthread_t tid1, tid2;
//
//    pthread_mutex_init(&mutex, NULL);
//
//    pthread_create(&tid2, NULL, boss, &mutex);
//    sleep(1);
//    pthread_create(&tid1, NULL, runner, &mutex);
//
//    pthread_join(tid1, NULL);
//    pthread_join(tid2, NULL);
//
//    return(0);
//}
//
//
//void *runner(void *param) {
//    pthread_mutex_t *mutex;
//
//    mutex = (pthread_mutex_t *) param;
//
//    printf("I am runner: waiting for the mutex\n");
//    pthread_mutex_lock(mutex);
//    printf("I am runner: done\n");
//
//    pthread_exit(0);
//}
//
//
//void *boss(void *param) {
//    pthread_mutex_t *mutex;
//
//    mutex = (pthread_mutex_t *) param;
//
//    printf("I am boss, locking the mutex\n");
//    pthread_mutex_lock(mutex);
//
//    printf("I am boss, sleeping for 5 seconds\n");
//    sleep(5);
//    printf("I am boss, unlocking the mutex\n");
//    pthread_mutex_unlock(mutex);
//    sleep(1);
//
//    pthread_exit(0);
//}
//
