#include <stdio.h>
#include <stdlib.h>

int fibonacci(int no)
{
  if (1 == no || 2 == no)
    return 1;
  return fibonacci(no-1) + fibonacci(no-2);
}

int main()
{
  short int numar, baza=10;
  char sir[1];

  scanf("%s", sir);
  numar=strtol(sir, NULL, baza);

  printf("fibonacci(%d)=%d\n", numar, fibonacci(numar));
  return 0;
}

