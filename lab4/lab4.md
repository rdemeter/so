# Gestiunea memoriei
Subsistemul de gestiune a memoriei din cadrul unui sistem de operare este folosit de toate celelalte subsisteme: scheduling, I/O, filesystem, gestiunea proceselor, networking. Memoria este o resursă importantă și sunt necesari algoritmi eficienți de utilizare și gestiune a acesteia.
Rolul subsistemului de gestiune a memoriei este de a ține evidența zonelor de memorie fizică ocupate sau libere, de a oferi proceselor sau celorlalte subsisteme acces la memorie și de a mapa paginile de memorie virtuală ale unui proces (pages) peste paginile fizice (frames).

Nucleul sistemului de operare oferă un set de interfețe (apeluri de sistem) care permit alocarea/dezalocarea de memorie, maparea unor regiuni de memorie virtuală peste fișiere, partajarea zonelor de memorie.
Din păcate, nivelul limitat de înelegere a acestor interfețe și a acțiunilor ce se petrec în spate conduc la o serie de probleme foarte des întâlnite în aplicatiile software: memory leak-uri, accese invalide, suprascrieri, buffer overflow, corupere de zone de memorie.
Este, în consecintă, fundamentală cunoașterea contextului în care acționează subsistemul de gestiune a memoriei și înelegerea interfeței pusă la dispoziție de sistemul de operare programatorului.

## Spațiul de adrese al unui proces
Spațiul de adrese al unui proces, sau, mai bine spus, spațiul virtual de adresă al unui proces reprezintă zona de memorie virtuală utilizabilă de un proces. Fiecare proces are un spațiu de adresă propriu. Chiar în situațiile în care două procese partajează o zonă de memorie, spațiul virtual este distinct, dar se mapează peste aceeași zonă de memorie fizică.

