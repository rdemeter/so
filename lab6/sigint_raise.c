#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void signalHandler( int signum ) {
   printf("Interrupt signal %d received.\n", signum);

   // cleanup and close up stuff here
   // terminate program

   exit(signum);
}

int main () {
   int i = 0;
   // register signal SIGINT and signal handler
   signal(SIGINT, signalHandler);

   while(++i) {
      printf("Going to sleep....\n");
      if( i == 3 ) {
         raise( SIGINT );
      }
      sleep(1);
   }

   return 0;
}
