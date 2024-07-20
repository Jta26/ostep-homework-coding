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
    pid_t cpid2 = fork();

    if (cpid < 0 || cpid2 < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    }
    if (cpid == 0)
    {
        sleep(3);
        printf("child process 1 finished\n");
    }
    if (cpid2 == 0)
    {
        printf("child process 2 finished\n");
    }
    else
    {
        printf("parent process waiting for child process 1 to finish\n");
        pid_t wait_id = waitpid(cpid, NULL, 0);
        printf("returned to parent\n");
    }
}

// Answers
// 1. wait pid is useful for when you want to wait for a specific child process to finish, and allow other child processes to continue running