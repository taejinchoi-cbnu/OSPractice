#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    int fd = open("./test.txt", O_CREAT|O_WRONLY|O_TRUNC, S_IRWXU);
    if (fd < 0) {
        fprintf(stderr, "open failed\n");
        exit(1);
    }

    int child = fork();
    if (child < 0) {
        fprintf(stderr, "fork failed\n");
        exit(1);
    }
    else if (child == 0) {
        char *child_msg = "Hello I'm Child!!\n";
        write(fd, child_msg, strlen(child_msg));
    }
    else {
        char *parent_msg = "Hello I'm Parent!!\n";
        write(fd, parent_msg, strlen(parent_msg));
    }

    close(fd);
    return 0;
}