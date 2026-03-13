// gcc -o ex7 ex7.c
// ./ex7

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Usage: %s <file>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd < 0) {
        perror("opening file");
        return 1;
    }

    char buf[1024];
    int n;
    long sum = 0;

    while((n = read(fd, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);
        sum += n;
    }

    close(fd);
    printf("Total number of bytes read is %ld\n", sum);
    return 0;
}