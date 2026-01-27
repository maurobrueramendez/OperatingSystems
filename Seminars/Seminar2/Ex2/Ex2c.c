#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    for (int i = 0; i < 100; i++) {
        pid_t pid = fork();

        if (pid == 0) {
            // child
            usleep(1000);
            _exit(0);
        } else {
            // parent
            wait(NULL);
        }
    }
    return 0;
}