#include <unistd.h>
#include <fcntl.h>

static int write_all(int fd, const void *buf, int n) {
    const char *p = (const char *)buf;
    int total = 0;
    while (total < n) {
        int w = (int)write(fd, p + total, (size_t)(n - total));
        if (w < 0) return -1;
        total += w;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        const char *msg = "Usage: ./Ex4 <file>\n";
        write_all(2, msg, 21);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        const char *msg = "Error: cannot open file\n";
        write_all(2, msg, 24);
        return 1;
    }

    off_t size = lseek(fd, 0, SEEK_END); // SEEK_END once
    if (size < 0) {
        const char *msg = "Error: lseek failed\n";
        write_all(2, msg, 20);
        close(fd);
        return 1;
    }

    char c;

    // We maintain progress by stepping back TWO bytes each loop:
    // -1 to point at the byte we want
    // read() moves +1
    // -1 again (net -1 per loop would get stuck), so instead we do:
    // lseek(-2), read(), and we advance backwards one byte each iteration.
    for (off_t i = 0; i < size; i++) {
        if (lseek(fd, -1, SEEK_CUR) < 0) { // step to previous byte
            const char *msg = "Error: lseek failed\n";
            write_all(2, msg, 20);
            close(fd);
            return 1;
        }

        if (read(fd, &c, 1) != 1) {
            const char *msg = "Error: read failed\n";
            write_all(2, msg, 19);
            close(fd);
            return 1;
        }

        write_all(1, &c, 1);

        // After read, offset is back where it was before the lseek(-1).
        // So we must move back ONE MORE to make net progress.
        if (lseek(fd, -1, SEEK_CUR) < 0) {
            // This will fail once we pass the start; that's okay at the end.
            break;
        }
    }

    close(fd);
    return 0;
}