#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]) {
    pid_t rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }

    if (rc == 0) {
        fprintf(stdout, "Child process (pid:%d) about to exec /bin/ls\n", (int)getpid());
        execl("/bin/ls", "ls", "-l", NULL);
        printf("This shouldn't print out\n");
        exit(1);
    }
    else {
        // 부모 프로세스
        wait(NULL);
        printf("Parent process: child %d has completed\n", rc);
    }

    return 0;
}