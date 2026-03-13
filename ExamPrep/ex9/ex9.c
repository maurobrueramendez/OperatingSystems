// gcc -o ex9 ex9.c
// ./ex9

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int p2c[2]; // pipe parent to child
    int c2p[2]; // pipe child to parent
    pipe(p2c);
    pipe(c2p);

    pid_t pid = fork();

    if(pid == 0) {
        close(c2p[0]);  // close c2p read 
        close(p2c[1]);  // close p2c write

        int x;
        read(p2c[0], &x, sizeof(x));
        int result = x * 2;
        write(c2p[1], &result, sizeof(result));

        close(c2p[1]);
        close(p2c[0]);
        _exit(0);   // not necessary
    } else {
        close(c2p[1]);
        close(p2c[0]);

        int x = 5;  // example
        write(p2c[1], &x, sizeof(x));
        int result;
        read(c2p[0], &result, sizeof(result));
        printf("Result is %d\n", result);

        close(c2p[0]);
        close(p2c[1]);
        waitpid(pid, NULL, 0);
    }

    return 0;
}