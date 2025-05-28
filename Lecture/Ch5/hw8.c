#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <error.h>

int pipefd[2];

int main(int argc, char *argv[]) {
    if (pipe(pipefd) < 0) {
        perror("pipe failed\n");
        exit(1);
    }

    // printf("%d\n", pipefd[0]); 3번 읽기 전용
    // printf("%d\n", pipefd[1]); 4번 쓰기 전용

    pid_t rc1 = fork();
    if (rc1 < 0) {
        perror("rc1 fork failed\n");
        exit(1);
    }
    else if (rc1 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        printf("Hello I'm first child: %d\n", (int)getpid());
        exit(0);
    }

    pid_t rc2 = fork();
    if (rc2 < 0) {
        perror("rc1 fork failed\n");
        exit(1);
    }
    else if (rc2 == 0) {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        printf("Hello I'm second child: %d\n", (int)getpid());
        exit(0);
    }
    
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(rc1, NULL, 0);
    waitpid(rc2, NULL, 0);

    printf("parent finished\n");
    
    return 0;
}