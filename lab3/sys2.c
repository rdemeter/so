#include <stdlib.h>
int main(int argc, char **argv)
{
  system("cd /etc/rc.d/rc$RUNLEVEL.d/; ls -la");
}

