#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void pipeline(int p[2]) { //完全不会, 看别人代码看懂的

    close(p[1]);

    int flag;
    int prime;
    int n;

    read(p[0], &prime, 4);
    printf("prime %d\n", prime);
    flag = read(p[0], &n, 4);

    if (flag) {
        int newp[2];
        pipe(newp);
        if (fork() != 0) {
            close(newp[0]);
            if (n%prime) write(newp[1], &n, 4);
            while (read(p[0], &n, 4)) {
                if(n%prime) write(newp[1], &n, 4);
            }
            close(p[0]);
            close(newp[1]);
            wait(0);
        }
        else {
            close(newp[1]);
            pipeline(newp);
            close(newp[0]);
            }
        }
        exit(0);
}

int 
main(int argc, char* argv[]) {

    int p[2];
    pipe(p);

    if (fork() == 0) {

        pipeline(p);
    }
    else {
        close(p[0]);
        for (int i = 2; i < 35; i += 1) {
        write(p[1], &i, 4);
        }
        close(p[1]);
        wait(0);
    }
    exit(0);
}
