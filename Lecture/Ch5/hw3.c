#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {

    pid_t child = fork();
    if (child < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }

    if (child == 0) {
        fprintf(stdout, "hello\n");
    }
    else {
        sleep(1);
        fprintf(stdout, "goodbye\n");
    }

    return 0;
}