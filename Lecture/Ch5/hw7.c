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
    else if (rc == 0) {
        printf("before close stdout\n");

        close(STDOUT_FILENO);

        printf("cna i print??\n");
    }
    else {
        wait(0x0);
        printf("I'm parent!\n");
    }
    return 0;
}  