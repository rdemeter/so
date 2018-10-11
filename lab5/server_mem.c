/*Server pentru memoria partajata*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>

/*zona in care va fi mapata memoria*/
void *mem;
/*descriptorul zonei de memorie*/
int shm_fd;

int main(int argc, char **argv)
{
  shm_fd = shm_open("/MY_SHM3", O_CREAT | O_RDWR, 0644);
  if (shm_fd < 0) {
    perror("open():");
    exit(-1);
  }

  if (ftruncate(shm_fd, 100) < 0) {
    perror("truncate():");
    exit(-1);
  }
  mem = mmap(0, 100, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
  if (mem == MAP_FAILED) {
    perror("mmap():");
    exit(-1);
  }

  ((int*)mem)[0] = 1337;
  printf("Am scris: %d\n", ((int*)mem)[0]);
  if (munmap(mem, 100) < 0) {
    perror("munmap():");
    exit(-1);
  }

  if (close(shm_fd) < 0) {
    perror("close():");
    exit(-1);
  }

  sleep(10);
  if (shm_unlink("/MY_SHM2") < 0) {
    perror("unlink():");
    exit(-1);
  }
  return 0;
}
