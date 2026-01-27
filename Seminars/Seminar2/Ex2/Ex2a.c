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
            sleep(1);
            _exit(0);
        } 
    }

    for (int i = 0; i < 100; i++) {
        // parent
        wait(NULL);
    }

    return 0;
}