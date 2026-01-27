#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd;
    int pid;

    fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        perror("Failed to open file.");
        return -1;
    }

    pid = fork();
    if (pid < 0) {
        perror("PID Error.");
        return -1;
    }

    int delay = getpid();
    if(pid == 0) {
        usleep(delay);              // only delay child since child must be second
        write(fd, "World", 5);
    } else {
        write(fd, "Hello", 5);
    }

    close(fd);
    return 0;
}