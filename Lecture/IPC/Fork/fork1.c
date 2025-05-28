#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t child;

    printf("Begin\n");

    child = fork();
    if (child < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (child == 0) {
		execl("./hello", "Hello", (char *) 0x0) ;
    }
    else {
        wait(0x0);
        printf("End\n");
    }
    return 0;
}