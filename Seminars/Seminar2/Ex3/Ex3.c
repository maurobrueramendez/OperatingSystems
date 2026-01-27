#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int fd[2];
    if (pipe(fd) < 0) {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {
        // child, read parent's PID from pipe
        close(fd[1]);       // close write end

        pid_t parent_pid_from_pipe;
        int n = read(fd[0], &parent_pid_from_pipe, sizeof(pid_t));
        if (n != sizeof(pid_t)) {
            perror("read");
            _exit(1);
        }

        pid_t real_parent = getppid();

        if (parent_pid_from_pipe == real_parent) {
            write(1, "OK: PID received matches getppid()\n", 34);

        } else {
            write(1, "ERROR: PID mismatch\n", 20);
        }

        close(fd[0]);
        _exit(0);

    } else {
        // parent write its PID to pipe
        close(fd[0]);       // close read end

        pid_t mypid = getpid();
        int n = write(fd[1], &mypid, sizeof(pid_t));
        if (n != sizeof(pid_t)) {
            perror("write");
        }

        close(fd[1]);
        wait(NULL);
    }

    return 0;
}