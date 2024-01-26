# Probleme

- [Probleme](#probleme)
  * [1. Problema rezervării biletelor](#1-problema-rezervării-biletelor)
  * [2. Problema producător-consumator](#2-problema-producător-consumator)
  * [3. Problema grădinii ornamentale](#3-problema-grădinii-ornamentale)
  * [4. Problema scriitor-cititor](#4-problema-scriitor-cititor)
  * [5. Problema H2O](#5-problema-H2O)

## 1. Problema rezervării biletelor

Se consideră 5 terminale de vânzare a 25 de bilete pentru un anumit scop (spectacol, călătorie). Se cere să se găsească o modalitate de a evita vânzarea mai multor bilete pentru acelaşi loc.
```c
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

```c
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

![Image](https://github.com/rdemeter/so/blob/master/lab9/figs/prodcons.png?raw=true)


## 3. Problema grădinii ornamentale

Intrarea în grădinile ornamentale ale unui oraş oriental se face prin N porţi. Să se ţină evidenţa persoanelor care au intrat în grădină.

## 4. Problema scriitor cititor

Considerăm o bază de date (un buffer) și multe procese care vor să acceseze această bază de date pentru scriere sau citire. Se permite ca mai multe procese să citească simultan dar, dacă unul din procese vrea să scrie (să modifice) baza de date, niciunul din celelalte procese nu au voie să acceseze baza de date (nici pentru scriere, nici pentru citire).
Se consideră următoarea secvență de program nesincronizat. Să se adauge elementele de sincronizare.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define DATA_LENGTH 20
#define NR_WRITER 5
#define NR_READER 10

char buffers[DATA_LENGTH];
void *writer(void* arg)
{
  int i, writePt = 0;
  char data;
  for(i=0; i < DATA_LENGTH; i++)
  {
    printf("writing#%ld\n", (long) arg);
    data = 'A' + rand() % 25;
    buffers[writePt] = data;
    sleep(rand() % 5);
    printf("write#%ld: buffer[%d]=%c\n", (long) arg, writePt, data);
    writePt = (writePt + 1);
  }
}
void* reader(void* arg)
{
  int i, readPt = 0;
  char data;
  for(i=0; i < DATA_LENGTH; i++)
  {
    printf("\t\treading#%ld\n", (long) arg);
    sleep(rand()%3);
    data = buffers[readPt];
    printf("\t\tread#%ld: buffer[%d]=%c\n", (long) arg, readPt, data);
    readPt = (readPt + 1);
  }
}

int main(void)
{
  long i;
  pthread_t wrt[NR_WRITER], rd[NR_READER];
  for(i = 0; i < NR_WRITER; i++)
    pthread_create(&wrt[i], NULL, writer, (void*) i);
  for(i = 0; i < NR_READER; i++)
    pthread_create(&rd[i], NULL, reader, (void*) i);

  for(i = 0; i < NR_WRITER; i++)
    pthread_join(wrt[i], NULL);
  for(i = 0; i < NR_READER; i++)
    pthread_join(rd[i], NULL);
  printf("done!\n");
  return 0;
}
```

## 5. Problema H2O

Thread-urile reprezintă atomi de hidrogen sau oxigen. O moleculă de apă se formează din doi atomi de hidrogen și unul de oxigen
- Dacă există doi atomi de hidrogen, va trebui să aștepte un atom de oxigen
- Dacă există un atom de oxigen, va trebui să aștepte doi atomi de hidrogen

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>

#define DATA_LENGTH 200

void *hydrogen(void* arg)
{
  int i;
  for(i=0; i < DATA_LENGTH; i++)
  {
    printf("H");
    sleep(rand() % 5);
  }
}

void* oxygen(void* arg)
{
  int i;
  for(i=0; i < DATA_LENGTH/2; i++)
  {
    printf("O");
    sleep(rand()%3);
  }
}

int main(void)
{
  pthread_t hyd, oxy;
  pthread_create(&hyd, NULL, hydrogen, NULL);
  pthread_create(&oxy, NULL, oxygen, NULL);

  pthread_join(hyd, NULL);
  pthread_join(oxy, NULL);
  return 0;
}
```
