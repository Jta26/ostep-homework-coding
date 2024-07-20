#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

// Write a program that creates a child process, and then in the child
// closes standard output (STDOUT FILENO). What happens if the child
// calls printf() to print some output after closing the descriptor?

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
        close(STDOUT_FILENO);
        printf("hello\n");
    }
    else
    {
        wait(NULL);
        printf("child process finished\n");
    }
    exit(0);
}

// Answers
// 1. It only closes the file descriptor in the child, the parent is still able to print to stdout