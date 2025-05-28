#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int x = 100;
    printf("x from main process: %d\n", x);

    int child = fork();
    if (child < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (child == 0) {
        printf("x from child process(%d): %d\n", (int)getpid(), x);
        x = 200;
        printf("now x is %d\n", x);
    }
    else {
        printf("x from parent process(%d): %d\n", (int)getpid(), x);
        x = 300;
        printf("now x is %d\n", x);
    }
    return 0;
}

/* parent 프로세스를 복제하여 child 프로세스를 생성하므로 처음 x는 같지만 child 프로세스에서 변경한 값이
    parent 프로세스로 전달되지는 않음 */