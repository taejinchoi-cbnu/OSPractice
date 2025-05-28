#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int pipe1[2];
    int pipe2[2];

    pipe(pipe1);
    int pc1 = fork();
    if (pc1 > 0) {
        close(pipe1[0]);
        dup2(pipe1[1], 1);
        close(pipe1[1]);
        execlp("cat", "cat", "input.txt", NULL);
    }
    else if (pc1 == 0) {
        pipe(pipe2);
        int pc2 = fork();
        if (pc2 > 0) {
            close(pipe1[1]);
            close(pipe2[0]);
            dup2(pipe1[0], 0);
            dup2(pipe2[1], 1);
            close(pipe1[0]);
            close(pipe2[1]);
            execlp("sort", "sort", NULL);
        }
        else if (pc2 == 0) {
            close(pipe1[0]);
            close(pipe1[1]);
            close(pipe2[1]);
            dup2(pipe2[0], 0);
            close(pipe2[0]);
            execlp("uniq", "uniq", NULL);
        }
    }
    
    return 0;
}