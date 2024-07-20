#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

static sig_atomic_t sigflag = 0;
static void sigusr1_handler()
{
    sigflag = 1;
}

int main(int argc, char *argv[])
{

    close(STDOUT_FILENO);
    open("test.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
    struct sigaction sa;
    sa.sa_handler = sigusr1_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {

        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    sigset_t mask, oldmask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);

    if (sigprocmask(SIG_BLOCK, &mask, &oldmask) == -1)
    {
        perror("sigprocmask");
        exit(EXIT_FAILURE);
    }

    pid_t rc = fork();
    if (rc < 0)
    {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    }
    else if (rc == 0)
    {
        printf("hello\n");
        kill(getppid(), SIGUSR1);
    }
    else
    {
        while (sigflag == 0)
        {
            sigsuspend(&oldmask);
        }
        printf("goodbye\n");
    }
}
