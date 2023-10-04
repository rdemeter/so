
# IPC

- [IPC](#ipc)
  * [Pipe-uri](#pipe-uri)
    + [Pipe-uri anonime](#pipe-uri-anonime)
    + [Pipe-uri cu nume](#Pipe-uri-cu-nume)
  * [Semafoare](#semafoare)
    + [Crearea și deschiderea](#crearea-și-deschiderea)
    + [Decrementare, incrementare și aflarea valorii](#decrementare--incrementare-și-aflarea-valorii)
    + [Inchiderea și distrugerea](#inchiderea-și-distrugerea)
  * [Cozi de mesaje](#cozi-de-mesaje)
    + [Crearea și deschiderea](#crearea-și-deschiderea-1)
    + [Trimiterea și recepționarea de mesaje](#trimiterea-și-recepționarea-de-mesaje)
    + [Inchiderea și ștergerea](#inchiderea-și--tergerea)
  * [Memorie partajată](#memorie-partajată)
    + [Crearea și deschiderea](#crearea-și-deschiderea-2)
    + [Redimensionarea](#redimensionarea)
    + [Maparea și eliberarea](#maparea-și-eliberarea)
    + [Inchiderea și ștergerea](#inchiderea-și-ștergerea-1)
  * [Resurse utile](#resurse-utile)

Linux pune la dispoziție urmatoarele mecanisme de comunicare intre procese (IPC - Inter Process Communication):
- fisiere
- pipe-uri (anonime si cu nume, studiate in lucrarea anterioara)
- semafoare (semaphores) - realizează sincronizarea execuțiilor unor procese
- cozi de mesaje (message queues) - realizează schimbul de mesaje cu orice proces sau server
- memorie partajată (shared memory) - realizează partajarea memoriei între procese
- socket (care se vor studia la materia Retele de Calculatoare)

Obiectele de tip IPC pe care se concentrează laboratorul de față sunt gestionate global de sistem și rămân în viață chiar dacă procesul creator moare. Faptul că aceste resurse sunt globale în sistem are implicații contradictorii. Pe de o parte, dacă un proces se termină, datele plasate în obiecte IPC pot fi accesate ulterior de alte procese; pe de altă parte, procesul proprietar trebuie să se ocupe și de dealocarea resurselor, altfel ele rămân în sistem până la ștergerea lor manuală sau până la un reboot. Faptul că obiectele IPC sunt globale în sistem poate duce la apariția unor probleme: cum numărul de mesaje care se află în cozile de mesaje din sistem e limitat global, un proces care trimite multe asemenea mesaje poate bloca toate celelalte procese.

ATENTIE!!! Pentru folosirea API-ului trebuie să includeți la linking biblioteca 'rt' (-lrt).

# Pipe-uri

Pipe-urile (canalele de comunicaţie) sunt mecanisme primitive de comunicare între procese. Un pipe poate conţine o cantitate limitată de date. Accesul la aceste date este de tip FIFO (datele se scriu la un capăt al pipe-ului şi sunt citite de la celălalt capăt). Sistemul de operare garantează sincronizarea între operaţiile de citire şi scriere la cele două capete.

![image](https://github.com/rdemeter/so/blob/master/lab5/figs/pipe.png?raw=true)

Există două tipuri de pipe-uri:

• pipe-uri anonime - pot fi folosite doar de procese înrudite (un proces părinte şi un copil sau doi copii) deoarece este accesibil doar prin moştenire. Aceste pipe-uri nu mai există după ce procesele şi-au terminat execuţia.

• pipe-uri cu nume - există ca fişiere cu drepturi de acces. Aceasta înseamnă că ele vor exista în continuare independent de procesul care le creează şi pot fi folosite de procese neînrudite.

## Pipe-uri anonime

Pipe-ul este un mecanism de comunicare unidirecţională între două procese. În majoritatea implementărilor de UNIX un pipe apare ca o zonă de memorie de o anumită dimensiune în spaţiul nucleului. Procesele care comunică printr-un pipe trebuie să aibă un grad de rudenie; de obicei, un proces care creează un pipe va apela după aceea fork, iar pipe-ul se va folosi pentru comunicarea între părinte şi fiu. În orice caz procesele care comunică prin pipe nu pot fi create de utilizatori diferiţi ai sistemului.
 

Apelul de sistem pentru creare este pipe:
```
int pipe(int filedes[2]);
```
Parametrul filedes returnează 2 descriptori de fişier: filedes[0] deschis pentru citire şi filedes[1] deschis pentru scriere. Se consideră că ieşirea lui filedes[1] este intrare pentru filedes[0].
Apelul returnează 0 în caz de succes şi -1 în caz de eroare. Observaţii:

- Citirea/scrierea din/în pipe-uri este atomică dacă nu se citesc/scriu mai mult de PIPE_BUF octeţi.
- Citirea/scrierea din/în pipe-uri se realizează cu ajutorul funcţiilor read / write.

Majoritatea aplicaţiilor care folosesc pipe-uri închid în fiecare dintre procese capătul de pipe neutilizat în comunicarea unidirecţională. Dacă unul dintre descriptori este închis se aplică regulile:

- O citire dintr-un pipe pentru care descriptorul de scriere a fost închis, după ce toate datele au fost citite, va returna 0, ceea ce indică sfârşitul fişierului. Descriptorul de scriere poate fi duplicat astfel încât mai multe procese să poată scrie în pipe. De regulă, în cazul pipe-urilor anonime există doar două procese, unul care scrie şi altul care citeşte pe când în cazul fişierelor FIFO pot exista mai multe procese care scriu date.
- O scriere într-un pipe pentru care descriptorul de citire a fost închis cauzează generarea semnalului SIGPIPE. Dacă semnalul este captat şi se revine din rutina de tratare, funcţia de sistem write returnează eroare şi variabila errno are valoarea EPIPE.

Cea mai frecventă greşeală relativ la lucrul cu pipe-urile constă în faptul că nu se trimite EOF prin pipe (citirea din pipe nu se termină) decât dacă sunt închise TOATE capetele de scriere din TOATE procesele care au deschis descriptorul de scriere în pipe (în cazul unui fork, nu uitaţi să închideţi capetele pipe-ului în procesul părinte).

Alte funcţii utile: **popen**, **pclose**.

Exemplu 6. pipe.c
```c
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(void)
{
    int   fd[2], nbytes;
    pid_t childpid;
    char  string[] = "Hello, world!\n";
    char  readbuffer[80];

    pipe(fd);

    if((childpid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    if(childpid == 0) {
        printf("Child process closes up input side of pipe\n");
        close(fd[0]);

        printf("Child: Send string through the output side of pipe\n");
        write(fd[1], string, (strlen(string)+1));
        printf("Child: Exiting\n");
        exit(0);
    }
    else {
        printf("Parent process closes up output side of pipe\n");
        close(fd[1]);

        printf("Parent: Read in a string from the pipe\n");
        nbytes = read(fd[0], readbuffer, sizeof(readbuffer));
        printf("Parent: Received string: %s\n", readbuffer);
    }

    return(0);
}
```
```
$gcc pipe.c -o pipe
```
## Pipe-uri cu nume

**FIFO** numite şi pipe-uri cu nume, elimină necesitatea ca procesele care comunică să fie înrudite deoarece acestea nu trebuie să îşi transmită descriptorii. Astfel, fiecare proces îşi poate deschide pentru citire sau scriere fişierul pipe cu nume (FIFO) care este un tip de fişier special care păstrează caracteristicile unui pipe. Comunicaţia se face într-un sens sau în ambele sensuri. Fişierele de tip FIFO pot fi localizate ca având litera p în primul câmp al drepturilor de acces (ls -l) .
Apelul de sistem pentru crearea FIFO este:
```
int mkfifo(const char *pathname, mode_t mode);
```
Parametrii sunt:
- pathname - reprezintă numele de cale al fişierului FIFO.
- mode - reprezintă un întreg ce indică drepturile de acces ale fişierului FIFO.

Apelul returnează 0 în caz de succes si -1 în caz de eroare.

Observaţii: după ce FIFO a fost creat, acestuia i se pot aplica toate funcţiile pentru operaţii cu fişiere: open, close, read, write la fel ca şi altor fişiere.

Modul de comportare al unui FIFO este afectat de flagul O_NONBLOCK astfel:
- Dacă O_NONBLOCK nu este specificat (cazul normal), atunci un open  pentru citire se va bloca până când un alt proces deschide acelaşi FIFO pentru scriere. Analog, dacă deschiderea este pentru scriere, se poate produce blocare până când un alt proces efectuează deschiderea pentru citire.
- Dacă se specifică O_NONBLOCK, atunci deschiderea pentru citire revine imediat, dar o deschidere pentru scriere poate returna eroare cu errno având valoarea ENXIO, dacă nu există un alt proces care a deschis acelaşi FIFO pentru citire.
Atunci când ultimul proces care scrie într-un FIFO îl închide, se va genera un "sfârşit de fişier" pentru procesul care citeşte din FIFO.

Exemplu 7. fifoserver.c
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <linux/stat.h>

#define  FIFO_FILE  "myfifo"
#define  SZBUF  80
#define  QUIT  "bye"

int main(void) {
    FILE* fp;
    char readbuf[SZBUF];

    /*create the fifo*/
    umask(0);
    mknod(FIFO_FILE, S_IFIFO|0666, 0);

    printf("Running server... To quit, send via the client the msg 'bye'.\n");

    while(1) {
        fp = fopen(FIFO_FILE,"r");
        fgets(readbuf, SZBUF, fp);

        if (!strcmp(readbuf,QUIT)) break;

        printf("Received string: %s\n",readbuf);
        fclose(fp);
    }
    return 0;
}
```
```
$gcc fifoserver.c -o fifoserver
```
Exemplu 8. fifoclient.c
```c
#include <stdio.h>
#include <stdlib.h>

#define  FIFO_FILE  "myfifo"

int main(int argc, char* argv[]) {
    FILE* fp;

    if (argc != 2) {
        fprintf(stderr,"USAGE %s [string]\n",argv[0]);
        return 1;
    }
    if ( (fp = fopen(FIFO_FILE,"w")) == NULL ) {
        perror("fopen");
        return 1;
    }
    fprintf(fp,"%s",argv[1]);
    fclose(fp);
    return 0;
}
```
```
$gcc fifoclient.c -o fifoclient
```

## Semafoare

Semafoarele sunt resurse IPC folosite pentru sincronizarea între procese (e.g. pentru controlul accesului la resurse). Operațiile asupra unui semafor pot fi de setare sau verificare a valorii (care poate fi mai mare sau
egala cu 0) sau de test and set. Un semafor poate fi privit ca un contor ce poate fi incrementat și decrementat, dar a cărui valoare nu poate scadea sub 0.

Semafoarele POSIX sunt de 2 tipuri:
- cu nume, folosite în general pentru sincronizare între procese distincte;
- fără nume, ce pot fi folosite doar pentru sincronizarea între firele de execuție ale unui proces sau între procese înrudite.

In contiunare vor fi luate în discuție semafoarele cu nume. Diferențele față de cele bazate pe memorie constau în funcțiile de creare și distrugere, celelalte funcții fiind identice.
- ambele tipuri de semafoare sunt reprezentate în cod prin tipul sem_t.
- semafoarele cu nume sunt indenficate la nivel de sistem printr-un șir de forma "/nume".
- header-ele necesare sunt <fcntl.h>, <sys/types.h> și <semaphore.h>.

### Crearea și deschiderea

Un proces poate crea sau deschide un semafor existent cu funcția sem_open:
```
sem_t* sem_open(const char *name, int oflag);
sem_t* sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
```
Comportamentul este similar cu cel de la deschiderea fișierelor. Dacă flag-ul O_CREAT este prezent, trebuie folosită cea de-a doua formă a funcției, specificând permisiunile și valoarea inițială.

### Decrementare, incrementare și aflarea valorii

Un semafor este decrementat cu funcția sem_wait:
```
int sem_wait(sem_t *sem);
```
Dacă semaforul are valoarea zero, funcția blochează până când un alt proces "deblochează" (incrementează) semaforul.
Pentru a incerca decrementarea unui semafor fără riscul de a rămâne blocat la acesta, un proces poate apela sem_trywait:
```
int sem_trywait(sem_t *sem);
```
In cazul în care semaforul are deja valoarea zero, funcția va intoarce -1 iar errno va fi setat la EAGAIN. Un semafor este incrementat cu funcția sem_post:
```
int sem_post(sem_t *sem);
```
In cazul în care semaforul are valoarea zero, un proces blocat în sem_wait pe acesta va fi deblocat.
Valoarea unui semafor (a contorului) se poate afla cu sem_getvalue:
```
int sem_getvalue(sem_t *sem, int *pvalue);
```
In cazul în care există procese blocate la semafor, implementarea apelului pe Linux va returna zero în valoarea referită de pvalue.
Toate aceste funcții întorc zero în caz de succes.

### Inchiderea și distrugerea
Un proces închide (notifică faptul că nu mai folosește) un semafor printr-un apel sem_close:
```
int sem_close(sem_t *sem);
```
Un proces poate șterge un semafor printr-un apel sem_unlink:
```
int sem_unlink(const char *name);
```
Distrugerea efectivă a semaforului are loc după ce toate procesele care l-au deschis apelează sem_close sau se termină. Totuși, chiar și în acest caz, apelul sem_unlink nu va bloca!

Exemplu: Implementați două procese care să fie sincronizate folosind un semafor cu nume.
```c
// server_sem.c
#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#define SEM_NAME "/my_semaphore"

int main(void) {
  sem_t *my_sem;
  int rc, pvalue;
  /* create semaphore with initial value of 1 */
  my_sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
  if(my_sem == SEM_FAILED) {
    printf("sem_open failed\n");
    return 0;
  }
  /* get the semaphore */
  sem_wait(my_sem);
  /* do important stuff protected by the semaphore */
  rc = sem_getvalue(my_sem, &pvalue);
  printf("sem is %d\n", pvalue);
  getchar();
  /* release the lock */
  sem_post(my_sem);
  rc = sem_close(my_sem);
  if(rc == -1)
    printf("sem_close failed\n");
  rc = sem_unlink(SEM_NAME);
  if(rc == -1)
    printf("sem_unlink failed\n");
  return 0;
}
```
```
$gcc server_sem.c -o server_sem -lpthread
```
Cele două procese se rulează din console diferite. După rularea primului proces, semaforul va fi creat în ”/dev/shm” cu numele “sem.my_semaphore” și procesul va aștepta apăsarea unei taste pentru a debloca următorul proces.
```c
// client_sem.c
#include <fcntl.h> /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#define SEM_NAME "/my_semaphore"
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
  printf("Asteapta la semafor\n");
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
```
```
$gcc client_sem.c -o client_sem -lpthread
```
Exercitiu: Să se modifice aplicatia, astfel incat procesele să astepte după resursele partajate, asa cum este ilustrat in figura, producand interblocarea proceselor, deadlock.

![](https://github.com/rdemeter/so/blob/master/lab5/figs/deadlock.jpg)

## Cozi de mesaje

Acestea permit proceselor schimbarea de date între procese sub forma de mesaje.
- la nivel de sistem sunt indentificabile printr-un string de forma "/nume".
- la nivel codului, o coada de mesage este reprezentata de un descriptor de tipul mqd_t.
- header-ele necesare pentru lucrul cu aceste obiecte sunt <fcntl.h>, <sys/types.h> si <mqueue.h>.

### Crearea și deschiderea
Funcțiile de creare și deschidere sunt similare ca forma și semantică celor de la semafoare:
```
mqd_t mq_open(const char *name, int oflag);
mqd_t mq_open(const char *name, int oflag, mode_t mode, struct mq_attr *attr);
```
În funcție de flag-uri (unul din cele de mai jos trebuie specificat), coada poate fi deschisă pentru:
- recepționare (O_RDONLY)
- trimitere (O_WRONLY)
- recepționare și trimitere (O_RDWR)

Daca attr e NULL, coada va fi creată cu atribute implicite. Structura mq_attr arată astfel:
```
struct mq_attr {
  long mq_flags; /* 0 sau O_NONBLOCK */
  long mq_maxmsg; /* numar maxim de mesaje in coada */
  long mq_msgsize; /* dimensiunea maxima a unui mesaj */
  long mq_curmsgs; /* numar de mesaje in coada */
};
```
### Trimiterea și recepționarea de mesaje

Pentru a trimite un mesaj (de lungime cunoscută, stocat într-un buffer) în coadă se apelează mq_send:
```
mqd_t mq_send(mqd_t mqdes, const char *buffer, size_t length, unsigned priority);
```
Mesajele sunt ținute în coadă în ordine descrescătoare a priorității.
În cazul în care coada este plină, apelul blochează. Dacă este o coadă non-blocantă (O_NONBLOCK), funcția va întoarce -1 iar errno va fi setat la EAGAIN.
Pentru a primi un mesaj dintr-o coadă (și anume: cel mai vechi mesaj cu cea mai mare prioritate) se folosește mq_receive:
```
ssize_t mq_receive(mqd_t mqdes, char *buffer, size_t length, unsigned *priority);
```
Dacă priority este non-NULL, zona de memorie către care face referire va reține prioritatea mesajului extras.
În cazul în care coada este vidă, apelul blochează. Daca este o coadă non-blocantă (O_NONBLOCK), comportamentul este similar cu cel al mq_send.

ATENTIE!!! La primirea unui mesaj, lungimea buffer-ului trebuie să fie cel puțin egală cu dimensiunea maximă a mesajelor pentru coada respectivă, iar la trimitere cel mult egală. Dimensiunea maximă implicită se poate afla pe Linux din /proc/sys/kernel/msgmax.

### Inchiderea și ștergerea
Închiderea (eliberarea "referinței") unei cozi este posibilă prin apelul mq_close:
```
mqd_t mq_close(mqd_t mqdes);
```
Ștergerea se realizează cu un apel mq_unlink:
```
mqd_t mq_unlink(const char *name);
```
Semantica este similară cu cea de la semafoare: coada nu va fi ștearsă efectiv decât după ce restul proceselor implicate o închid.

Exemplu: Implementați un protocol simplu de comunicație între un client și un server folosind cozi de mesaje. Clientul se va conecta la server și va trimite acestuia numărul 1337 pe care server-ul îl va afișa. Apoi clientul va trimite server-ului un mesaj de închidere.
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <string.h>
#define QUEUE_NAME "/my_queue3"
#define NUMBER 1337
#define MAX_SIZE 1024
#define MSG_END "done"
/* descriptorul cozii de mesaje */
mqd_t q;

int main(int argc, char **argv) {
 char buf[MAX_SIZE];
 if ((q = mq_open(QUEUE_NAME, O_WRONLY)) < 0) {
   perror("mq_open():");
   exit(-1);
 }
 snprintf(buf, MAX_SIZE, "%d", 1337);
 if (mq_send(q, buf, strlen(buf), 0) < 0) {
   perror("mq_send():");
   exit(-1);
 }
 if (mq_send(q, MSG_END, strlen(MSG_END), 0) < 0) {
   perror("mq_send2():");
   exit(-1);
 }
 if (mq_close(q) < 0) {
   perror("mq_close():");
   exit(-1);
 }
 return 0;
}
```
```
$gcc -Wall client_mq.c -o client_mq -lrt
```
```c
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <mqueue.h>
#include <string.h>
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
  q = mq_open(QUEUE_NAME, O_CREAT | O_RDONLY, 0644, &attr);
  if (q < 0) {
    perror("mq_open():");
    exit(-1);
  }
  do {
    ssize_t bytes_read;
    if ((bytes_read = mq_receive(q, buf, MAX_SIZE + 1, NULL)) < 0) {
      perror("mq_receive():");
      exit(-1);
    }
    buf[bytes_read] = 0;
    if (!strcmp(buf, MSG_END)) done = 1;
    printf("Am primit de la client: %s (done = %d)\n", buf, done);
  } while (!done);

  if (mq_close(q) < 0) {
    perror("mq_close():");
   exit(-1);
  }
  if (mq_unlink(QUEUE_NAME) < 0) {
    perror("mq_unlink():");
   exit(-1);
  }
  return 0;
}
```
```
$gcc server_mq.c -o server_mq -lrt
```
## Memorie partajată

Acest mecanism permite comunicarea între procese prin accesul direct și partajat la o zonă de memorie bine determinată.
- la nivelul sistemului, o zonă este identificată printr-un string de forma "/nume";
- la nivelul codului, o zona este reprezentată printr-un file descriptor (int).
- header-ele necesare pentru lucrul cu aceste obiecte sunt <fcntl.h>, <sys/types.h> și <sys/mman.h>.

### Crearea și deschiderea

Apelul de creare/deschidere este similar ca semantica apelului open pentru fișiere "obișnuite":
```
int shm_open(const char *name, int flags, mode_t mode);
```
Ca flag de acces trebuie specificat fie O_RDONLY fie O_RDWR.

### Redimensionarea

O zonă de memorie partajată nou creată are dimensiunea inițială zero. Pentru a o dimensiona se folosește ftruncate:
```
int ftruncate(int fd, off_t length);
```

### Maparea și eliberarea
Pentru a putea utiliza o zona de memorie partajată după deschidere, aceasta trebuie mapată în spațiul de memorie al procesului. Aceasta se realizează printr-un apel mmap:
```
void *mmap(void *address, size_t length, int protection, int flags, int fd, off_t offset);
```
Valoarea intoarsă reprezintă un pointer către începutul zonei de memorie sau MAP_FAILED în caz de eșec.

Acest apel are o largă aplicabilitate și va fi discutat în cadrul laboratorului de memorie virtuală. Momentan, pentru a mapa întregul conținut al unei zone (shm_fd) de dimensiune cunoscută (shm_len), recomandăm
folosirea apelului
```
mem = mmap(0, shm_len, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
```
Când maparea nu mai este necesară, prin apelul munmap se realiează demaparea:
```
int munmap(void *address, size_t length);
```
### Inchiderea și ștergerea
Închiderea unei zone de memorie partajată este identică cu închiderea unui fișier: apelul close.

Odată ce o zonă de memoria a fost demapată și închisă în toate procesele implicate, se poate șterge prin shm_unlink:
```
int shm_unlink(const char *name);
```
Semantica este identică cu cea de la funcțiile *_unlink anterioare: ștergerea efectivă este amânată până ce toate procesele implicate închid zona în cauză.

Exemplu: Folosind memoria partajată, realizați un transfer simplu de informație între două procese astfel: server-ul va crea o zona de 4k de memorie și va pune numărul 1337 începând cu primul octet, clientul va citi și afișa acest număr.

Client pentru memoria partajată
```c
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
  shm_fd = shm_open("/MY_SHM2", O_RDWR, 0644);
  if (shm_fd < 0) {
    perror("shm_open():");
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
```
```
$gcc client_mem.c -o client_mem -lrt
```
Server pentru memoria partajata
```c
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
  shm_fd = shm_open("/MY_SHM2", O_CREAT | O_RDWR, 0644);
  if (shm_fd < 0) {
    perror("shm_open():");
    exit(-1);
  }
  if (ftruncate(shm_fd, 100) < 0) {
    perror("ftruncate():");
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
```
```
$gcc server_mem.c -o server_mem -lrt
```
## Resurse utile
• Fast User-level Locking In Linux
