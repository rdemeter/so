/*Client pentru memoria partajata*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

/* zona in care va fi mapata memoria */
void *mem;
/* descriptorul zonei de memorie */
int shm_fd;

int main(int argc, char **argv)
{
  shm_fd = shm_open("/MY_SHM3", O_RDWR, 0644);
  if (shm_fd < 0) {
    perror("open():");
    exit(-1);
  }

  mem = mmap(0, 100, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (mem == MAP_FAILED) {
    perror("mmap():");
    exit(-1);
  }

  printf("Am primit de la server: %d\n", ((int*)mem)[0]);
  if (munmap(mem, 100) < 0) {
    perror("munmap():");
    exit(-1);
  }

  if (close(shm_fd) < 0) {
    perror("close():");
    exit(-1);
  }
  return 0;
}

