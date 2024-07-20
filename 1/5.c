#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

int main(int argc, char *argv[])
{

    pid_t cpid = fork();

    if (cpid < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        printf("child process finished\n");
        // returns -1
        pid_t what_child_id = wait(NULL);
        printf("child process id: %d\n", what_child_id);
    }
    else
    {
        printf("parent process waiting for child\n");
        pid_t wait_id = wait(NULL);
        printf("returned to parent\n");
    }
}

// answers
// 1. wait returns the child process id that died
// 2. if you call wait in the child process that has no children, it will return -1
