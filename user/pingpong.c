#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[]) {
    int p1[2];
    int p2[2];
    pipe(p1);
    pipe(p2);
    
    char buf[2] = "a";
    char child[2];
    char parent[2];


    if (fork() == 0) {
        close(p1[1]);
        close(p2[0]);

        if (write(p2[1], buf, 1) != 1) {
            printf("fail to write in child\n");
            exit(1);
        };
        if (read(p1[0], child, 1) != 1) {
            printf("fail to read in child\n");
            exit(1);
        };


        printf("%d: received ping\n", getpid());

        close(p1[0]);
        close(p2[1]);
        exit(0);
    }
    else {
        close(p1[0]);
        close(p2[1]);

        if (read(p2[0], parent, 1) != 1) {
            printf("fail to read in child\n");
            exit(1);
        };
        if (write(p1[1], parent, 1) != 1) {
            printf("fail to write in child\n");
            exit(1);
        };


        wait(0);
        printf("%d: received pong\n", getpid());

        close(p1[1]);
        close(p2[0]);
    }
    exit(0);
}