// gcc -o ex6 ex6.c 
// ./ex6

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    int fd[2];
    pipe(fd);
    char buffer[1024];
    pid_t pid = fork();

    if(pid == 0) {
        close(fd[0]);
        char* args[] = {"ls", NULL};
        dup2(fd[1], STDOUT_FILENO);
        execvp(args[0], args);
        close(fd[1]);
    } else {
        close(fd[1]);
        int n = read(fd[0], buffer, sizeof(buffer) - 1);
        if(n > 0){
            buffer[n] = '\0';
            printf("%s", buffer);
        }
        close(fd[0]);
        waitpid(pid, NULL, 0);
    }

    return 0;
}