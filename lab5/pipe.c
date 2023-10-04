#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    int   fd[2], nbytes;
    pid_t childpid;
    char  string[] = "Hello, world!\n";
    char  readbuffer[80];

    pipe(fd);

    if((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if(childpid == 0) {
        printf("Child process closes up input side of pipe\n");
        close(fd[0]);

        printf("Child: Send string through the output side of pipe\n");
        write(fd[1], string, (strlen(string)+1));
        printf("Child: Exiting\n");
        exit(0);
    }
    else {
        printf("Parent process closes up output side of pipe\n");
        close(fd[1]);

        printf("Parent: Read in a string from the pipe\n");
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Parent: Received string: %s", readbuffer);
    }
    return(0);
}
