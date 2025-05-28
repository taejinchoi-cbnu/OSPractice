#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    pid_t rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }

    if (rc == 0) {
        fprintf(stdout, "child is %d\n", (int)getpid());
        
        int wc = wait(0x0);
        fprintf(stdout, "value of child wait is %d\n", wc);
    }
    else {
        fprintf(stdout, "parent is %d\n", (int)getpid());

        int wc = waitpid(rc, NULL, 0);
        fprintf(stdout, "value of parent wait is %d\n", wc);
    }

    return 0;
}