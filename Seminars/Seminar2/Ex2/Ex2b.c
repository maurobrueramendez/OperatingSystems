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
            char *args[] = {"sleep", "1", NULL};
            execvp("sleep", args);                  // replaces child memory

            perror("execvp");
            _exit(1);
        } 
    }

    for (int i = 0; i < 100; i++) {
        // parent
        wait(NULL);
    }

    return 0;
}