#include <unistd.h>     // read/write/close
#include <fcntl.h>      // open + flags like create for files
#include <stdlib.h>     // rand/srand
#include <time.h>
#include <stdio.h>      // sprintf (formats int into char buffer)

// write() is allowed to write fewer bytes than we're asked for
// so create write_all() function to loop until all is written
static int write_all(int fd, const void *buf, size_t n) {
    const char *p = (const char *)buf;  // convert buf to byte pointer to do pointer math
    size_t total = 0;       // tracks bytes written
    
    while(total < n) {
        ssize_t w = write(fd, p + total, n - total);    // bytes written
        if(w < 0) return -1;
        total += (size_t)w;
    }

    return 0;
}

// argc is number of command line arguments, require two paths argv[]
int main(int argc, char *argv[]) {
    if(argc != 3) {
        const char *msg = "Usage: ./Ex2 <text_file> <binary_file>\n";
        write_all(2, msg, 41);  // stderr (error output)
        return 1;
    }

    const char *text_path = argv[1];
    const char *bin_path = argv[2];

    // open() provides a handle to the file, O_... will write, create, or overwrite if it exists/not
    int fd_text = open(text_path, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (fd_text < 0) {
        const char *msg = "Error: cannot open text file\n";
        write_all(2, msg, 32);
        return 1;
    }

    int fd_bin = open(bin_path, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if (fd_bin < 0) {
        const char *msg = "Error: cannot open bin file\n";
        write_all(2, msg, 32);
        return 1;
    }

    // seed randomness
    srand((unsigned)time(NULL));    // time(NULL) = current time in seconds

    // write raw int in binary
    for(int i = 0; i < 100; i++) {
        int x = rand();     // one random int
        if(write_all(fd_bin, &x, sizeof(int)) < 0) {
            const char *msg = "Error: write to binary failed\n";
            write_all(2, msg, 31);
            close(fd_text);
            close(fd_bin);
            return 1;
        }

        char buf[64];
        int len = 0;

        len = sprintf(buf, "%d", x);    // convert the int into characters
        if(write_all(fd_text, buf, (size_t)len) < 0) {
            const char *msg = "Error: write to text failed\n";
            write_all(2, msg, 29);
            close(fd_text);
            close(fd_bin);
            return 1;
        }

        // add commmas
        if(i != 99) { 
            char comma = ',';
            if(write_all(fd_text, &comma, 1) < 0) {
                const char *msg = "Error: write comma failed\n";
                write_all(2, msg, 26);
                close(fd_text);
                close(fd_bin);
                return 1;
            }
        }
    }

    close(fd_text);
    close(fd_bin);
    return 0;
}