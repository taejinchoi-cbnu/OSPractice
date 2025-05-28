#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main() {
    pid_t child;
    child = fork();

    if (child < 0) {
        // fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (child == 0) {
        // child process
        printf("hello I'm child\n");
    }
    else {
        // parent processs
        printf("hello I'm parent of %d\n", (int)getpid());
    }

    return 0;
}