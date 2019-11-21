#include <stdio.h>
#include <stdlib.h>

#define __USE_GNU
#include <string.h>
#include <signal.h>

static void  sigint_handler(int signum) {
    printf(" recieved SIGINT\n");
}

int main(void) {
    struct sigaction signal;
    sigset_t mask;

    sigemptyset(&mask);
    sigaddset(&mask, SIGINT);

    signal.sa_flags = SA_RESTART;
    signal.sa_mask = mask;
    signal.sa_handler = sigint_handler;
    sigaction(SIGINT, &signal, NULL);

    while(1);

    return 0;
}
