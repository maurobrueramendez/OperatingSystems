// gcc -o ex13 ex13.c
// ./ex13

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    char buffer[1024];

    int file = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if(file < 0) {
        perror("Opening file");
        return 1;
    }

    if(pid == 0) {
        close(fd[0]);

        dup2(fd[1], STDOUT_FILENO);

        char* args[] = {"ls", NULL};
        execvp(args[0], args);

        close(fd[1]);
    } else {
        close(fd[1]);

        int n;
        while((n = read(fd[0], buffer, sizeof(buffer))) > 0) {
            write(file, buffer, n);
        }

        waitpid(pid, NULL, 0);
        close(file);
        close(fd[0]);
    }

    return 0;
}