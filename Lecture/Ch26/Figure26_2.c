#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include "common.h"
#include "common_threads.h"

void *mythread(void *arg) {
    printf("%s\n", (char*) arg);
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t p1, p2;
        
    printf("main: begin\n");

    char *mesaage1= "hello A";
    char *mesaage2 = "hello B";

    Pthread_create(&p1, NULL, mythread, (void *)&mesaage1);
    Pthread_create(&p2, NULL, mythread, (void *)&mesaage2);
    
    Pthread_join(p1, NULL);
    Pthread_join(p2, NULL);
    printf("main: end\n");
    
    return 0;
}

/*
~/Os_practice/Ch26$ ./Figure26_2 
main: begin
A
B
main: end
~/Os_practice/Ch26$ ./Figure26_2 
main: begin
B
A
main: end
*/

/* 실행 순간에 따라 먼저 실행되는 스레드가 다르다!! */
