#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<mqueue.h>
#include<string.h>

#define QUEUE_NAME "/my_queue3"
#define NUMBER 1337
#define MAX_SIZE 1024
#define MSG_END "done"
/* descriptorul cozii de mesaje */
mqd_t q;
int main(int argc, char **argv)
{
  char buf[MAX_SIZE];
  if ((q = mq_open(QUEUE_NAME, O_WRONLY)) < 0)
  {
    perror("open():");
    exit(-1);
  }
  snprintf(buf, MAX_SIZE, "%d", 1337);
  if (mq_send(q, buf, strlen(buf), 0) < 0)
  {
    perror("send():");
    exit(-1);
  }
  if (mq_send(q, MSG_END, strlen(MSG_END), 0) < 0)
  {
    perror("send2():");
    exit(-1);
  }
  if (mq_close(q) < 0) 
  {
    perror("close():");
    exit(-1);
  }
  return 0;
}
