#include <unistd.h>   // read, write, close
#include <fcntl.h>    // open, O_* flags
#include <stdio.h>    // sscanf
#include <stdlib.h>   // size_t

static int write_all(int fd, const void *buf, size_t n) {
    const char *p = (const char *)buf;
    size_t total = 0;
    while(total < n) {
        ssize_t w = write(fd, p + total, n - total);
        if(w < 0) return -1;
        total += (size_t)w;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        const char *msg = "Usage: ./Ex3 <output_file>\n";
        write_all(2, msg, 29);
        return 1;
    }
    
    const char *out_path = argv[1];
    int fd_out = open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0640);
    if(fd_out < 0) {
        const char *msg = "Error: cannot open output file\n";
        write_all(2, msg, 32);
        return 1;
    }

    long long sum = 0;

    char buf[1024];
    char carry[2048];
    int carry_len = 0;

    while(1) {
        ssize_t r = read(0, buf, sizeof(buf));
        if(r < 0) {
            const char *msg = "Error: read failed \n";
            write_all(2, msg, 20);
            close(fd_out);
            return 1;
        }

        if(r == 0) break;     // EOF

        // append read bytes onto carry buffer
        if (carry_len + (int)r >= (int)sizeof(carry)) {
            const char *msg = "Error: input line too long\n";
            write_all(2, msg, 29);
            close(fd_out);
            return 1;
        }

        for (int i = 0; i < (int)r; i++) {
            carry[carry_len++] = buf[i];
        }

        // parse integers from carry
        int pos = 0;
        while(pos < carry_len) {
            while(pos < carry_len && (carry[pos] == ' ' || carry[pos] == '\n' || carry[pos] == '\t' || carry[pos] == '\r')) {
                pos++;
            }
            if(pos >= carry_len) break;

            int value;
            int consumed = 0;
            
            if(sscanf(&carry[pos], "%d%n", &value, &consumed) == 1) {
                sum += value;
                pos += consumed;
            } else {
                break;
            }
        }

        int leftover = carry_len - pos;
        for (int i = 0; i < leftover; i++) {
            carry[i] = carry[pos + i];
        }
        carry_len = leftover;
    }

    int out_sum = (int)sum;
    if (write_all(fd_out, &out_sum, sizeof(out_sum)) < 0) {
        const char *msg = "Error: write failed\n";
        write_all(2, msg, 21);
        close(fd_out);
        return 1;
    }

    close(fd_out);
    return 0;
}