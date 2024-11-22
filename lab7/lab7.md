# Thread-uri

- [Thread-uri](#thread-uri)
  * [Introducere](#introducere)
    + [Diferențe dintre thread-uri şi procese](#diferențe-dintre-thread-uri-şi-procese)
    + [Avantajele thread-urilor](#avantajele-thread-urilor)
- [Tipuri de thread-uri](#tipuri-de-thread-uri)
  * [Crearea firelor de execuţie](#crearea-firelor-de-execuţie)
  * [Așteptarea firelor de execuţie](#așteptarea-firelor-de-execuţie)
  * [Terminarea firelor de execuţie](#terminarea-firelor-de-execuţie)
  * [Atributele unui thread](#atributele-unui-thread)
- [Cedarea procesorului](#cedarea-procesorului)
- [Alte operaţii](#alte-operaţii)
- [Compilare](#compilare)
- [Exerciţii](#exerciţii)

## Introducere
În laboratoarele anterioare a fost prezentat conceptul de proces, acesta fiind unitatea elementară de alocare a resurselor utilizatorilor. În acest laborator este prezentat conceptul de fir de execuţie (sau thread), acesta fiind unitatea elementară de planificare într-un sistem. Ca și procesele, thread-urile reprezintă un mecanism prin care un calculator poate să ruleze mai multe lucruri simultan.

Un fir de execuţie există în cadrul unui proces, și reprezintă o unitate de execuţie mai fină decât acesta. În momentul în care un proces este creat, în cadrul lui există un singur fir de execuţie, care execută programul
secvenţial. Acest fir poate la rândul lui să creeze alte fire de execuţie; aceste fire vor rula porţiuni ale binarului asociat cu procesul curent, posibil aceleaşi cu firul iniţial (care le-a creat).

### Diferențe dintre thread-uri şi procese
- procesele nu partajează resurse între ele (decât dacă programatorul foloseşte un mecanism special pentru asta - vezi IPC), pe când thread-urile partajează în mod implicit majoritatea resurselor unui proces.
Modificarea unei astfel de resurse dintr-un fir este vizibilă instantaneu şi celorlalte:
  - segmentele de memorie precum .heap, .data şi .bss (deci şi variabilele stocate în ele)
  - descriptorii de fişiere (aşadar, închiderea unui fişier este vizibilă imediat pentru toate threadurile)
  - sockeţii
- fiecare fir are un context de execuţie propriu, format din
  - stivă
  - set de regiştri (deci şi un contor de program - registrul (E)IP)

Procesele sunt folosite de SO pentru a grupa şi aloca resurse, iar firele de execuţie pentru a planifica execuţia de cod care accesează (în mod partajat) aceste resurse.

### Avantajele thread-urilor
Deoarece thread-urile aceluiaşi proces folosesc tot spaţiul de adrese al procesului de care aparţin, folosirea lor
are o serie de avantaje:
- crearea/distrugerea unui thread durează mai puţin decât crearea/distrugerea unui proces
- timpul context switch-ului între thread-urile aceluiaşi proces este foarte mic, întrucât nu e necesar să se "comute" şi spaţiul de adrese (pentru mai multe informaţii, căutaţi "TLB flush")
- comunicarea între thread-uri are un overhead minim (practic se face prin modificarea unor zone de memorie din spaţiul de adresă)

Firele de execuţie se pot dovedi utile în multe situaţii, de exemplu, pentru a îmbunătăţi timpul de răspuns al aplicaţiilor cu interfeţe grafice (GUI), unde prelucrările CPU-intensive se fac de obicei într-un thread diferit de cel care afişează interfaţa.

De asemenea, ele simplifică structura unui program și conduc la utilizarea unui număr mai mic de resurse (pentru că nu mai este nevoie de diversele forme de IPC pentru a comunica).

# Tipuri de thread-uri
Există 3 categorii de thread-uri :
- Kernel Level Threads (KLT)
- User Level Threads (ULT)
- Fire de execuţie hibride

Kernel Level Threads
Managementul thread-urilor este făcut de kernel, şi programele user-space pot crea/distruge thread-uri printr-un set de apeluri de sistem. Kernel-ul menţine informaţii de context atât pentru procese cât si pentru thread-urile din cadrul proceselor, iar planificarea pentru execuţie se face la nivel de thread.

Avantaje :
- dacă avem mai multe procesoare putem lansa în execuţie simultană mai multe thread-uri ale aceluiasi proces; blocarea unui fir nu înseamnă blocarea întregului proces.
- putem scrie cod în kernel care să se bazeze pe thread-uri.

Dezavantaje :
- comutarea de context o face kernelul, deci pentru fiecare schimbare de context se trece din firul de execuţie în kernel și apoi se mai face încă o schimbare din kernel în alt fir de execuţie, deci viteza de comutare este mică.

User Level Threads
Kernel-ul nu este conștient de existenţa lor, şi managementul lor este făcut de procesul în care ele există, folosind de obicei o bibliotecă. Astfel, schimbarea contextului nu necesită intervenţia kernel-ului, iar algoritmul
de planificare depinde de aplicaţie.

Avantaje :
- schimbarea de context nu implică kernelul, deci avem o comutare rapidă
- planificarea poate fi aleasă de aplicaţie și deci se poate alege una care să favorizeze creşterea vitezei aplicaţiei noastre
- thread-urile pot rula pe orice SO, deci şi pe cele care nu suportă thread-uri (au nevoie doar de biblioteca ce le implementează)

Dezavantaje :
- kernel-ul nu știe de thread-uri, deci dacă un thread apelează ceva blocant toate thread-urile planificate de aplicaţie vor fi blocate. Cele mai multe apeluri de sistem sunt blocante
- kernel-ul planifică thread-urile de care ştie, fiecare pe un singur procesor la un moment dat. În cazul user-level threads, el va vedea un singur thread. Astfel, chiar dacă 2 thread-uri user-level sunt
implementate folosind un singur thread "văzut" de kernel, ele nu vor putea folosi eficient resursele sistemului (vor împărţi amândouă un acelaşi procesor).

Fire de execuţie hibride
Aceste fire încearcă să combine avantajele thread-urilor user-level cu cele ale thread-urilor kernel-level. O modalitate de a face acest lucru este de a utiliza fire kernel-level pe care să fie multiplexate fire user-level. KLT sunt unităţile elementare care pot fi distribuite pe procesoare. De regulă crearea thread-urilor se face în user space și tot aici se face aproape toată planificarea şi sincronizarea. Kernel-ul ştie doar de KLT-urile pe care sunt multiplexate ULT, şi doar pe acestea le planifică. Programatorul poate schimba eventual numărul de KLT alocate unui proces.

În ceea ce privește thread-urile, POSIX nu specifică dacă acestea trebuie implementate în user-space sau kernelspace. Linux le implementează în kernel-space, dar nu diferenţiază thread-urile de procese decât prin faptul că thread-urile partajează spaţiul de adresă (atât thread-urile, cât şi procesele, sunt un caz particular de "task"). Pentru folosirea thread-urilor în Linux trebuie să includem header-ul pthread.h unde se găsesc declaraţiile funcţiilor și tipurilor de date necesare şi să utilizăm biblioteca libpthread.

## Crearea firelor de execuţie
Pentru crearea unui nou fir de execuţie se foloseste funcţia pthread_create :
```c
#include <pthread.h>
int pthread_create(pthread_t *tid, const pthread_attr_t *tattr, void*(*start_routine)(void *), void *arg);
 ```
Noul fir creat se va executa concurent cu firul de execuţie din care a fost creat. Acesta va executa codul specificat de funcţia start_routine căreia i se va pasa argumentul arg. Folosind arg se poate transmite firului de execuţie un pointer la o structură care sa conţină toţi "parametrii" necesari acestuia.
Prin parametrul tattr se stabilesc atributele noului fir de execuţie. Dacă transmitem valoarea NULL thread-ul va fi creat cu atributele implicite.

## Așteptarea firelor de execuţie
La fel ca la procese, un părinte îi poate aștepta fiul apelând pthread_join (înlocuiește waitpid).
```c
int pthread_join(pthread_t th, void **thread_return);
```
Primul parametru specifică identificatorul firului de execuţie așteptat, iar al doilea parametru specifică unde se va plasa codul întors de funcţia copil (printr-un pthread_exit sau printr-un return). În caz de succes se întoarce valoarea 0, altfel se întoarce o valoare negativă reprezentând un cod de eroare.

Thread-urile se împart în două categorii :
- unificabile :
  - permit unificarea cu alte threaduri care apelează pthread_join.
  - resursele ocupate de thread nu sunt eliberate imediat după terminarea threadului, ci mai sunt păstrate până când un alt thread va executa pthread_join (analog cu procesele zombie)
  - threadurile sunt implicit unificabile
- detaşabile
  - un thread este detaşabil dacă :
    - a fost creat detaşabil.
    - i s-a schimbat acest atribut în timpul execuţiei prin apelul pthread_detach.
- nu se poate executa un pthread_join pe ele
- vor elibera resursele imediat ce se vor termina (analog cu ignorarea semnalului SIGCHLD în părinte atunci când se termină procesele copil)

## Terminarea firelor de execuţie
Un fir de execuţie se termină la un apel al funcţiei pthread_exit :
```c
void pthread_exit(void *retval);
```
Dacă nu există un astfel de apel este adăugat unul, în mod automat, la sfârșitul codului firului de execuţie.
Prin parametrul retval se comunică părintelui un mesaj despre modul de terminare al copilului. Această valoare va fi preluată de funcţia pthread_join.
Metodele ca un fir de execuţie să termine un alt thread sunt:
- stabilirea unui protocol de terminare (spre exemplu, firul master setează o variabilă globală, pe care firul slave o verifică periodic).
- mecanismul de "thread cancellation", pus la dispozitie de libpthread. Totuşi, această metodă nu este recomandată, pentru că este greoaie, şi pune probleme foarte delicate la clean-up.

```c
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
void* thread_function(void){
  char *a = malloc(12);
  strcpy(a, "hello world");
  pthread_exit((void*)a);
}
int main(){
  pthread_t thread_id;
  char *b;
  pthread_create (&thread_id, NULL, &thread_function, NULL);
  //here we are reciving one pointer value so to use that we need double pointer
  pthread_join(thread_id,(void**)&b);
  printf("b is %s\n",b);
  free(b); // lets free the memory
}
```
Exemplul de mai sus creează un fir de execuție și returnează un pointer cu **pthread_exit()**. În timp ce rulează thread-ul creat, thread-ul principal poate executa alte sarcini. Când se dorește utilizarea datelor generate de thread se apelează funcția **pthread_join()**.

## Atributele unui thread
Atributele reprezintă o modalitate de specificare a unui comportament diferit de comportamentul implicit.
Atunci când un fir de execuţie este creat cu pthread_create se poate specifica un atribut pentru
respectivul fir de execuţie. Atributele implicite sunt suficiente pentru marea majoritate a aplicaţiilor.
Cu ajutorul unui atribut se pot schimba:
- starea: unificabil sau detaşabil
- politica de alocare a procesorului pentru thread-ul respectiv (round robin, FIFO, sau system default)
- prioritatea (cele cu prioritate mai mare vor fi planificate, în medie, mai des)
- dimensiunea şi adresa de start a stivei

## Cedarea procesorului

Un thread cedează dreptul de execuție unui alt thread, în urma unuia din următoarele evenimente:
- efectuează un apel blocant (cerere de I/O, sincronizare cu un alt thread) şi kernel-ul decide că este rentabil să facă un context switch
- i-a expirat cuanta de timp alocată de către kernel
- cedează voluntar dreptul, folosind funcţia:
```
#include <sched.h>
int sched_yield(void);
```
Dacă există alte procese interesate de procesor acesta li se oferă, iar dacă nu există nici un alt proces în așteptare pentru procesor, firul curent îi continuă execuţia.

# Alte operaţii
Dacă dorim să fim siguri că un cod de iniţializare se execută o singură dată putem folosi funcţia :
```
pthread_once_t once_control = PTHREAD_ONCE_INIT;
int pthread_once(pthread_once_t *once_control, void (*init_routine) (void));
```
Scopul funcţiei pthread_once este de a asigura că o bucată de cod (de obicei folosită pentru iniţializări) se execute o singură dată. Argumentul once_control este un pointer la o variabilă iniţializată cu PTHREAD_ONCE_INIT. Prima oară când această funcţie este apelată ea va apela funcţia init_routine și va schimba valoarea variabilei once_control pentru a ţine minte că iniţializarea a avut loc. Următoarele apeluri ale acestei funcţii cu același once_control nu vor face nimic.

Funcţia pthread_once întoarce întotdeauna 0.
Pentru a determina identificatorul thread-ului curent se poate folosi funcţia :
```
pthread_t pthread_self(void);
```
Pentru a determina dacă doi identificatori se referă la același thread se poate folosi :
```
int pthread_equal(pthread_t thread1, pthread_t thread2);
```
Pentru aflarea/modificarea priorităţilor sunt disponibile următoarele apeluri:
```
int pthread_setschedparam(pthread_t target_thread, int policy, const struct sched_param *param);
int pthread_getschedparam(pthread_t target_thread, int *policy, struct sched_param *param);
```
# Compilare
La compilare trebuie specificată și biblioteca **libpthread** (deci se va folosi argumentul -lpthread).
**Atentie!** Nu link-aţi un program single-threaded cu această bibliotecă. Dacă faceţi așa ceva se vor stabili niște mecanisme multithreading care vor fi iniţializate la execuţie. Atunci programul va fi mult mai lent, va ocupa mult mai multe resurse și va fi mult mai dificil de depanat.

**Exemplu**
În continuare este prezentat un exemplu simplu în care sunt create 2 fire de execuţie, fiecare afișând un caracter de un anumit număr de ori pe ecran.
```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
/* structura ce contine parametrii transmisi fiecarui thread */
struct parametri {
  char caracter; /* caracterul afisat */
  int numar; /* de cate ori va fi afisat */
};
/* functia executata de thread-uri */
void* afisare_caracter(void *params)
{
  struct parametri* p = (struct parametri*) params;
  int i;
  for (i=0; i<p->numar; i++)
    printf("%c", p->caracter);
  printf("\n");
  return NULL;
}
int main()
{
  pthread_t fir1, fir2;
  struct parametri fir1_args, fir2_args;
  /* cream un thread care va afisa 'x' de 11 ori */
  fir1_args.caracter = 'x';
  fir1_args.numar = 11;
  if (pthread_create(&fir1, NULL, &afisare_caracter, &fir1_args)) {

    perror("pthread_create");
    exit(1);
  }
  /* cream un thread care va afisa 'y' de 13 ori */
  fir2_args.caracter = 'y';
  fir2_args.numar = 13;
  if (pthread_create(&fir2, NULL, &afisare_caracter, &fir2_args)) {
    perror("pthread_create");
    exit(1);
  }
  /* asteptam terminarea celor doua fire de executie */
  if (pthread_join(fir1, NULL))
    perror("pthread_join");
  if (pthread_join(fir2, NULL))
    perror("pthread_join");
  return 0;
}
```
Compilați, rulați aplicația și interpretați rezultatele. Comanda utilizată pentru a compila acest exemplu va fi:
```
$gcc -o exemplu exemplu.c -lpthread
$./exemplu
```
# Exerciţii
Pentru că nu aţi parcurs încă noţiunile necesare pentru a sincroniza thread-urile între ele, în cadrul acestui laborator vom folosi apeluri sleep() acolo unde e nevoie de sincronizare.

Realizaţi un program care creează 2 thread-uri. Thread-urile create vor partaja un descriptor de fisiere, modificat de către fiecare din ele, la momente diferite. Thread-urile să afișeze mesaje specifice la ieșirea standard. Explicați succesiunea mesajelor. Programul principal va aștepta încheierea execuției celor două thread-uri.
