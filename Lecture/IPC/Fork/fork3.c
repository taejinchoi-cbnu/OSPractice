#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

int main() {
    pid_t child;

    child = fork();
    if (child == 0) {
        int fd = open("hello.out", O_CREAT | O_WRONLY, 0644);
        dup2(fd, 1);
        close(fd);
        
        execl("./hello", "hello", (char *)0x0);
    }

    wait(0x0);
    return 0;
}