#include <unistd.h>

int main() {
    char buffer[1024];
    int bytes_read;
    int count = 0;

    while((bytes_read = read(0, buffer, sizeof(buffer))) > 0) {
        for(int i = 0; i < bytes_read; i++) {
            if(buffer[i] == 'a') {
                count++;
            }
        }
    }

    char out[20];       // output string buffer, eventually sent to write()
    int len = 0;        // tracks num of characters placed into out[]

    // after this, if count was 0, now out contains '0' and len is 1
    if(count == 0) {       
        out[len++] = '0';   // manually put '0' into buffer if count is 0 so as to run loop
    } else {
        int temp = count;   // store count since we will divide by 10 repeatedly
        char rev[20];       // temp buffer to hold digits in reverse order, since %10 gets last digit first
        int rlen = 0;       // number of digits stored in buffer

        while(temp > 0) {
            rev[rlen++] = (temp % 10) + '0';    // obtain last digit and convert to ASCII
            temp /= 10;     // chops off last digit
        }

        for(int i = rlen - 1; i >= 0; i--) {    // start at last index till 0
            out[len++] = rev[i];    // copy reversing into out
        }
    }

    out[len++] = '\n';

    write(1, out, len);
}