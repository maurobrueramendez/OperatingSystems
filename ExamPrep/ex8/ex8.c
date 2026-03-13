// gcc -o ex8 ex8.c
// ./ex8

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd[2];
    pipe(fd);
    char buffer[1024];
    pid_t pid = fork();

    int file = open("out.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if(file < 0) {
        perror("Open file");
        return -1;
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
            write(file, buffer, sizeof(buffer));
        }

        close(fd[0]);
        close(file);
        waitpid(pid, NULL, 0);
    }

    return 0;
}