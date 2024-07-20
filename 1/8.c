#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

// Write a program that creates two children, and connects the standard output of one to the standard input of the other, using the
// pipe() system call.

int main(int argc, char *argv[])
{

    // creates the pipe between the 2 file descriptors
    int pipefd[2];
    pid_t pipe_id = pipe(pipefd);
    // if it's -1, it failed to create it, but who knows why
    if (pipe_id == -1)
    {
        fprintf(stderr, "pipe failed\n");
        exit(EXIT_FAILURE);
    }

    // create space in memory for the 2 child process ids
    pid_t cpid[2];

    // fork the first child process
    cpid[0] = fork();

    if (cpid[0] < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (cpid[0] == 0)
    {
        // close the read end of the pipe for the first child
        close(pipefd[0]);
        if (pipefd[1] != STDOUT_FILENO)
        {
            // duplicates the stdout file descriptor to the write end of the pipe (closes the second arg)
            int stdout_dup = dup2(pipefd[1], STDOUT_FILENO);
            if (stdout_dup != STDOUT_FILENO)
            {

                fprintf(stderr, "dup2 failed\n");
                exit(EXIT_FAILURE);
            }
            // closes the stdout for this child
            close(pipefd[1]);
        }
        // prints to the write end of the pipe
        printf("hello from process 1");
    }
    else
    {
        // fork the second child process
        cpid[1] = fork();
        if (cpid[1] < 0)
        {
            fprintf(stderr, "fork failed\n");
            exit(EXIT_FAILURE);
        }
        if (cpid[1] == 0)
        {
            // close the write end of the pipe for the second child (unused)
            close(pipefd[1]);
            if (pipefd[0] != STDIN_FILENO)
            {
                // duplicates the stdin file descriptor to the read end of the pipe (closes the second arg)
                int stdin_dup = dup2(pipefd[0], STDIN_FILENO);
                if (stdin_dup != STDIN_FILENO)
                {
                    fprintf(stderr, "dup2 failed\n");
                    exit(EXIT_FAILURE);
                }
            }
            // create space in memory to store the read data
            char buf[BUFSIZ];
            read(pipefd[0], buf, BUFSIZ);
            // prints the data read from the pipe
            printf("read in process 2 from the pipe: %s\n", buf);
        }
        else
        {
            // waits for process 1 to finish writing to the pipe
            waitpid(cpid[0], NULL, 0);
            // waits for process 2 to finish reading from the pipe (and printing to the unclosed stdout in child process 2)
            waitpid(cpid[1], NULL, 0);
            printf("child processes finished\n");
        }
    }

    return 0;
}
