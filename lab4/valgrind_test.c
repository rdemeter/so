#include <stdlib.h>
#include <string.h>

int main(void)
{
  char buf[10];
  char *p;
  /* no init */
  strcat(buf, "al");
  /* overflow */
  buf[11] = 'a';
  p = malloc(70);
  p[10] = 5;
  free(p);
  /* write after free */
  p[1] = 'a';
  p = malloc(10);
  /* memory leak */
  p = malloc(10);
  /* underrun */
  p--;
  *p = 'a';
  return 0;
}