![image](https://github.com/rdemeter/so/blob/master/lab4/figs/process_address_space.jpg)

În figura alăturată este prezentat un spațiu de adresă tipic pentru un proces. În sistemele de operare moderne, în spațiul virtual al fiecărui proces se mapează memoria nucleului, aceasta poate fi mapată fie la începutul fie la sfârsitul spațiului de adresă. (Note). În continuare ne vom referi numai la spațiul de adresă din user-space pentru un proces.
Cele 4 zone importante din spațiul de adresă al unui proces sunt zona de date, zona de cod, stiva și heap-ul. După cum se observă și din figură, stiva și heap-ul sunt zonele care pot creşte. De fapt, aceste două zone sunt dinamice şi au sens doar în contextul unui proces. De partea cealaltă,
informaţiile din zona de date şi din zona de cod sunt descrise în executabil.

## Zona de cod
Zona/segmentul de cod (denumit şi 'text segment') reprezintă instrucţiunile programului.
Registrul de tip 'instruction pointer' va referi adrese din zona de cod. Se citeşte instrucţiunea indicată, se decodifică şi se interpretează, după care se incrementează contorul programului şi se trece la următoarea instrucţiune. Zona de cod este, de obicei, o zonă read-only.
## Zone de date
Zonele de date conţin variabilele globale definite într-un program şi variabilele de tipul read-only. În funcţie de tipul de date există mai multe sub-tipuri de zone de date.
## .data
Zona .data conţine variabilele globale iniţializate la valori nenule ale unui program. De exemplu:
```
static int a = 3;
char b = 'a';
```
## .bss
Zona .bss conţine variabilele globale neiniţializate sau iniţializate la zero ale unui program. De exemplu:
```
static int a;
char b;
```
În general acestea nu vor fi prealocate în executabil ci în momentul creării precesului. Alocare zonei .bss se face peste pagini fizice zero (zeroed frames).
## .rodata
Zona .rodata conţine informaţie care poate fi doar citită, nu şi modificată. Aici sunt stocate constantele:
```
const int a;
const char *ptr;
şi literalii:
"Hello, World!" "En Taro Adun!"
```
## Stiva
Stiva este o regiune dinamică în cadrul unui proces. Stiva este folosită pentru a reţine "stack frame-urile" (link) în cazul apelurilor de funcţii şi pentru a stoca variabilele locale. Pe marea majoritate a arhitecturilor moderne stiva creşte în jos şi heap-ul creşte în sus. Stiva este gestionată automat de compilator. La fiecare revenire din funcţie stiva este golită.
În figura de mai jos este prezentată o vedere conceptuală asupra stivei in momentul apelului unei funcţii.

![image](https://github.com/rdemeter/so/blob/master/lab4/figs/call_stack.png)

## Heap-ul
Heap-ul este zona de memorie dedicată alocării dinamice a memoriei. Heap-ul este folosit pentru7 alocarea de regiuni de memorie a căror dimensiune se află doar la runtime.
La fel ca şi stiva, heap-ul este o regiune dinamică şi care îi modifică dimensiunea. Spre deosebire de stivă, însă, heap-ul nu este gestionat de compilator. Este de datoria programatorului să ştie câtă memorie trebuie să aloce şi să reţină cât a alocat şi când trebuie să dezaloce. Problemele frecvente în majoritatea programelor țin de pierderea referinţelor la zonele alocate (memory leaks) sau referirea de zone nealocate sau insuficient alocate (accese invalide).
La limbaje precum Java, Lisp, etc. unde nu există "pointer freedom", eliberarea spaţiului alocat se face automat prin intermediul unui garbage collector (link). Pe aceste sisteme se previne problema pierderii referinţelor, dar încă rămâne activă problema referirii zonelor nealocate.

## Alocarea memoriei
Alocarea memoriei este realizată static de compilator sau dinamic, în timpul execuţiei. Alocarea statică este realizată în segmentele de date pentru variabilele locale sau pentru literali.
În timpul execuţiei, variabilele se alocă pe stivă sau în heap. Alocarea pe stivă se realizează automat de compilator pentru variabilele locale unei funcții (mai puțin variabilele locale prefixate de identificatorul static).

Alocarea dinamică se realizează în heap. Alocarea dinamică are loc atunci când nu se ştie în momentul compilării câtă memorie va fi necesară pentru o variabilă, o structură, un vector. Dacă se ştie din momentul compilării cât spaţiu va ocupa o varibilă, se recomandă alocarea ei statică, pentru
a preveni erorile frecvente apărute în contextul alocării dinamice.
Pentru a fragmenta cât mai puţin spațiul de adrese al procesului, ca urmare a alocărilor şi dezalocărilor unor zone de dimensiuni variate, alocatorul de memorie va organiza segmentul de date alocate dinamic sub formă de heap, de unde şi numele segmentului.

## Alocarea memoriei în Linux
În Linux alocarea memoriei pentru procesele utilizator se realizează prin intermediul funcţiilor de bibliotecă malloc, calloc şi realloc iar dezalocarea ei prin intermediul funcţiei free.
Aceste funcţii reprezintă apeluri de bibliotecă şi rezolvă cererile de alocare şi dezalocare de memorie pe cât posibil în user space. Aşadar, se ţin nişte tabele care specifică zonele de memorie alocate în heap. Dacă există zone libere pe heap, un apel malloc care cere o zonă de memorie care poate fi încadrată într-o zonă liberă din heap va fi satisfăcut imediat marcând în tabel zona respectivă ca fiind alocată şi întorcând programului apelant un pointer spre ea.
Dacă în schimb se cere o zonă care nu încape în nicio zonă liberă din heap, malloc va încerca extinderea heap-ului prin apelul de sistem brk sau mmap.
```
void *calloc(size_t nmemb, size_t size);
void *malloc(size_t size);
void *realloc(void *ptr, size_t size);
void free(void *ptr);
```
Întotdeauna eliberaţi (free) memoria alocată. Memoria alocată de proces este eliberată automat la terminarea procesului însă în cazul unui proces server, de exemplu, care rulează foarte mult timp şi nu eliberează memoria alocată acesta va ajunge să ocupe toată memoria disponibilă în sistem cauzând astfel consecințe nefaste. Atenţie să nu eliberaţi de două ori aceeaşi zonă de memorie întrucât acest lucru va avea drept urmare coruperea tabelelor ţinute de malloc ceea ce va duce din nou la consecinţe nefaste. Întrucât funcția free se întoarce imediat dacă primeşte ca parametru un pointer NULL, este recomandat ca după un apel free, pointer-ul să fie resetat la NULL.
Câteva exemple de alocare a memoriei sunt prezentate în continuare:
```
int n = atoi(argv[1]);
char *str;
/* de obicei malloc-ul primeşte argumentul de spaţiu în forma size_elems * num_elems */
str = (char *) malloc((n + 1) * sizeof(char));
if (NULL == str)
{ 
  perror("malloc");
  exit(EXIT_FAILURE);
}
[...]
free(str);
str = NULL;
---
/* crearea unui vector de siruri ce contine doar argumentele unui program */
char **argv_no_exec;
/* se aloca spatiu pentru argumente */
argv_no_exec = (char **) malloc((argc - 1) * sizeof(char*));
if (NULL == argv_no_exec)
{ 
  perror("malloc");
  exit(EXIT_FAILURE);
}
/* se creeaza referinte catre argumente */
for (i = 1; i < argc; i++)
  argv_no_exec[i-1] = argv[i];
[...]

free(argv_no_exec);
argv_no_exec = NULL;
```
Apelul realloc este folosit pentru modificarea spatiului de memorie alocat dupa un apel malloc:
```
int *p;
p = (int *) malloc(n * sizeof(int));
if (NULL == p) { 
  perror("malloc");
  exit(EXIT_FAILURE);
}
[...]
p = (int *) realloc(p, (n + extra) * sizeof(int));
[...]
free(p);
p = NULL;
```
Apelul calloc este folosit pentru alocarea de zone de memorie al căror conţinut este nul (plin de valori de zero). Spre deosebire de malloc, apelul va primi două argumente: numărul de elemente şi dimensiunea unui element.
```
list_t *list_v; /* list_t poate fi orice tip de date din C (mai putin void) */
list_v = (list_t *) calloc(n, sizeof(list_t));
if (NULL == list_v) { 
  perror("calloc");
  exit(EXIT_FAILURE);
}
[...]
free(p);
p = NULL;
```
Mai multe informaţii găsiţi în manualul bibliotecii standard C şi în pagina de manual man malloc.

## Probleme de lucru cu memoria
Lucrul cu heap-ul este una dintre cauzele principale ale apariţiilor problemelor de programare.
Lucrul cu pointerii, necesitatea folosirii unor apeluri de sistem/bibliotecă pentru alocare/dezalocare, pot conduce la o serie de probleme care afectează (de multe ori fatal) funcţionarea unui program.
Problemele cele mai des întâlnite în lucrul cu memoria sunt:
- accesul invalid la memorie
- leak-urile de memorie

Accesul invalid la memorie presupune accesarea unor zone care nu au fost alocate sau au fost eliberate.
Leak-urile de memorie sunt situaiile în care se pierde referinţa la o zonă alocată anterior. Acea zonă va rămâne ocupată până la încheierea procesului. Ambele probleme şi utilitarele care pot fi folosite pentru combaterea acestora vor fi prezentate în continuare.

## Acces invalid
De obicei, accesarea unei zone de memorie invalide rezultă într-o eroare de pagină (page fault) şi terminarea procesului (în Unix înseamnă trimiterea semnalului SIGSEGV - afişarea mesajului 'Segmentation fault'). Totuşi, dacă eroarea apare la o adresă invalidă dar într-o pagină validă, hardwareul şi sistemul de operare nu vor putea sesiza acţiunea ca fiind invalidă. Acest lucru se datorează faptului că alocarea memoriei se face la nivel de
pagină. Pot exista situaţii în care să fie folosită doar jumătate din pagină. Deşi cealaltă jumătate conţine adrese invalide, sistemul de operare nu va putea detecta accesele invalide la acea zonă.
Asemenea accese pot duce la coruperea heap-ului şi la pierderea consistenţei memoriei alocate. După cum se va vedea în continuare, există utilitare care ajută la detectarea acestor situaţii.

Un tip special de acces invalid este buffer overflow. Acest tip de atac presupune referirea unor regiuni valide din spaţiul de adresă al unui proces prin intermediul unei variabile care nu ar trebui să poată referenţia aceste adrese. De obicei, un atac de tip buffer overflow rezultă în rularea de cod nesigur. Protecţia la accese de tip buffer overflow se realizează prin verificarea limitelor unui buffer/vector fie la compilare, fie la rulare.

## GDB - Detectarea zonei de acces invalid de tip page fault
Pe lângă facilităţi de bază precum urmărirea unei variabile sau configurarea de puncte de oprire (breakpoints), GDB pune la dispoziţia utilizatorilor şi comenzi avansate, utile în anumite cazuri. Comanda disassamble poate fi folosită pentru a afișa codul maşină generat de compilator. Comanda **info reg** afişează conţinutul registrelor. Aceste comenzi sunt folosite rar, atunci când utilizatorul încearcă să depaneze codul generat de compilator, sau când are părţi din program scrise direct în asamblare.
O comandă foarte utilă atunci când se depanează programe complexe este **backtrace**. Această comandă afişează toate apelurile de funcţii în curs de execuţie.
Exemplu: fibonacci_test.c
```
#include <stdio.h>
#include <stdlib.h>
int fibonacci(int no)
{
  if (1 == no || 2 == no)
    return 1;
  return fibonacci(no-1) + fibonacci(no-2);
}
int main()
{
  short int numar, baza=10;
  char sir[1];

  scanf("%s", sir);
  numar=strtol(sir, NULL, baza);
  printf("fibonacci(%d)=%d\n", numar, fibonacci(numar));
  return 0;
}
```
Pentru exemplul de mai sus, vom demonstra utilitatea comenzii backtrace:
```
$ gcc -Wall fibonacci_test.c -g
$ gdb a.out
(gdb) break 8
Breakpoint 1 at 0x8048482: file fibonacci_test.c, line 8.
(gdb) run
Starting program: /home/a.out
7
Breakpoint 1, fibonacci (no=2) at fibonacci_test.c:8
8 return 1;
(gdb) bt
#0 fibonacci (no=2) at fibonacci_test.c:8
#1 0x0804849d in fibonacci (no=3) at fibonacci_test.c:9
#2 0x0804849d in fibonacci (no=4) at fibonacci_test.c:9
#3 0x0804849d in fibonacci (no=5) at fibonacci_test.c:9
#4 0x0804849d in fibonacci (no=6) at fibonacci_test.c:9
#5 0x0804849d in fibonacci (no=7) at fibonacci_test.c:9
#6 0x0804851c in main () at fibonacci_test.c:20
#7 0x4003d280 in libc_start_main () from lib/libc.so.6
(gdb)
```
Se observă că la afişarea apelurilor de funcţii se afişează şi parametrii cu care a fost apelată funcţia. Acest lucru este posibil datorită faptului că atât variabilele locale cât şi parametrii acesteia sunt păstraţi pe stivă până la ieşirea din funcţie.

Fiecare funcţie are alocată pe stivă un frame, în care sunt plasate variabilele locale funcţiei, parametrii pasaţi funcţiei şi adresa de revenire din functie. În momentul în care o funcţie este apelată, se creează un nou frame prin alocarea de spaţiu pe stivă de către funcţia apelată. Astfel, dacă avem apeluri de
funcţii imbricate, atunci stiva va conţine toate frame-urile tuturor funcţiilor apelate imbricat.
GDB dă posibilitatea utilizatorului să examineze frame-urile prezente în stivă. Astfel, utilizatorul poate alege oricare din frame-urile prezente folosind comanda frame. După cum s-a observat, exemplul anterior are un bug ce se manifestă atunci când numărul introdus de la tastatură depăşeşte dimensiunea buffer-ului alocat (static). Acest tip de eroare poartă denumirea de buffer overflow şi este extrem de gravă. Cele mai multe atacuri de la distanţă pe un sistem sunt cauzate de acest tip de erori. Din păcate, acest tip de eroare nu este uşor de detectat, pentru că în procesul de buffer overrun se pot suprascrie alte variabile, ceea ce duce la detectarea erorii nu imediat când s-a făcut suprascrierea, ci mai târziu, când se va folosi variabila afectat.
```
$ gdb a.out
(gdb) run
Starting program: /home/a.out
10
Program received signal SIGSEGV, Segmentation fault.
0x08048497 in fibonacci (no=-299522) at fibonacci_test.c:9
9 return fibonacci(no-1) + fibonacci(no-2);
(gdb) bt -5
#299520 0x0804849d in fibonacci (no=-2) at fibonacci_test.c:9
#299521 0x0804849d in fibonacci (no=-1) at fibonacci_test.c:9
#299522 0x0804849d in fibonacci (no=0) at fibonacci_test.c:9
#299523 0x0804851c in main () at fibonacci_test.c:20
#299524 0x4003e280 in libc_start_main () from /lib/libc.so.6
```
Din analiza de mai sus se observă că funcţia fibonacci a fost apelată cu valoarea 0. Cum funcţia nu testează ca parametrul să fie valid, se va apela recursiv de un număr suficient de ori pentru a cauza umplerea stivei programului. Se pune problema cum s-a apelat funcţia cu valoarea 0, când trebuia apelată cu valoarea 10.
```
$ gdb a.out
(gdb) run
Starting program: /home/a.out
Program received signal SIGSEGV, Segmentation fault.
0x08048497 in fibonacci (no=-299515) at fibonacci_test.c:9
9 return fibonacci(no-1) + fibonacci(no-2);
(gdb) bt -2
#299516 0x0804851c in main () at fibonacci_test.c:20
#299517 0x4003d280 in libc_start_main () from /lib/libc.so.6
(gdb) fr 299516
#299516 0x0804851c in main () at fibonacci_test.c:20
20 printf("fibonacci(%d)=%d\n", numar, fibonacci(numar));
(gdb) print numar
$1 = 0
(gdb) print baza
$2 = 48
(gdb)
```
Se observă că problema este cauzată de faptul că variabila baza a fost alterată. Pentru a determina când sa întâmplat acest lucru, se poate folosi comanda watch. Această comandă primește ca parametru o expresie şi va opri execuţia programului de fiecare dată când valoarea expresiei se schimbă.
(gdb) quit
```
$ gdb a.out
(gdb) break main
Breakpoint 1 at 0x80484d6: file fibonacci_test.c, line 15.
(gdb) run
Starting program: /home/a.out
Breakpoint 1, main () at fibonacci_test.c:15
15 short int numar, baza=10;
(gdb) n
18 scanf("%s", sir);
(gdb) watch baza
Hardware watchpoint 2: baza
(gdb) continue
Continuing.

Hardware watchpoint 2: baza
Old value = 10
New value = 48
0x40086b41 in _IO_vfscanf () from /lib/libc.so.6
(gdb) bt
#0 0x40086b41 in _IO_vfscanf () from /lib/libc.so.6
#1 0x40087259 in scanf () from /lib/libc.so.6
#2 0x080484ed in main () at fibonacci_test.c:18
#3 0x4003d280 in libc_start_main () from /lib/libc.so.6 (gdb)
```
Din analiza de mai sus se observă că valoarea variabilei este modificată în funcţia \_IO_vfscanf, care la rândul ei este apelată de către functia scanf. Dacă se analizează apoi parametrii pasaţi funcției scanf se observă imediat cauza erorii.
Pentru mai multe informaţii despre GDB consultaţi manualul online (alternativ pagina info - info gdb) sau folosiţi comanda help din cadrul GDB.
mcheck - verificarea consistenţei heap-ului
glibc permite verificarea consistenţei heap-ului prin intermediul apelului mcheck definit în mcheck.h.
Apelul mcheck forţează malloc să execute diverse verificări de consistenţă precum scrierea peste un bloc alocat cu malloc.
Alternativ, se poate folosi opţiunea -lmcheck la legarea programului fără a afecta sursa acestuia.
Varianta cea mai simplă este folosirea variabilei de mediu MALLOC_CHECK_. Dacă un program va fi lansat în execuţie cu variabila MALLOC_CHECK_ configurată, atunci vor fi afişate mesaje de eroare (eventual programul va fi terminat forţat - aborted).
Mai jos se găseşte un exemplu de cod cu probleme în alocarea şi folosirea heap-ului:
Exemplu: mcheck_test.c
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(void)
{
  int *v1;
  v1 = (int *) malloc(5 * sizeof(*v1));

  if (NULL == v1) {
    perror("malloc");
    exit (EXIT_FAILURE);
  }
  /* overflow */
  v1[6] = 100;
  free(v1);
  /* write after free */
  v1[6] = 100;
  /* reallocate v1 */
  v1 = malloc(10 * sizeof(int));
  if (NULL == v1) {
    perror("malloc");
    exit (EXIT_FAILURE);
  }
  return 0;
}
```
Mai jos programul este compilat şi rulat. Mai întâi este rulat fără opţiuni de mcheck, după care se defineşte variabila de mediu MALLOC_CHECK_ la rularea programului. Se observă că deşi se depăşeşte spaţiul alocat pentru vectorul v1 şi se referă vectorul după eliberarea spaţiului, o rulare
simplă nu rezultă în afişarea nici unei erori.
Totuşi, dacă definim variabila de mediu MALLOC_CHECK_, se detectează cele două erori. De observat că o eroare este detectată doar în momentul unui nou apel de memorie interceptat de mcheck.
```
$ gcc -Wall –g mcheck_test.c -o mcheck_test
$ ./mcheck_test
$ MALLOC_CHECK_=1 ./mcheck_test malloc: using debugging hooks
*** glibc detected *** ./mcheck_test: free(): invalid pointer: 0x0000000000601010 ***
*** glibc detected *** ./mcheck_test: malloc: top chunk is corrupt: 0x0000000000601020***
```
mcheck nu este o soluţie completă şi nu detectează toate erorile ce pot apărea în lucrul cu memoria. Detectează, totuşi, un număr important de erori şi reprezintă o facilitate importantă a glibc. O descriere completă găsiți în pagina asociată din manualul glibc.

## Leak-uri de memorie
Un leak de memorie apare în două situaţii:
- un program omite să elibereze o zonă de memorie
- un program pierde referinţa la o zonă de memorie dealocată şi, drept consecință, nu o poate elibera

Memory leak-urile au ca efect reducerea cantităţii de memorie existentă în sistem. Se poate ajunge, în situaţiile extreme, la consumarea întregii memorii a sistemului şi la imposibilitatea de funcţionare a diverselor aplicaţii ale acestuia.
Ca şi în cazul problemei accesului invalid la memorie, utilitarul Valgrind este foarte util în detectarea leak-urilor de memorie ale unui program.
## mtrace
Un utilitar care poate fi folosit la depanarea erorilor de lucru cu memoria este mtrace. Acest utilitar ajută la identificarea leak-urilor de memorie ale unui program.
Utilitarul mtrace se folosete cu apelurile mtrace şi muntrace implementate în biblioteca standard C:
```
#include <mcheck.h>
void mtrace(void);
void muntrace(void);
```
Utilitarul **mtrace** introduce handlere pentru apelurile de biblioteca de lucru cu memoria (malloc, realloc, free). Apelurile mtrace şi muntrace activează, respectiv dezactivează monitorizarea apelurilor de bibliotecă de lucru cu memoria. Jurnalizarea operaţiilor efectuate se realizează în fişierul definit de variabile de mediu MALLOC_TRACE.
După ce apelurile au fost înregistrate în fişierul specificat, utilizatorul poate să folosească utilitarul mtrace pentru analiza acestora.
În exemplul de mai jos este prezentată o situaţie în care se alocă memorie fără a fi eliberată:
Exemplu: mtrace_test.c
```
#include <stdlib.h>
#include <mcheck.h>
int main(void)
{
  /* start memcall monitoring */
  mtrace();
  malloc(10);
  malloc(20);
  malloc(30);
  /* stop memcall monitoring */
  muntrace();
  return 0;
}
```
În secvenţa de comenzi de mai jos se compilează fişierul de mai sus, se stabileşte fişierul de jurnalizare şi se rulează comanda mtrace pentru a detecta problemele din codul de mai sus.
```
$ gcc -Wall -g mtrace_test.c -o mtrace_test
$ export MALLOC_TRACE=./mtrace.log
$ ./mtrace_test
$ cat mtrace.log
= Start
@ ./mtrace_test:[0x40054b] + 0x601460 0xa
@ ./mtrace_test:[0x400555] + 0x601480 0x14
@ ./mtrace_test:[0x40055f] + 0x6014a0 0x1e
= End
$ mtrace mtrace_test mtrace.log
Memory not
freed:
-----------------
Address Size Caller
0x0000000000601460 0xa at /home/mtrace.c:11
0x0000000000601480 0x14 at /home/mtrace.c:12
0x00000000006014a0 0x1e at /home/mtrace.c:15
```
Mai multe informaţii despre detectarea problemelor de alocare folosind mtrace gasiti în pagina asociată din manualul glibc.

## Dublă dezalocare
Denumirea "dublă dezalocare" oferă o bună intuiţie asupra cauzei: eliberarea de două ori a aceluiaşi spaţiu de memorie. Dubla dezalocare poate avea efecte negative deoarece afectează structurile interne folosite pentru a gestiona memoria ocupată.
În ultimele versiuni ale bibliotecii standard C se detectează automat cazurile de dublă dezalocare. Fie exemplul de mai jos:
Exemplu: dfree.c
```
#include <stdlib.h>
int main(void)
{
  char *p;
  p = malloc(10);
  free(p);
  free(p);
  return 0;
}
```
Rularea executabilului obţinut din programul de mai sus duce la afişarea unui mesaj specific
al glibc de eliberare dublă a unei regiuni de memorie şi terminarea programului:
```
$ gcc -Wall -g dfree.c -o dfree
$ ./dfree
*** glibc detected *** ./dfree: double free or corruption (fasttop):
0x0000000000601010 ***
======= Backtrace: =========
/lib/libc.so.6[0x2b675fdd502a]
/lib/libc.so.6(cfree+0x8c)[0x2b675fdd8bbc]
./dfree[0x400510]
/lib/libc.so.6( libc_start_main+0xf4)[0x2b675fd7f1c4]
./dfree[0x400459]
```
Situaţii de dezalocare sunt, de asemenea, detectate de Valgrind.

## Valgrind
Valgrind reprezintă o suită de utilitare folosite pentru operaţii de debugging şi profiling. Cel mai
popular este Memcheck, un utilitar care permite detectarea de erori de lucru cu memoria (accese
invalide, memory leak-uri etc.). Alte utilitare din suita Valgrind sunt Cachegrind, Callgrind utile
pentru profiling sau Helgrind, util
pentru depanarea programelor multithreaded.
În continuare ne vom referi doar la utilitarul Memcheck de detectare a erorilor de lucru cu
memoria. Mai precis, acest utilitar detectează următoarele tipuri de erori:
- folosirea de memorie neiniţializată
- citirea/scrierea din/în memorie după ce regiunea respectivă a fost eliberată
- citirea/scrierea dincolo de sfârşitul zonei alocate
- citirea/scrierea pe stivă în zone necorespunzătoare
- memory leak-uri
- folosirea necorespunzătore de apeluri malloc/new şi free/delete

Valgrind nu necesită adaptarea codului unui program ci folosete direct executabilul (binarul) asociat unui program. La o rulare obişnuită Valgrind va primi argumentul --tool pentru a preciza utilitarul folosit şi programul care va fi verificat de erori de lucru cu memoria.
În exemplul de rulare de mai jos se foloseşte programul prezentat la seciunea mcheck:
```
$ valgrind --tool=memcheck ./mcheck
==17870== Memcheck, a memory error detector.
==17870== Copyright (C) 2002-2007, and GNU GPL'd, by Julian Seward et al.
==17870== Using LibVEX rev 1804, a library for dynamic binary translation.
==17870== Copyright (C) 2004-2007, and GNU GPL'd, by OpenWorks LLP.
==17870== Using valgrind-3.3.0-Debian, a dynamic binary instrumentation framework.
==17870== Copyright (C) 2000-2007, and GNU GPL'd, by Julian Seward et al.
==17870== For more details, rerun with: -v
==17870==
==17870== Invalid write of size 4
==17870== at 0x4005B1: main (mcheck_test.c:17)
==17870== Address 0x5184048 is 4 bytes after a block of size 20 alloc'd
==17870== at 0x4C21FAB: malloc (vg_replace_malloc.c:207)
==17870== by 0x400589: main (mcheck_test.c:10)
==17870==
==17870== Invalid write of size 4
==17870== at 0x4005C8: main (mcheck_test.c:22)
==17870== Address 0x5184048 is 4 bytes after a block of size 20 free'd
==17870== at 0x4C21B2E: free (vg_replace_malloc.c:323)
==17870== by 0x4005BF: main (mcheck_test.c:19)
==17870==
==17870== ERROR SUMMARY: 2 errors from 2 contexts (suppressed: 8 from 1)
==17870== malloc/free: in use at exit: 40 bytes in 1 blocks.
==17870== malloc/free: 2 allocs, 1 frees, 60 bytes allocated.
==17870== For counts of detected errors, rerun with: -v
==17870== searching for pointers to 1 not-freed blocks.
==17870== checked 76,408 bytes.
==17870==
==17870== LEAK SUMMARY:
==17870== definitely lost: 40 bytes in 1 blocks.
==17870== possibly lost: 0 bytes in 0 blocks.
==17870== still reachable: 0 bytes in 0 blocks.
==17870== suppressed: 0 bytes in 0 blocks.
==17870== Rerun with --leak-check=full to see details of leaked memory.
```
S-a folosit utilitarul memcheck pentru obţinerea informaţiilor de acces la memorie.
Se recomandă folosirea opţiunii -g la compilarea programului pentru a prezenta în executabil informaţii de depanare. În rularea de mai sus, Valgrind a identificat două erori: una apare la linia 17 de cod şi este corelată cu linia 10 (malloc), iar cealaltă apare la linia 22 şi este corelată cu linia 19 (free):
```
10 v1 = (int *) malloc (5 * sizeof(*v1));
11 if (NULL == v1) {
12 perror ("malloc");
13 exit (EXIT_FAILURE);
14 }
15
16 /* overflow */
17 v1[6] = 100;
18
19 free(v1);
20
21 /* write after free */
22 v1[6] = 100;
```
Exemplul următor reprezintă un program cu o gamă variată de erori de alocare a memoriei:
Exemplu: valgrind_test.c
```
#include <stdlib.h>
#include <string.h>
int main(void)
{
  char buf[10];
  char *p;
  /* no init */
  strcat(buf, "al");
  /* overflow */
  buf[11] = 'a';
  p = malloc(70);
  p[10] = 5;
  free(p);
  /* write after free */
  p[1] = 'a';
  p = malloc(10);
  /* memory leak */
  p = malloc(10);
  /* underrun */
  p--;
  *p = 'a';
  return 0;
}
```
În continuare, se prezintă comportamentul executabilului obţinut la o rulare obişnuită şi la o rulare sub Valgrind:
```
$ gcc -Wall -g valgrind_test.c -o valgrind_test
$ ./valgrind_test
$ valgrind --tool=memcheck ./valgind_test
==18663== Memcheck, a memory error detector.
==18663== Copyright (C) 2002-2007, and GNU GPL'd, by Julian Seward et al.
==18663== Using LibVEX rev 1804, a library for dynamic binary translation.
==18663== Copyright (C) 2004-2007, and GNU GPL'd, by OpenWorks LLP.
==18663== Using valgrind-3.3.0-Debian, a dynamic binary instrumentation framework.
==18663== Copyright (C) 2000-2007, and GNU GPL'd, by Julian Seward et al.
==18663== For more details, rerun with: -v
==18663==
==18663== Conditional jump or move depends on uninitialised value(s)
==18663== at 0x40050D: main (valgrind_test.c:10)
==18663==
==18663== Invalid write of size 1
==18663== at 0x400554: main (valgrind_test.c:20)
==18663== Address 0x5184031 is 1 bytes inside a block of size 70 free'd
==18663== at 0x4C21B2E: free (vg_replace_malloc.c:323)
==18663== by 0x40054B: main (valgrind_test.c:17)
==18663==
==18663== Invalid write of size 1
==18663== at 0x40057C: main (valgrind_test.c:28)
==18663== Address 0x51840e7 is 1 bytes before a block of size 10 alloc'd
==18663== at 0x4C21FAB: malloc (vg_replace_malloc.c:207)
==18663== by 0x40056E: main (valgrind_test.c:24)
==18663==
==18663== ERROR SUMMARY: 6 errors from 3 contexts (suppressed: 8 from 1)
==18663== malloc/free: in use at exit: 20 bytes in 2 blocks.
==18663== malloc/free: 3 allocs, 1 frees, 90 bytes allocated.
==18663== For counts of detected errors, rerun with: -v
==18663== searching for pointers to 2 not-freed blocks.
==18663== checked 76,408 bytes.
==18663==
==18663== LEAK SUMMARY:
==18663== definitely lost: 20 bytes in 2 blocks.
==18663== possibly lost: 0 bytes in 0 blocks.
==18663== still reachable: 0 bytes in 0 blocks.
==18663== suppressed: 0 bytes in 0 blocks.
==18663== Rerun with --leak-check=full to see details of leaked memory.
```
Se poate observa că la o rulare obişnuită programul nu generează nici un fel de eroare. Totuşi, la rularea
Valgrind apar erori în 3 contexte:
1. la apelul strcat (linia 10) şirul nu a fost iniţializat
2. se scrie în memorie după free (linia 20: p[1] = 'a')
3. underrun (linia 28)
În plus există leak-uri de memorie datorită noului apel malloc care asociază o nouă valoare lui p (linia 24).

Valgrind este un utilitar de bază în depanarea programelor. Este facil de folosit (nu este intrusiv, nu necesită modificarea surselor) şi permite detectarea unui număr important de erori de programare apărute ca urmare a gestiunii defectuoase a memoriei.
Informaţii complete despre modul de utilizare a Valgrind şi a utilitarelor asociate se găsesc în paginile de documentaţie Valgrind: http://valgrind.org/docs/manual/index.html

## Alte utilitare pentru depanarea problemelor de lucru cu memoria
Utilitarele prezentate mai sus nu sunt singurele folosite pentru detectarea problemelor apărute în lucrul cu memoria. Alte utilitare sunt:
- http://en.wikipedia.org/wiki/Category:Memory_management_software
- dmalloc
- mpatrol
- DUMA
- Electric Fence

## Resurse utile
• Linux System Programming - Chapter 8 - Memory Management
• Windows System Programming - Chapter 5 - Memory Management (Win32 and Win64
Memory Management Architecture, Heaps, Managing Heap Memory
• Linux Application Programming - Chapter 7 - Memory Debugging Tools
• Windows Memory Management
• Virtual Memory Allocation and Paging
• GDB manual
• Valgrind Home
• Using Valgrind to Find Memory Leaks
• The Memory Management Reference
• IBM trial download: Rational Purify 7.0
• Using Purify
• Memory Management Software
• Smashing the Stack for Fun and Profit
