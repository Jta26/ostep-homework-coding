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
    char *args[] = {"ls"};
    // l = list
    // v = array (vector?)
    // p = path
    // e = environment
    execle(args[0], args[0], NULL);
}
