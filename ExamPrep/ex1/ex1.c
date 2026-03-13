// gcc -o ex1 ex1.c
// ./ex1

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int maint() {
    int fd[2];
    pipe(fd);

    for(int i = 0; i < 2; i++) {
        pid_t pid = fork();
        if(pid == 0) {
            close(fd[0]);
            pid_t mypid = getpid();
            write(fd[1], &mypid, sizeof(pid_t));
            close(fd[1]);
            _exit(0);
        }
    }

    close(fd[1]);
    for(int i = 0; i < 2; i++) {
        pid_t p;
        read(fd[0], &p, sizeof(pid_t));
        printf("%d\n", p);
    }
    close(fd[0]);
    for(int i = 0; i < 2; i++) {
        wait(NULL);
    }
    return 0;
}
