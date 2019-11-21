#include <stdio.h>
#include <stdlib.h>

#define __USE_GNU
#include <string.h>
#include <signal.h>


static void  usr2_handler(int signum)
{
    printf(" recieved SIGINT\n");
}



int main(void)
{
    struct sigaction semnale;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    semnale.sa_flags = SA_RESTART;
    semnale.sa_mask = mask;
    semnale.sa_handler = usr2_handler;
    sigaction(SIGINT, &semnale, NULL);

    while(1);

    return 0 ;

}
