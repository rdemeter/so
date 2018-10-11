#include <stdio.h>
#include <stdlib.h>

#define __USE_GNU
#include <string.h>
#include <signal.h>
int main(void)
{
  char *sig_p;
  sig_p = strsignal(SIGKILL);
  printf("signal %d is %s\n", SIGKILL, sig_p);
  psignal(SIGKILL, "death and decay");
  return 0;
}


