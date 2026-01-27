#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int p2c[2]; // parent to child
    int c2p[2]; // child to parent

    if (pipe(p2c) < 0) { perror("pipe p2c"); exit(1); }
    if (pipe(c2p) < 0) { perror("pipe c2p"); exit(1); }

    pid_t pid = fork();
    if (pid < 0) { perror("fork"); exit(1); }

    if (pid == 0) {
        // child 
        close(p2c[1]); // child doesn't write to p2c
        close(c2p[0]); // child doesn't read from c2p

        int x;
        while (read(p2c[0], &x, sizeof(int)) == sizeof(int)) {
            if (x % 2 == 0) {
                write(c2p[1], &x, sizeof(int));
            }
        }

        close(p2c[0]);
        close(c2p[1]);
        _exit(0);

    } else {
        // parent 
        close(p2c[0]); // parent doesn't read from p2c
        close(c2p[1]); // parent doesn't write to c2p

        // send all CLI args as ints to child
        for (int i = 1; i < argc; i++) {
            int val = atoi(argv[i]);
            write(p2c[1], &val, sizeof(int));
        }

        // close write end so child sees EOF
        close(p2c[1]);

        // read evens back and print them
        int y;
        while (read(c2p[0], &y, sizeof(int)) == sizeof(int)) {
            printf("%d ", y);
        }
        printf("\n");

        close(c2p[0]);
        wait(NULL);
    }

    return 0;
}
