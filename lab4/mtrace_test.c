#include <stdlib.h>
#include <mcheck.h>

int main(void)
{
  /* start memcall monitoring */
  mtrace();

  malloc(10);
  malloc(20);
  malloc(30);

  /* stop memcall monitoring */
  muntrace();

  return 0;
}
