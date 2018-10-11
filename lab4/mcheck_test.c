#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
  int *v1;

  v1 = (int *) malloc(5 * sizeof(*v1));
  if (NULL == v1) {
    perror("malloc");
    exit (EXIT_FAILURE);
  }
  /* overflow */
  v1[6] = 100;

  free(v1);

  /* write after free */
  v1[6] = 100;

  /* reallocate v1 */
  v1 = malloc(10 * sizeof(int));
  if (NULL == v1) {
    perror("malloc");
    exit (EXIT_FAILURE);
  }

  return 0;
}
