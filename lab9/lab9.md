# Probleme

## 1. Problema rezervării biletelor

Se consideră 5 terminale de vânzare a 25 de bilete pentru un anumit scop (spectacol, călătorie). Se cere să se găsească o modalitate de a evita vânzarea mai multor bilete pentru acelaşi loc.
```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_SELLERS 5 //50
#define NUM_TICKETS 25 //250000
static int numTickets = NUM_TICKETS;

void *sellerThread(void* arg)
{
  int total = 0;
  long threadid = (long) arg;
  do {
    if(numTickets <= 0) break;
    numTickets--;
    total++;
    printf("Seller %ld sold one (%d left)\n", threadid, numTickets);
    //sleep(rand() % 5);
  } while(1);
  printf("Seller %ld finished! (I sold %d tickets)\n", threadid, total);
  pthread_exit((void*) total);
}

int main(void)
{
  long i;
  int total = 0;
  pthread_t tids[NUM_SELLERS];
  for(i=0; i < NUM_SELLERS; i++)
    pthread_create(&tids[i], NULL, sellerThread, (void*) i);
  for(i=0; i < NUM_SELLERS; i++) {
    int value;
    pthread_join(tids[i], (void**)&value);
    total +=value;
  }
  printf("Done %d!\n", total);
  return 0;
}
```
Obs: Pentru a rula programul şi a vedea că se vând mai multe bilete decât locuri se creşte numărul de terminale, numărul de bilete şi se comentează liniile cu printf().

```
$gcc rezervarebilete.c -o rezervarebilete -lpthread
$./rezervarebilete
Seller 2 sold one (24 left)
Seller 2 sold one (20 left)
Seller 2 sold one (19 left)
Seller 2 sold one (18 left)
Seller 2 sold one (17 left)
Seller 2 sold one (16 left)
Seller 2 sold one (15 left)
Seller 2 sold one (14 left)
Seller 2 sold one (13 left)
Seller 2 sold one (12 left)
Seller 2 sold one (11 left)
Seller 2 sold one (10 left)
Seller 2 sold one (9 left)
Seller 2 sold one (8 left)
Seller 2 sold one (7 left)
Seller 2 sold one (6 left)
Seller 2 sold one (5 left)
Seller 2 sold one (4 left)
Seller 2 sold one (3 left)
Seller 2 sold one (2 left)
Seller 2 sold one (1 left)
Seller 2 sold one (0 left)
Seller 2 finished! (I sold 22 tickets)
Seller 1 sold one (24 left)
Seller 1 finished! (I sold 1 tickets)
Seller 3 sold one (22 left)
Seller 3 finished! (I sold 1 tickets)
Seller 4 sold one (21 left)
Seller 4 finished! (I sold 1 tickets)
Seller 0 sold one (23 left)
Seller 0 finished! (I sold 1 tickets)
Done 26!
```
## 2. Problema producător-consumator
Fie un buffer la care în permanenţă au acces 2 entităţi: un producător, care adaugă elemente ori de câte ori bufferul permite acest lucru (nu se depăşeşte capacitatea), şi un consumator care extrage elemente ori de câte ori este posibil (până când va fi gol).

- Să se rezolve această problemă folosind semafoare.
- Să se creeze încă un producător sau încă un consumator.
- Să se rezolve această problemă folosind variabile de condiţie.

```
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_TOTAL_BUFFERS 5
#define DATA_LENGTH 20
char buffers[NUM_TOTAL_BUFFERS];

void *producer(void* x)
{
  int i, writePt = 0;
  char data;
  for(i=0; i < DATA_LENGTH; i++)
  {
    data = 'A' + rand() % 25;
    buffers[writePt] = data;
    printf("producer: buffer[%d]=%c\n", writePt, data);
    writePt = (writePt + 1) % NUM_TOTAL_BUFFERS;
  }
}

void* consumer(void* y)
{
  int i, readPt = 0;
  char data;
  for(i=0; i < DATA_LENGTH; i++)
  {
    data = buffers[readPt];
    printf("\t\tconsumer: buffer[%d]=%c\n", readPt, data);
    readPt = (readPt + 1) % NUM_TOTAL_BUFFERS;
  }
}

int main(void)
{
  pthread_t prod, cons;
  pthread_create(&prod, NULL, producer, NULL);
  pthread_create(&cons, NULL, consumer, NULL);
  pthread_join(prod, NULL);
  pthread_join(cons, NULL);
  printf("done!\n");
  return 0;
}
```

```
#gcc prodcons.c -o prodcons -lpthread
#./prodcons
producer: buffer[0]=I
producer: buffer[1]=L
producer: buffer[2]=C
producer: buffer[3]=P
producer: buffer[4]=S
producer: buffer[0]=K
producer: buffer[1]=L
producer: buffer[2]=R
producer: buffer[3]=Y
producer: buffer[4]=V
producer: buffer[0]=M
producer: buffer[1]=C
producer: buffer[2]=P
producer: buffer[3]=J
producer: buffer[4]=N
producer: buffer[0]=B
producer: buffer[1]=P
    consumer: buffer[0]=
    consumer: buffer[1]=P
    consumer: buffer[2]=B
    consumer: buffer[3]=J
    consumer: buffer[4]=N
    consumer: buffer[0]=B
    consumer: buffer[1]=P
    consumer: buffer[2]=B
    consumer: buffer[3]=J
    consumer: buffer[4]=N
    consumer: buffer[0]=B
    consumer: buffer[1]=P
    consumer: buffer[2]=B
    consumer: buffer[3]=J
    consumer: buffer[4]=N
    consumer: buffer[0]=B
    consumer: buffer[1]=P
    consumer: buffer[2]=B
    consumer: buffer[3]=J
    consumer: buffer[4]=N
producer: buffer[2]=B
producer: buffer[3]=W
producer: buffer[4]=L
done!
```
Aşa cum se observă din mesajele afişate, producătorul suprascrie date, consumatorul citeşte aceleaşi date de mai multe ori.

Rezolvare1:

Se folosesc 3 semafoare. Un semafor pe post de mutex pentru secţiunea critică (semafor binar, cu valoarea iniţială 1), semaforul EntriesFree, cu valoarea iniţială egală cu numărul de locaţii libere şi un semafor EntriesUsed, cu valoarea iniţială egală cu 0.

![alt text](https://github.com/rdemeter/so/blob/master/lab9/figs/prodcons.png)


## 3. Problema grădinii ornamentale

Intrarea in grădinile ornamentale ale unui oraş oriental se face prin N porţi. Să se ţină evidenţa persoanelor care au intrat în grădină.
