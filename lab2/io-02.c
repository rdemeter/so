#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main (void){
  int fd;
  char *buf;
  ssize_t bytes_read;

  /* alocam spatiu pentru buffer-ul de citire */
  buf = malloc(101);
  if (buf == NULL){
    perror ("malloc");
    exit (EXIT_FAILURE);
  }

  /* deschidem fisierul */
  fd = open ("gabi.txt", O_RDONLY);
  if (fd < 0) {
    perror ("open gabi.txt");
    exit (EXIT_FAILURE);
  }

  /* ne pozitionăm în fisier */
  if (lseek (fd, -100, SEEK_END) < 0) {
    perror ("lseek");
    exit (EXIT_FAILURE);
  }

  /* citim ultimele 100 caractere in buffer */
  bytes_read = read (fd, buf, 100);
  if (bytes_read < 0) {
    perror ("read");
    exit (EXIT_FAILURE);
  }
  buf[bytes_read] = '\0';

  /* afisam sirul citit */
  printf("file contents [%d bytes]: \n%s\n", bytes_read, buf);

  /* inchidem fisierul */
  close (fd);

  /* eliberam buffer-ul alocat */
  free (buf);
  return 0;
}
