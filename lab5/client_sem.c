#include <stdio.h>
#include <fcntl.h>	/* For O_* constants */
#include <sys/stat.h>	/* For mode constants */
#include <semaphore.h>

#define SEM_NAME 	"/my_semaphore"

int main(void)
{
  sem_t *my_sem;
  int rc, pvalue;

  /* create semaphore with initial value of 1 */
  my_sem = sem_open(SEM_NAME, O_RDWR);
  if(my_sem == SEM_FAILED) {
    printf("sem_open failed\n");
    return 0;
  }

  printf("Asteapta la semafor!\n");

  /* get the semaphore */
  sem_wait(my_sem);

  /* do important stuff protected by the semaphore */
  rc = sem_getvalue(my_sem, &pvalue);
  printf("sem is %d\n", pvalue);

  /* release the lock */
  sem_post(my_sem);

  rc = sem_close(my_sem);
  if(rc == -1)
    printf("sem_close failed\n");

  return 0;
}

