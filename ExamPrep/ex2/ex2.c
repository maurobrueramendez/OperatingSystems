// gcc -o ex2 ex2.c
// ./ex2

#include <stdio.h> 
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    char buff[1024];

    if(pid == 0) {
        close(fd[0]);
        char *args[] = {"ls", NULL};
        dup2(fd[1], STDOUT_FILENO);
        execvp(args[0], args);
        close(fd[1]);
    } else {
        close(fd[1]);
        int n = read(fd[0], buff, sizeof(buff) - 1);
        if (n > 0) {
            buff[n] = '\0';
            printf("%s", buff);
        }
        close(fd[0]);
        waitpid(pid, NULL, 0);
    }
    return 0;
}