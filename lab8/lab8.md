# Sincronizare thread-uri

- [Sincronizare thread-uri](#sincronizare-thread-uri)
- [Mutex](#mutex)
  * [Inițializarea/distrugerea unui mutex](#inițializarea-distrugerea-unui-mutex)
  * [Tipuri de mutex-uri](#tipuri-de-mutex-uri)
  * [Ocuparea/eliberearea unui mutex](#ocuparea-eliberearea-unui-mutex)
  * [Încercarea neblocantă de ocupare a unui mutex](#Încercarea-neblocantă-de-ocupare-a-unui-mutex)
  * [Exemplu de utilizare a mutex-urilor](#exemplu-de-utilizare-a-mutex-urilor)
- [Futexuri](#futexuri)
- [Semafoare](#semafoare)
  * [Operații pe semafoare](#operații-pe-semafoare)
- [Variabile de condiție](#variabile-de-condiție)
  * [Inițializarea/distrugerea unei variabile de condiție](#inițializarea-distrugerea-unei-variabile-de-condiție)
  * [Blocarea la o variabilă condiție](#blocarea-la-o-variabilă-condiție)
  * [Blocarea la o variabilă condiție cu timeout](#blocarea-la-o-variabilă-condiție-cu-timeout)
  * [Exemplu de utilizare a variabilelor de condiție](#exemplu-de-utilizare-a-variabilelor-de-condiție)
- [Bariere](#bariere)
  * [Inițializarea/distrugearea unei bariere](#inițializarea-distrugearea-unei-bariere)
  * [Așteptarea la o barieră](#așteptarea-la-o-barieră)

Pentru sincronizarea firelor de execuție avem la dispoziție următoarele mecanisme:
- mutex
- semafoare
- variabile de condiție
- bariere.

# Mutex
Mutex-urile sunt obiecte de sincronizare utilizate pentru a asigura accesul exclusiv la o secțiune de cod în care se accesează date partajate între două sau mai multe fire de execuție. Un mutex are două stări posibile: ocupat și liber. Un mutex poate fi ocupat de un singur fir de execuție la un moment dat.
Atunci când un mutex este ocupat de un fir de execuție, el nu mai poate fi ocupat de niciun altul. În acest caz, o cerere de ocupare venită din partea unui alt fir, în general va bloca firul până în momentul în care mutex-ul devine liber.

## Inițializarea/distrugerea unui mutex

Un mutex poate fi inițializat/distrus în mai multe moduri:
- folosind o macrodefiniție
```
// initializare statica a unui mutex cu atribute implicite
// NB: mutexul nu este eliberat, durata de viata a mutexului
// este durata de viata a programului.
pthread_mutex_t mutex_static = PTHREAD_MUTEX_INITIALIZER;
```
- inițializat cu atribute implicite
```
// semnaturile functiilor de initializare si distrugere de mutex:
int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
void initializare_mutex_cu_atribute_implicite()
{
  pthread_mutex_t mutex_implicit;
  pthread_mutex_init(&mutex_implicit, NULL); // atrr=NULL -> atribute implicite
  // ... folosirea mutexului ...
  // ???
  // eliberare mutex
  pthread_mutex_destroy(&mutex_implicit);
}
```
- inițializare cu atribute explicite
```
// NB: functia pthread_mutexattr_settype si macro-ul PTHREAD_MUTEX_RECURSIVE
// sunt disponibile doar daca se defineste _XOPEN_SOURCE la o valoare >= 500 *INAINTE*
// de a include <pthread.h>. Pentru mai multe detalii consultai feature_test_macros(7).
#define _XOPEN_SOURCE 500
#include <pthread.h>

void initializare_mutex_recursiv()
{
  // definim atribute, le initializam si marcam tipul ca fiind recursiv.
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  
  // definim un mutex recursiv, il initializam cu atributele definite anterior
  pthread_mutex_t mutex_recursiv;
  pthread_mutex_init(&mutex_recursiv, &attr);
  
  // eliberam resursele atributului dupa crearea mutexului
  pthread_mutexattr_destroy(&attr);
  
  // ... folosirea mutexului ...
  // ???
  
  // eliberare mutex
  pthread_mutex_destroy(&mutex_recursiv);
}
```
NB: Mutex-ul trebuie să fie liber pentru a putea fi distrus. În caz contrar funcția va întoarce codul de eroare EBUSY. Întoarcerea valorii 0 semnifică succesul apelului.

## Tipuri de mutex-uri

Folosind atributele de initializare se pot crea mutex-uri cu proprietăti speciale:
- activarea moștenirii de prioritate (priority inharitance) pentru a preveni inversiunea de prioritate (priority invesion). Există trei protocoale de moștenire a prioritătii:
  - PTHREAD_PRIO_NONE nu se moștenește prioritatea când deținem mutex-ul creat cu acest atribut
  - PTHREAD_PRIO_INHERIT dacă deținem un mutex creat cu acest atribut și dacă există fire de execuție blocate pe acel mutex se moștenește prioritatea firului de execuție cu cea mai mare prioritate
  - PTHREAD_PRIO_PROTECT dacă firul de execuție curent deține unul sau mai multe mutex-uri, acesta va executa la maximul priorităților specificată pentru toți mutecșii deținuți.

```
#define _XOPEN_SOURCE 500
#include <pthread.h>
int pthread_mutexattr_getprotocol(const pthread_mutexattr_t * attr, int * protocol);
int pthread_mutexattr_setprotocol(pthread_mutexattr_t *attr, int protocol);
```

- modul de comportare la preluări recursive ale mutex-ului
  - PTHREAD_MUTEX_NORMAL nu se fac verificări, preluarea recursivă duce la deadlock
  - PTHREAD_MUTEX_ERRORCHECK se fac verificări, preluarea recursivă duce la întoarcerea unei erori
  - PTHREAD_MUTEX_RECURSIVE mutex-urile pot fi preluate recursiv din același thread, și trebuie eliberate de același număr de ori.

```
#define _XOPEN_SOURCE 500
#include <pthread.h>
pthread_mutexattr_gettype(const pthread_mutexattr_t * attr, int * protocol);
pthread_mutexattr_settype(pthread_mutexattr_t * attr, int protocol);
```

## Ocuparea/eliberearea unui mutex
Funcțiile de ocupare blocantă/eliberare a unui mutex:
```
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
```
Dacă mutex-ul este liber în momentul apelului, acesta va fi ocupat de firul apelant și funcția va întoarce imediat.
Dacă mutex-ul este ocupat de un alt fir, apelul va bloca până la eliberarea mutex-ului. Dacă mutexul este deja
ocupat de firul de execuție curent (lock recursiv), comportamentul funcției este dictat de tipul mutex-ului:

|Tip mutex |Lock recursiv |Unlock |
| --- | --- | --- |
|PTHREAD_MUTEX_NORMAL |deadlock |eliberează mutexul |
|PTHREAD_MUTEX_ERRORCHECK |returnează eroare |eliberează mutexul|
|PTHREAD_MUTEX_RECURSIVE|incrementează contorul de ocupări |decrementează contorul de ocupări (la zero eliberează mutexul)|
|PTHREAD_MUTEX_DEFAULT |deadlock |eliberează mutexul| 

Nu este garantată o ordine FIFO de ocupare a unui mutex. Oricare din firele aflate în așteptare la deblocarea unui mutex pot să-l acapareze.

## Încercarea neblocantă de ocupare a unui mutex

Pentru a încerca ocuparea unui mutex fără a aștepta eliberarea acestuia în cazul în care este deja ocupat, se va apela funcția:
```
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int rc = pthread_mutex_trylock(&mutex);
if (rc == 0) {
  // mutexul era liber si l-am ocupat cu succes.
} else if (rc == EBUSY) {
  // mutexul este detinut de altcineva si nu l-am putut ocupa

  // dar în loc să mă blochez ca la un apel pthread_mutex_lock(&mutex)
  // am întors eroarea EBUSY.
} else {
  // a avut loc o altă eroare
}
```

## Exemplu de utilizare a mutex-urilor

Un exemplu de utilizare a unui mutex pentru a serializa accesul la variabilă globală global_counter:
```
#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 5

// mutex global
pthread_mutex_t mutex;

int global_counter = 0;

void *thread_routine(void *arg) {
  // preluam mutexul global
  pthread_mutex_lock(&mutex);
  
  // afisam si modificam valoarea variabilei globale 'global_counter'
  printf("Thread %d says global_counter=%d\n", (int) arg, global_counter);
  global_counter ++;
  
  // eliberam mutexul pentru a acorda acces altui fir de executie
  pthread_mutex_unlock(&mutex);
  return NULL;
}
int main(void) {
  int i;
  pthread_t tids[NUM_THREADS];
  
  // mutexul este initializat o singura data si folosit de toate firele de executie
  pthread_mutex_init(&mutex, NULL);
  
  // firele de executie vor executa codul functiei 'thread_routine'
  // in locul unui pointer la date utile, se trimite in ultimul argument
  // un intreg - identificatorul firului de executie
  for (i = 0; i < NUM_THREADS; i++)
    pthread_create(&tids[i], NULL, thread_routine, (void *) i);
  
  // asteptam ca toate firele de executie sa se termine
  for (i = 0; i < NUM_THREADS; i++)
    pthread_join(tids[i], NULL);

  // eliberam resursele mutexului
  pthread_mutex_destroy(&mutex);
  return 0;
}
```
```
$ gcc -Wall mutex.c -o mutex -lpthread
$ ./mutex
Thread 1 says global_counter=0
Thread 2 says global_counter=1
Thread 3 says global_counter=2
Thread 4 says global_counter=3
Thread 0 says global_counter=4
```

Exemplu: Detectarea race-urilor la modificarea variabilelor partajate de mai multe fire de executie.
```
#include <pthread.h>
#include <stdio.h>

#define INC_LIMIT 50000
#define THREADS_NO 100

int a = 0, b = 0;

void* single_increment(void* arg)
{
  int i;
  for(i = 0; i < INC_LIMIT; i++)
    a++;
  return NULL;
}

void* double_increment(void* arg)
{
  int i;
  for(i = 0; i < INC_LIMIT; i++) {
    b++;
    b++;
  }
  return NULL;
}

int main()
{
  int i;
  pthread_t sinc_threads[THREADS_NO];
  pthread_t dinc_threads[THREADS_NO];
  for(i = 0; i < THREADS_NO; i++) {
    pthread_create(&sinc_threads[i], NULL, single_increment, NULL);
    pthread_create(&dinc_threads[i], NULL, double_increment, NULL);
  }
  for(i = 0; i < THREADS_NO; i++) {
    pthread_join(sinc_threads[i], NULL);
    pthread_join(dinc_threads[i], NULL);
  }
  printf("Single Incremented variable: got %d expected %d\n", a, INC_LIMIT*THREADS_NO);
  printf("Double incremented variable: got %d expected %d\n", b, 2 * INC_LIMIT*THREADS_NO);
  return 0;
}
```
```
$ gcc race.c -o race -lpthread
$ ./race
Single Increment variable: got 4860254 expected 5000000
Double Increment variable: got 9479559 expected 10000000
```
Să se rezolve această problemă folosind două mutex-uri, astfel ca rezultatul obținut să fi cel așteptat.

```
$ gcc race.c -o race -lpthread
$ ./race
Single Increment variable: got 5000000 expected 5000000
Double Increment variable: got 10000000 expected 10000000
```

# Futexuri
Mutexurile din firele de execuție POSIX sunt implementate cu ajutorul futexurilor, din considerente de performanță. Numele de futex vine de la Fast User-space muTEX. Ideea de la care a plecat implementarea futexurilor a fost aceea de a optimiza operația de ocupare a unui mutex în cazul în care acesta nu este deja ocupat. Dacă mutexul nu este ocupat, el va fi ocupat fără ca procesul care îl ocupă să se blocheze. În acest caz, nefiind necesară blocarea, nu este necesar ca procesul să intre în kernel-mode (pentru a intra într-o stare de așteptare). Optimizarea constă în testarea și setarea atomică a valorii mutexului (printr-o instrucțiune de tip test-and-set-lock) în user-space, eliminându-se trap-ul în kernel în cazul în care nu este necesară blocarea.
Futexul poate fi orice variabilă dintr-o zonă de memorie partajată între mai multe fire de executie sau procese.
Asadar, operatiile efective cu futexurile se fac prin intermediul functiei do_futex, disponibilă prin includerea headerului linux/futex.h. Signatura ei arată astfel:
```
long do_futex(unsigned long uaddr, int op, int val, unsigned long timeout, unsigned long uaddr2, int val2);
```
În cazul în care este necesară blocarea, do_futex va face un apel de sistem - sys_futex. Futexurile pot fi utile (și poate fi necesară utilizarea lor explicită) în cazul sincronizării proceselor, alocate în variabile din zone de memorie partajată între procesele respective.

# Semafoare
Semafoarele sunt obiecte de sincronizare ce reprezintă o generalizare a mutexurilor prin aceea că salvează numărul de operații de eliberare (incrementare) efectuate asupra lor. Practic, un semafor reprezintă un întreg
care se incrementează/decrementează atomic. Valoarea unui semafor nu poate scădea sub 0. Dacă semaforul are valoarea 0, operația de decrementare se va bloca până când valoarea semaforului devine strict pozitivă. Mutexurile pot fi privite, așadar, ca niște semafoare binare.
Operațiile care pot fi efectuate asupra semafoarelor POSIX sunt:

```
- deschiderea unui semafor identificat prin nume, folosit pentru a sincroniza procese diferite
sem_t* sem_open(const char *name, int oflag);
sem_t* sem_open(const char, *name, int oflag, mode_t mode, unsigned int value);

- închiderea unui semafor cu nume
int sem_close(sem_t *sem);

- stergerea din sistem a unui semafor cu nume
int sem_unlink(const char *name);

- initializarea unui semafor fara nume
* sem - semaforul nou creat
* pshared - 0 daca semaforul nu este partajat decat de firele de executie ale procesului curent
          - non zero: semafor partajat cu alte procese in acest caz 'sem' trebuie alocat intr-o zona de memorie partajata
* value - valoarea initiala a semaforului

int sem_init(sem_t *sem, int pshared, unsigned int value);

- distrugerea unui semafor fara nume
int sem_destroy(sem_t *sem);
```

## Operații pe semafoare

```
// incrementarea (V)
int sem_post(sem_t *sem);

// decrementarea blocantă (P)
int sem_wait(sem_t *sem);

// decrementarea neblocantă
int sem_trywait(sem_t *sem);

// citirea valorii
int sem_getvalue(sem_t *sem, int *pvalue);
```

Semafoarele POSIX au fost prezentate în cadrul laboratorului de comunicare inter-proces.

Un exemplu de utilizare semafoare în limbajul Java este prezentat in video-ul următor:
[Coordinating Threads via Java Semaphore](https://www.youtube.com/watch?v=T3BDJS4JSr8)

În aplicație se creeaza două semafoare și două thread-uri care se vor executa în ordine ping-pong, ping-pong, ...

![Image](https://github.com/rdemeter/so/blob/5753ce3de97ff696da71f788379a0dec973d1bb8/lab8/figs/ping-pong.png?raw=true)

# Variabile de condiție

Variabilele condiție pun la dispoziție un sistem de notificare pentru fire de execuție, permițându-i unui fir să se blocheze în așteptarea unui semnal din partea unui alt fir. Folosirea corectă a variabilelor condiție presupune un protocol cooperativ între firele de execuție.

Mutexurile (mutual exclusion locks) și semafoarele permit blocarea altor fire de execuție. Variabilele de condiție se folosesc pentru a bloca firul curent de execuție până la îndeplinirea unei condiții.
Variabilele condiție sunt obiecte de sincronizare care-i permit unui fir de executie să-i suspende executia până când o condiție (predicat logic) devine adevărată. Când un fir de execuție determină că predicatul a devenit adevărat, va semnala variabila condiție, deblocând astfel unul sau toate firele de execuție blocate la acea variabilă condiție (în funcție de cum se dorește).

O variabilă condiție trebuie întotdeauna folosită împreună cu un mutex pentru evitarea race-ului care se produce când un fir se pregătește să aștepte la variabila condiție în urma evaluării predicatului logic, iar alt fir semnalizează variabila condiție chiar înainte ca primul fir să se blocheze, pierzându-se astfel semnalul. Așadar, operațiile de semnalizare, testare a condiției logice și blocare la variabila condiție trebuie efectuate având ocupat mutex-ul asociat variabilei condiție. Condiția logică este testată sub protecția mutex-ului, iar dacă nu este îndeplinită, firul apelant se blochează la variabila condiție, eliberând atomic mutex-ul. În momentul deblocării, un fir de execuție va încerca să ocupe mutex-ul asociat variabilei condiție. De asemenea, testarea predicatului logic trebuie făcută într-o buclă, pentru că dacă sunt eliberate mai multe fire deodată, doar unul va reuși să ocupe mutex-ul asociat condiției. Restul vor aștepta ca acesta să-l elibereze, însă este posibil ca firul care a ocupat mutexul să schimbe valoarea predicatului logic pe durata deținerii mutex-ului. Din acest motiv
celelalte fire trebuie să testeze din nou predicatul pentru că altfel i-ar începe execuția presupunând predicatul adevărat, când el este, de fapt, fals.

## Inițializarea/distrugerea unei variabile de condiție
```
// initializare statica a unei variabile de conditie cu atribute implicite
// NB: variabila de conditie nu este eliberata,
// durata de viata a variabilei de condiie este durata de viata a programului.
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
// semnaturile functiilor de initializare si eliberare de variabile de condiie:
int pthread_cond_init(pthread_cond_t *cond, pthread_condattr_t *attr);
int pthread_cond_destroy(pthread_cond_t *cond);
```
Ca și la mutex-uri:
- dacă parametrul attr este nul se folosesc atribute implicite
- trebuie să nu existe nici un fir de execuție în așteptare pe variabila de condiție atunci când aceasta este distrusă, altfel se întoarce EBUSY.

## Blocarea la o variabilă condiție
Pentru a-i suspenda execuția și a aștepta la o variabilă condiție, un fir de execuție va apela:
```
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);
```
Firul de execuție apelant trebuie să fi ocupat deja mutexul asociat, în momentul apelului. Funcția pthread_cond_wait va elibera mutexul și se va bloca, așteptând ca variabila condiție să fie semnalizată de un alt fir de execuție. Cele două operații sunt efectuate atomic. În momentul în care variabila condiție este semnalizată,
se va încerca ocuparea mutex-ului asociat, și după ocuparea acestuia, apelul funcției va întoarce. Observați că firul de execuție apelant poate fi suspendat, după deblocare, în așteptarea ocupării mutex-ului asociat, timp în care predicatul logic, adevărat în momentul deblocării firului, poate fi modificat de alte fire.
De aceea, apelul pthread_cond_wait trebuie efectuat într-o buclă în care se testează valoarea de adevăr a predicatului logic asociat variabilei condiție, pentru a asigura o serializare corectă a firelor de execuție. Un alt argument pentru testarea în buclă a predicatului logic este acela că un apel pthread_cond_wait poate fi întrerupt de un semnal asincron (vezi laboratorul de semnale), înainte ca predicatul logic să devină adevărat. Dacă firele de execuție care așteptau la variabila condiție nu ar testa din nou predicatul logic, i-ar continua execuția presupunând greșit că acesta e adevărat.

## Blocarea la o variabilă condiție cu timeout

Pentru a-i suspenda execuția și a aștepta la o variabilă condiție, nu mai târziu de un moment specificat de timp, un
fir de execuție va apela:
```
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex, const struct timespec *abstime);
```
Funcția se comportă la fel ca pthread_cond_wait, cu exceptia faptului că dacă variabila condiție nu este
semnalizată mai devreme de abstime, firul apelant este deblocat, și după ocuparea mutex-ului asociat, funcția se
întoarce cu eroarea ETIMEDOUT. Parametrul abstime este absolut și reprezintă numărul de secunde trecute de la
1 ianuarie 1970, ora 00:00.

Deblocarea unui singur fir blocat la o variabilă condiție
Pentru a debloca un singur fir de execuție blocat la o variabilă condiție se va semnaliza variabila condiție astfel:
```
int pthread_cond_signal(pthread_cond_t *cond);
```
Dacă la variabila condiție nu așteaptă niciun fir de executie, apelul funcției nu are efect și semnalizarea se va
pierde. Dacă la variabila condiție așteaptă mai multe fire de execuție, va fi deblocat doar unul dintre acestea.
Alegerea firului care va fi deblocat este făcută de planificatorul de fire de execuție. Nu se poate presupune că
firele care așteaptă vor fi deblocate în ordinea în care i-au început așteptarea. Firul de execuție apelant trebuie
să dețină mutexul asociat variabilei condiție în momentul apelului acestei funcții.
Exemplu. mutex-test.c
```
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>
pthread_mutex_t count_lock;
pthread_cond_t count_nonzero;
unsigned count;
void *decrement_count() {
  while(1)
  {
    pthread_mutex_lock(&count_lock);
    if (count == 0)
      pthread_cond_wait(&count_nonzero, &count_lock);
    count = count - 1;
    printf("dec count=%d\n", count);
    pthread_mutex_unlock(&count_lock);
  }
}
void *increment_count() {
  while(1)
  {
    pthread_mutex_lock(&count_lock);
    count = count + 1;
    printf("inc count=%d\n", count);
    if (count > 2) {
      printf("signal ...\n");
      pthread_cond_signal(&count_nonzero);
    }
    pthread_mutex_unlock(&count_lock);
  }
}
int main() {
 pthread_t dec, inc;
 pthread_create(&dec, NULL, decrement_count, NULL);
 pthread_create(&inc, NULL, increment_count, NULL);
 pthread_join(inc, NULL);
 pthread_join(dec, NULL);
 return 0;
}
```
```
$gcc mutex-test.c -o mutex-test -lpthread
$./mutex-test
inc count=1
inc count=2
inc count=3
signal ...
dec count=2
dec count=1
dec count=0
inc count=1
inc count=2
inc count=3
signal ...
inc count=4
inc count=5 …
```
Deci, după semnalarea variabilei de condiție nu se garantează execuția thread-ului care așteaptă la variabilă.
Deblocarea tuturor firelor blocate la o variabilă condiție Pentru a debloca toate firele de execuție blocate la o variabilă condiție, se semnalizează variabila condiție astfel:
```
int pthread_cond_broadcast(pthread_cond_t *cond);
```
Dacă la variabila condiție nu așteaptă niciun fir de execuție, apelul funcției nu are efect și semnalizarea se va pierde. Dacă la variabila condiție așteaptă fire de executie, toate acestea vor fi deblocate, dar vor concura pentru
ocuparea mutex-ului asociat variabilei condiție. Firul de execuție apelant trebuie să dețină mutex-ul asociat variabilei condiție în momentul apelului acestei funcții.

## Exemplu de utilizare a variabilelor de condiție
În următorul program se utilizează o barieră pentru a sincroniza firele de execuție ale programului.
Bariera este implementată cu ajutorului unei variabile de condiție.
```
#include <stdio.h>
#include <pthread.h>
#define NUM_THREADS 5
// implementarea unei bariere *nereentrante* cu variabile de conditie
struct my_barrier_t {
  // mutex folosit pentru a serializa accesele la datele interne ale barierei
  pthread_mutex_t lock;
  // variabila de conditie pe care se astepta sosirea tuturor firelor de executie
  pthread_cond_t cond;
  // numar de fire de executie care trebuie sa mai vina pentru a elibera bariera
  int nr_still_to_come;
};
struct my_barrier_t bar;

void my_barrier_init(struct my_barrier_t * bar, int nr_still_to_come)
{
  pthread_mutex_init(&bar->lock, NULL);
  pthread_cond_init(&bar->cond, NULL);
  // cate fire de executie sunt asteptate la bariera.
  bar->nr_still_to_come = nr_still_to_come;
}
void my_barrier_destroy(struct my_barrier_t * bar)
{
  pthread_cond_destroy(&bar->cond);
  pthread_mutex_destroy(&bar->lock);
}
void *thread_routine(void *arg)
{
  int thd_id = (int) arg;
  // inainte de a lucra cu datele interne ale barierei trebuie sa preluam mutexul
  pthread_mutex_lock(&bar.lock);
  printf("thd %d: before the barrier\n", thd_id);
  // suntem ultimul fir de executie care a sosit la bariera?
  int is_last_to_arrive = (bar.nr_still_to_come == 1);
  // decrementam numarul de fire de executie asteptate la bariera
  bar.nr_still_to_come --;
  // cat timp mai sunt threaduri care nu au ajuns la bariera, asteptam.
  while (bar.nr_still_to_come != 0)
    // lockul se elibereaza automat inainte de a incepe asteptarea
    pthread_cond_wait(&bar.cond, &bar.lock);
  
  // ultimul thread ajuns la bariera va semnaliza celelalte threaduri
  if (is_last_to_arrive) {
    printf(" let the flood in\n");
    pthread_cond_broadcast(&bar.cond);
  }
  printf("thd %d: after the barrier\n", thd_id);
  // la iesirea din functia de asteptare se preia automat mutexul, trebuie eliberat.
  pthread_mutex_unlock(&bar.lock);
  return NULL;
}
int main(void) {
  int i;
  pthread_t tids[NUM_THREADS];
  my_barrier_init(&bar, NUM_THREADS);
  for (i = 0; i < NUM_THREADS; i++)
    pthread_create(&tids[i], NULL, thread_routine, (void *) i);
  for (i = 0; i < NUM_THREADS; i++)
    pthread_join(tids[i], NULL);
  my_barrier_destroy(&bar);
  return 0;
}
```
```
$ gcc -Wall cond_var.c -o cond_var -lpthread
$ ./cond_var
thd 0: before the barrier
thd 2: before the barrier
thd 3: before the barrier
thd 4: before the barrier
thd 1: before the barrier
let the flood in
thd 1: after the barrier
thd 2: after the barrier
thd 3: after the barrier
thd 4: after the barrier
thd 0: after the barrier
```
Din execuția programului se observă:
- ordinea în care sunt planificate firele de execuție nu este identică cu cea a creării lor
- ordinea în care sunt trezite firele de execuție ce așteaptă la o variabilă de condiție nu este identică cu ordinea în care acestea au intrat în așteptare.

# Bariere
Standardul POSIX definește și un set de funcții și structuri de date de lucru cu bariere. Aceste funcții sunt disponibile dacă se definește macro-ul _XOPEN_SOURCE la o valoare >= 600.
Cu bariere POSIX, programul de mai sus poate fi simplificat:
```
#define _XOPEN_SOURCE 600
#include <pthread.h>
#include <stdio.h>
pthread_barrier_t barrier;
#define NUM_THREADS 5
void *thread_routine(void *arg)
{
  int thd_id = (int) arg;
  int rc;
  printf("thd %d: before teh barrier\n", thd_id);
  // toate firele de executie asteapta la bariera.
  rc = pthread_barrier_wait(&barrier);
  if (rc == PTHREAD_BARRIER_SERIAL_THREAD) {
    // un singur fir de execuie (posibil ultimul) va intoarce PTHREAD_BARRIER_SERIAL_THREAD
    // restul firelor de execuie întorc 0 în caz de succes.
    printf("thd %d let the flood in\n", thd_id);
  }
  printf("thd %d: after the barrier\n", thd_id);
  return NULL;
}

int main()
{
  int i;
  pthread_t tids[NUM_THREADS];
  // bariera este initializata o singura data si folosita de toate firele de executie
  pthread_barrier_init(&barrier, NULL, NUM_THREADS);
  // firele de executie vor executa codul functiei 'thread_routine'
  // in locul unui pointer la date utile, se trimite in ultimul argument
  // un intreg - identificatorul firului de executie
  for (i = 0; i < NUM_THREADS; i++)
    pthread_create(&tids[i], NULL, thread_routine, (void *) i);
  // asteptam ca toate firele de executie sa se termine
  for (i = 0; i < NUM_THREADS; i++)
    pthread_join(tids[i], NULL);
  // eliberam resursele barierei
  pthread_barrier_destroy(&barrier);
  return 0;
}
```
```
$ gcc -Wall barrier.c -o barrier -lpthread
$ ./barrier
thd 0: before the barrier
thd 2: before the barrier
thd 1: before the barrier
thd 3: before the barrier
thd 4: before the barrier
let the flood in
thd 4: after the barrier
thd 2: after the barrier
thd 3: after the barrier
thd 0: after the barrier
thd 1: after the barrier
```

## Inițializarea/distrugearea unei bariere
```
// pentru a folosi functiile de lucru cu bariere e nevoie să se definească
// _XOPEN_SOURCE la o valoare >= 600. Pentru detalii consultati feature_test_macros(7).
#define _XOPEN_SOURCE 600
#include <pthread.h>
// attr -> un set de atribute, poate fi NULL (se folosesc atribute implicite)
// count -> numărul de fire de executie care trebuie să ajungă
// la barieră pentru ca aceasta să fie eliberată
int pthread_barrier_init(pthread_barrier_t * barrier, const pthread_barrierattr_t * attr, unsigned count);
// trebuie să nu existe fire de executie în ateptare la barieră
// înainte de a apela functia _destroy,
// altfel, se întoarce EBUSY i nu se distruge bariera.
int pthread_barrier_destroy(pthread_barrier_t *barrier);
```
## Așteptarea la o barieră
```
#define _XOPEN_SOURCE 600
#include <pthread.h>
int pthread_barrier_wait(pthread_barrier_t *barrier);
```
Dacă bariera a fost creată cu count=N, primele N-1 fire de execuție care apelează pthread_barrier_wait se blochează. Când sosește ultimul (al N-lea), va debloca toate cele N-1 fire de execuție. Funcția pthread_barrier_wait întoarce trei valori:
- EINVAL în cazul în care bariera nu este inițializată (singura eroare definită)
- PTHREAD_BARRIER_SERIAL_THREAD în caz de succes, un singur fir de execuție va întoarce valoarea aceasta nu e specificat care este acel fir de execuție (nu e obligatoriu să fie ultimul ajuns la barieră)
- 0 valoare întoarsă în caz de succes de celelalte N-1 fire de execuție.

Exercițiu: Să se creeze un mutex normal, un thread în care se cheamă funcția pthread_mutex_lock() de două ori. Se va bloca programul, deadlock. Pentru rezolvarea problemei se va crea mutex-ul recursiv.
```
void *thread_routine( )
{
  pthread_mutex_lock(&lock);
  pthread_mutex_lock(&lock);
  
  printf("am ajuns in zona protejata de mutex\n");
  
  pthread_mutex_unlock(&lock);
  pthread_mutex_unlock(&lock);
}
```
Exercițiu: Să se implementeze un deadlock intre două thread-uri folosind doi mutex. Să se rezolve deadlockul folosind blocare conditionată pthread_mutex_trylock.

```
THREAD1
pthread_mutex_lock(&m1);
/* use resource 1 */
pthread_mutex_lock(&m2);
/* use resources 1 and 2 */
pthread_mutex_unlock(&m2);
pthread_mutex_unlock(&m1);
```
```
THREAD2
pthread_mutex_lock(&m2);
/* use resource 2 */
pthread_mutex_lock(&m1);
/* use resources 1 and 2 */
pthread_mutex_unlock(&m1);
pthread_mutex_unlock(&m2);
```
Rezolvare thread2:
```
for (; ;)
{
 pthread_mutex_lock(&m2);
 if(pthread_mutex_trylock(&m1)==0)
   /* got it */
   break;
 /* didn't get it */
 pthread_mutex_unlock(&m2);
}
/* get locks; no processing */
pthread_mutex_unlock(&m1);
pthread_mutex_unlock(&m2);
```
