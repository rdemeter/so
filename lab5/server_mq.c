#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<mqueue.h>
#include<string.h>

#define QUEUE_NAME "/my_queue3"
#define NUMBER 1337
#define MAX_SIZE 1024
#define MSG_END "done"
/*descriptorul cozii de mesaje*/
mqd_t q;
int main(int argc, char **argv)
{
  struct mq_attr attr;
  char buf[MAX_SIZE + 1];
  int done = 0;
  attr.mq_flags = 0; /* blocking */
  attr.mq_maxmsg = 10;
  attr.mq_msgsize = MAX_SIZE;
  attr.mq_curmsgs = 0;
  q = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, 		&attr);
  if (q < 0) {
    perror("open():");
    exit(-1);
  }
  do {
    ssize_t bytes_read;
    if ((bytes_read = mq_receive(q, buf, MAX_SIZE + 1, NULL)) < 0) {
      perror("receive():");
      exit(-1);
    }
    buf[bytes_read] = 0;
    if (!strcmp(buf, MSG_END)) done = 1;
    printf("Am primit de la client: %s (done = %d)\n", buf, done);
  } while (!done);

  if (mq_close(q) < 0) {
    perror("close():");
    exit(-1);
  }

  if (mq_unlink(QUEUE_NAME) < 0) {
    perror("unlink():");
    exit(-1);
  }
  return 0;
}

