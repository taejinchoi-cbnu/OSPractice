#include <stdio.h>
#include <pthread.h>
#include "common.h"
#include "common_threads.h"

static volatile int counter = 0;

void *mythread(void *arg) {
    printf("%s: begin\n", (char*) arg);
    
    for (int i = 0; i < 1e7; i++) {
        counter = counter +1;
    }
    printf("%s: done\n", (char*) arg);
    return NULL;
}

int main(int argc, char* argv[]) {
    pthread_t p1, p2;
    printf("main: begin (counter = %d)\n", counter);
    pthread_create(&p1, NULL, mythread, "A");
    pthread_create(&p2, NULL, mythread, "B");

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("main: done with both (counter = %d)\n", counter);

    return 0;
}

/*user@DESKTOP-4NC27MI:~/Os_practice/Ch26$ ./a.out 
main: begin (counter = 0)
A: begin
B: begin
B: done
A: done
main: done with both (counter = 11568709)
user@DESKTOP-4NC27MI:~/Os_practice/Ch26$ ./a.out 
main: begin (counter = 0)
A: begin
B: begin
A: done
B: done
main: done with both (counter = 12094752)
user@DESKTOP-4NC27MI:~/Os_practice/Ch26$ ./a.out 
main: begin (counter = 0)
A: begin
B: begin
A: done
B: done
main: done with both (counter = 11778966)
*/

/* 실행 할 때마다 counter의 값이 달라짐! */