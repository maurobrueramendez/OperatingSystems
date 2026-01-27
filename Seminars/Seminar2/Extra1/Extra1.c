#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        write(2, "Usage: ./exe N\n", 15);
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        write(2, "N must be > 0\n", 13);
        return 1;
    }

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(1);
        }

        if (pid == 0) {
            // child, execute "ps <my_pid>"
            char pid_str[32];
            int len = snprintf(pid_str, sizeof(pid_str), "%d", (int)getpid());
            if (len < 0) _exit(1);

            char *args[] = {"ps", pid_str, NULL};
            execvp("ps", args);

            perror("execvp");
            _exit(1);
        }
    }

    // parent, wait for all children
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}
