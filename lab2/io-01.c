#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {

  int fd1, fd2;
  fd1 = open ("alina.txt", O_WRONLY | O_TRUNC);
  if (fd1 < 0) {
    perror ("open alina");
    exit (EXIT_FAILURE);
  }

  fd2 = open ("dan.txt", O_RDWR | O_CREAT, 0644);
  if (fd1 < 0) {
    perror ("open dan");
    exit (EXIT_FAILURE);
  }
}

