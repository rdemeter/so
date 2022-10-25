# GNU Make și GDB
 
## GNU Make

**Make** este un utilitar care permite automatizarea și eficientizarea sarcinilor. In mod particular este folosit pentru automatizarea compilării programelor. După cum s-a precizat, pentru obținerea unui executabil provenind din mai multe surse este ineficientă compilarea de fiecare dată a fiecărui fișier și apoi link-editarea.
Se compilează fiecare fișier separat, iar la o modificare se va recompila doar fișierul modificat.

## Exemplu simplu Makefile

Utilitarul Make foloseşte un fişier de configurare denumit Makefile. Un astfel de fişier conţine reguli şi comenzi de automatizare. În continuare este prezentat un exemplu foarte simplu de Makefile cu ajutorul căruia se va specifica sintaxa Make.

Exemplu 7. Makefile
```
all:
    gcc -Wall intro-04.c -o intro-04
clean:
    rm -f intro-04
```
Pentru rularea exemplului de mai sus se folosesc comenzile:
```
$ make
gcc -Wall intro-04.c -o intro-04
$ ./intro-04
variabila my_uber_var are valoarea 12345
```
Exemplul prezentat mai sus conţine două reguli: all şi clean. La rularea comenzii make se execută prima regulă din Makefile (în cazul de faţă all, nu contează în mod special denumirea). Comanda executată este gcc     -Wall intro-04.c -o intro-04. Se poate preciza explicit ce regulă să se execute prin transmiterea
ca argument comenzii make:
```
$ make clean
rm -f intro-04
$ make all
gcc -Wall intro-04.c -o intro-04
```
În exemplul de mai sus se foloseşte regula clean pentru a şterge executabilul intro-04 şi comanda make all pentru a obţine din nou acel executabil.

Se observă că nu se transmite niciun argument comenzii make pentru a preciza fișierul Makefile care va trebui analizat. În mod implicit, GNU Make caută, în ordine, fişierele GNUmakefile, Makefile, makefile şi le
analizează. Pentru a preciza ce fişier Makefile trebuie analizat, se foloseşte opţiunea -f. Astfel, în exemplul de mai jos, folosim fişierul Makefile.ex1:
```
$ mv Makefile Makefile.ex1
$ make
make: *** No targets specified and no makefile found.	Stop.
$ make -f Makefile.ex1
gcc -Wall intro-04.c -o intro-04
$ make -f Makefile.ex1 clean rm -f intro-04
$ make -f Makefile.ex1 all
gcc -Wall intro-04.c -o intro-04
```
În primă fază se incearcă rularea simplă a comenzii make. Întrucât make nu găseşte niciunul din fişierele GNUmakefile, Makefile sau makefile, returnează eroare. Prin precizarea opţiunii -f Makefile.ex1 se specifică fişierul Makefile de analizat. De asemenea, se poate preciza şi regula care să fie executată.

## Sintaxa unei reguli

În continuare este prezentată sintaxa unei reguli dintr-un fişier Makefile:
```
target: prerequisites
<tab> command
```
target este, de obicei, fişierul care se va obţine prin rularea comenzii command. După cum s-a observat şi
din exemplul anterior, poate să fie o ţintă virtuală care nu are asociat un fişier. prerequisites reprezintă
dependinţele necesare pentru a urmări regula; de obicei sunt fişiere necesare pentru obţinerea ţintei. <tab>
reprezintă caracterul tab şi trebuie neaparat folosit înaintea precizării comenzii. command o listă de comenzi
(niciuna*, una, oricâte) rulate în momentul în care se trece la obţinerea ţintei.

Un exemplu indicat pentru un fişier Makefile este:

Exemplu 8. Makefile.ex2
```
all: intro-04

intro-04: intro-04.o
    gcc intro-04.o -o intro-04

intro-04.o: intro-04.c
    gcc -Wall -c intro-04.c

clean:
    rm -f *.o *~ intro-04
```
Se observă prezenţa regulii all care va fi executată implicit. all are ca dependinţă intro-04 şi nu execută nicio
comandă; intro-04 are ca dependinţă intro-04.o şi realizează link-editarea fişierului intro-04.o; intro-04.o are
ca dependinţă intro-04.c şi realizează compilarea şi asamblarea fişierului intro-04.c. Pentru obţinerea executabilului se foloseşte comanda:
```
$ make -f Makefile.ex2
gcc -Wall -c intro-04.c
gcc intro-04.o -o intro-04
```

## Funcţionarea unui fişier Makefile

Pentru explicarea funcţionării unui fişier Makefile, vom folosi exemplul de mai sus. În momentul rulării comenzii make se poate preciza target-ul care se doreşte a fi obţinut. Dacă acesta nu este precizat, este considerat implicit primul target întâlnit în fişierul Makefile folosit; de obicei, acesta se va numi all.

Pentru obţinerea unui target trebuie satisfăcute dependinţele (prerequisites) acestuia. Astfel,

- pentru obţinerea target-ului all trebuie obţinut target-ul intro-04, care este un nume de executabil
- pentru obţinerea target-ului intro-04 trebuie obţinut target-ul intro-04.o
- pentru obţinerea target-ului intro-04.o trebuie obţinut intro-04.c; acest fişier există deja, şi cum acesta
nu apare la randul lui ca target în Makefile, nu mai trebuie obţinut
- drept urmare se rulează comanda asociată obţinerii intro-04.o; aceasta este gcc -Wall -c intro-04.c
- rularea comenzii duce la obţinerea target-ului intro-04.o, care este folosit ca dependinţă pentru intro-04
- se rulează comanda gcc intro-04.o -o intro-04 pentru obţinerea intro-04
- intro-04 este folosit ca dependinţă pentru all; acesta nu are asociată nicio comandă deci este automat
obţinut.

De remarcat este faptul că un target nu trebuie să aibă neapărat numele fişierului care se obţine. Se
recomandă, însă, acest lucru pentru înţelegerea mai uşoară a fişierului Makefile, şi pentru a beneficia de faptul
că make utilizează timpul de modificare al fişierelor pentru a decide când nu trebuie să facă nimic.

Acest format al fişierului Makefile are avantajul eficientizării procesului de compilare. Astfel, după ce s-a obţinut executabilul intro-04 conform fişierului Makefile anterior, o nouă rulare a make nu va genera nimic:
```
$ make -f Makefile.ex2
make: Nothing to be done for `all'.
```
Mesajul "Nothing to be done for 'all'" înseamnă că ţinta all are toate dependinţele satisfăcute. Dacă, însă,
folosim comanda touch pe fişierul obiect, se va considera că a fost modificat şi vor trebui refăcute target-urile care depindeau de el:
```
$ touch intro-04.o
$ make -f Makefile.ex2
gcc intro-04.o -o intro-04
$ make -f Makefile.ex2
make: Nothing to be done for `all'.
```
La fel, dacă ştergem fişierul obiect, acesta va trebui regenerat, ca şi toate target-urile care depindeau, direct sau indirect, de el:
```
$ rm intro-04.o
$ make -f Makefile.ex2
gcc -Wall -c intro-04.c
gcc intro-04.o -o intro-04
```

## Folosirea variabilelor

Un fişier Makefile permite folosirea de variabile. Astfel, un exemplu uzual de fişier Makefile este:

Exemplu 9. Makefile.ex3
```
CC = gcc
CFLAGS = -Wall -g all: intro-04
intro-04: intro-04.o
    $(CC) $^ -o $@

intro-04.o: intro-04.c
    $(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
    rm -f *.o *~ intro-04
```
În exemplul de mai sus au fost definite variabilele CC şi CFLAGS. Variabila CC reprezintă compilatorul folosit, iar variabila CFLAGS reprezintă opţiunile (flag-urile) de compilare utilizate; în cazul de faţă sunt afişarea avertismentelor şi compilarea cu suport de depanare.
```
Referirea unei variabile se realizează prin intermediul construcţiei $(VAR_NAME).
Astfel, $(CC) se înlocuieşte cu gcc, iar $(CFLAGS) se înlocuieşte cu -Wall -g.
```
                             
```
Nişte variabile predefinite sunt $@, $^ şi $<.
- $@ se expandează la numele target-ului. 
- $^ se expandează la lista de cerinţe,
- $< se expandează la prima cerinţă.
```
                                                                                                                               În acest fel, comanda

```
$(CC) $^ -o $@
se expandează la
gcc intro-04.o -o intro-04
```
iar comanda
```
$(CC) $(CFLAGS) -c $<
se expandează la
gcc -Wall -g -c intro-04.c
```
Pentru mai multe detalii despre variabile consultaţi pagina info sau manualul online.

## Folosirea regulilor implicite

De foarte multe ori nu este nevoie să se precizeze comanda care trebuie rulată; aceasta poate fi detectată implicit.

Astfel, în cazul în care se precizează regula:
```
main.o: main.c
```
se foloseşte implicit comanda
```
$(CC) $(CFLAGS) -c -o $@ $<
```
Astfel, fişierul Makefile.ex2 de mai sus poate fi simplificat, folosind reguli implicite, ca mai jos:

Exemplu 10. Makefile.ex4
```
CC = gcc
CFLAGS = -Wall -g all: intro-04
intro-04: intro-04.o intro-04.o: intro-04.c
.PHONY: clean

clean:
    rm -f *.o *~ intro-04
```
Pentru rulare, se foloseşte comanda:
```
$ make -f Makefile.ex4
gcc -Wall -g	-c -o intro-04.o intro-04.c gcc	intro-04.o	-o intro-04
```
Se observă că se folosesc reguli implicite. Makefile-ul poate fi simplificat şi mai mult, ca în exemplul de mai jos:

Exemplul 11. Makefile.ex5
```
CC = gcc
CFLAGS = -Wall -g all: intro-04
intro-04: intro-04.o

.PHONY: clean

clean:
    rm -f *.o *~ intro-04
```
În exemplul de mai sus s-a eliminat regula intro-04.o: intro-04.c. Make "vede" că nu există fişierul intro-04.o şi caută fişierul C din care poate să-l obţină. Pentru aceasta creează o regulă implicită şi compilează fişierul intro-04.c:
```
$ make -f Makefile.ex5
gcc -Wall -g	-c -o intro-04.o intro-04.c gcc intro-04.o	-o intro-04
```
De remarcat este faptul că dacă avem un singur fișier sursă nici nu trebuie să existe un fișier Makefile pentru a obține executabilul dorit.
```
$ ls
intro-04.c
$ make intro-04
cc intro-04.c -o intro-04
```
Pentru mai multe detalii despre reguli implicite consultaţi pagina info sau manualul online.

## Exemplu complet de Makefile

Folosind toate facilitaţile de până acum, ne propunem compilarea unui executabil client şi a unui executabil server.

Fişierele folosite sunt:

- executabilul server depinde de fişierele C server.c, sock.c, cli_handler.c, log.c, sock.h, cli_handler.h, log.h;
- executabilul client depinde de fişierele C client.c, sock.c, user.c, log.c, sock.h, user.h, log.h;

Dorim, aşadar, obţinerea executabilelor client şi server pentru rularea celor două entităţi. Structura fişierului Makefile este prezentată mai jos:

Exemplu 12. Makefile.ex6
```
CC = gcc	# compilatorul folosit
CFLAGS = -Wall -g	# optiunile pentru compilare
LDLIBS = -lefence	# optiunile pentru linking
```
```
# creeaza executabilele client si server
all: client server

# leaga modulele client.o user.o sock.o in executabilul client
client: client.o user.o sock.o log.o

# leaga modulele server.o cli_handler.o sock.o in executabilul server
server: server.o cli_handler.o sock.o log.o

# compileaza fisierul client.c in modulul obiect client.o
client.o: client.c sock.h user.h log.h

# compileaza fisierul user.c in modulul obiect user.o
user.o: user.c user.h

# compileaza fisierul sock.c in modulul obiect sock.o
sock.o: sock.c sock.h

# compileaza fisierul server.c in modulul obiect server.o
server.o: server.c cli_handler.h sock.h log.h

# compileaza fisierul cli_handler.c in modulul obiect cli_handler.o
cli_handler.o: cli_handler.c cli_handler.h

# compileaza fisierul log.c in modulul obiect log.o
log.o: log.c log.h

.PHONY: clean

clean:
    rm -fr *~ *.o server client
```
Pentru obţinerea executabilelor server şi client se foloseşte:
```
$ make -f Makefile.ex6
gcc -Wall -g -c -o client.o client.c gcc -Wall -g -c -o user.o user.c
gcc -Wall -g -c -o sock.o sock.c gcc -Wall -g -c -o log.o log.c
gcc client.o user.o sock.o log.o -lefence -o client gcc -Wall -g -c -o server.o server.c
gcc -Wall -g -c -o cli_handler.o cli_handler.c
gcc server.o cli_handler.o sock.o log.o -lefence -o server
```
Regulile implicite intră în vigoare şi se obţin, pe rând, fişierele obiect şi fişierele executabile. Variabila LDLIBS este folosită pentru a preciza bibliotecile cu care se face link-editarea pentru obţinerea executabilului.

# Depanarea programelor

Exist câteva unelte GNU care pot fi folosite atunci când nu reuşim să facem un program să ne asculte. gdb, acronimul de la "Gnu DeBugger" este probabil cel mai util dintre ele, dar există și altele, cum ar fi ElectricFence, gprof sau mtrace. gdb va fi prezentat pe scurt în secţiunile ce urmează.

## GDB

Dacă doriţi să depanaţi un program cu GDB nu uitaţi să compilaţi programul cu optiunea -g. Folosirea acestei opţiuni duce la includerea în executabil a informaţiilor de debug.

## Rularea GDB

GDB poate fi folosit în două moduri pentru a depana programul:

- rulându-l folosind comanda gdb
- folosind fisierul core generat în urma unei erori grave (de obicei segmentation fault)


Cea de a doua modalitate este utilă în cazul în care bug-ul nu a fost corectat înainte de lansarea programului.
În acest caz, dacă utilizatorul întâlneşte o eroare gravă, poate trimite programatorului fişierul core cu care acesta poate depana programul şi corecta bug-ul.

Cea mai simplă formă de depanare cu ajutorul GDB este cea în care dorim să determinăm linia programului la care s-a produs eroarea. Pentru exemplificare considerăm următorul program:

Exemplu 13. exemplul-6.c
```
#include <stdio.h>
int f(int a, int b)
{
    int c;
    c = a + b;
    return c;
}

int main()
{
    char *bug = 0;
    *bug = f(1, 2);
    return 0;
}
```
După compilarea programului acesta poate fi depanat folosind GDB. După încărcarea programului de depanat, GDB într-un mod interactiv. Utilizatorul poate folosi apoi comenzi pentru a depana programul:
```
$ gcc -Wall -g exemplul-6.c
$ gdb a.out
[...]
(gdb) run
Starting program: a.out

Program received signal SIGSEGV, Segmentation fault.
0x08048411 in main () at exemplul-6.c:16
16	*bug=f(1, 2);
(gdb)
```
 
Prima comandă folosită este **run**. Această comandă va porni execuţia programului. Dacă această comandă primeşte argumente de la utilizator, acestea vor fi transmise programului. Înainte de a trece la prezentarea unor comenzi de bază din gdb, să demonstrăm cum se poate depana un program cu ajutorul fişierului core:
```
# ulimit -c 4
# ./a.out
Segmentation fault (core dumped)
# gdb a.out core
Core was generated by `./a.out'.
Program terminated with signal 11, Segmentation fault.
#0	0x08048411 in main () at exemplul-6.c:16
16	*bug=f(1, 2); 
(gdb)
```
## Comenzi de bază GDB

Câteva din comenzile de bază în gdb sunt **breakpoint**, **next** şi **step**. Prima dintre ele primeşte ca argument un nume de funcţie (ex: main), un număr de linie şi, eventual, un fişier (ex: break sursa.c:50) sau o adresă (ex: break *0x80483d3). Comanda next va continua execuția programului până ce se va ajunge la următoarea linie din codul sursă. Dacă linia de executat conţine un apel de funcţie, funcţia se va executa complet. Dacă se doreşte şi inspectarea funcţiilor trebuie să se folosească step. Folosirea acestor comenzi este exemplificată mai jos:
```
$ gdb a.out
(gdb) break main
Breakpoint 1 at 0x80483f6: file exemplul-6.c, line 14. 
(gdb) run
Starting program: a.out

Breakpoint 1, main () at exemplul-6.c:14
14	char *bug=0; 
(gdb) next
16	*bug=f(1, 2); 
(gdb) next

Program received signal SIGSEGV, Segmentation fault.
0x08048411 in main () at exemplul-6.c:16
16	*bug=f(1, 2); 
(gdb) run
The program being debugged has been started already. 
Start it from the beginning? (y or n) y
Starting program: a.out

Breakpoint 1, main () at exemplul-6.c:14
14	char *bug=0; 
(gdb) next
16	*bug=f(1, 2); 
(gdb) step
f (a=1, b=2) at exemplul-6.c:8
8	c=a+b; 
(gdb) next
9	return c; 
(gdb) next
10	} 
(gdb) next

Program received signal SIGSEGV, Segmentation fault.
0x08048411 in main () at exemplul-6.c:16
16	*bug=f(1, 2); 
(gdb)
```
O altă comandă utilă este **list**. Aceasta va lista fişierul sursă al programului depanat. Comanda primeşte ca argument un număr de linie (eventual nume fişier), o funcţie sau o adresă de la care să listeze. Al doilea argument este opţional şi precizează câte linii vor fi afişate. În cazul în care comanda nu are niciun parametru, ea va lista de unde s-a oprit ultima afişare.
```
$ gdb a.out
(gdb) list exemplul-6.c:1
1	/* exemplul-6.c */
2	#include <stdio.h>>
3
4	int f(int a, int b)
5	{
6	int c;
7
8	c=a+b;
9	return c;
10	}
(gdb) break exemplul-6.c:8
Breakpoint 1 at 0x80483d6: file exemplul-6.c, line 8. 
(gdb) run
Starting program: a.out

Breakpoint 1, f (a=1, b=2) at exemplul-6.c:8
8	c=a+b; (gdb) next
9	return c; 
(gdb) continue
Continuing.

Program received signal SIGSEGV, Segmentation fault.
0x08048411 in main () at exemplul-6.c:16
16	*bug=f(1, 2);
```
Comanda **continue** se foloseşte atunci când se doreşte continuarea execuţiei programului. Ultima comandă de bază este **print**. Cu ajutorul acesteia se pot afişa valorile variabilelor din funcţia curentă sau a variabilelor globale. print poate primi ca argument şi expresii complicate (dereferenţieri de pointeri, referenţieri ale variabilelor, expresii aritmetice, aproape orice expresie C valid). În plus, print poate afişa structuri de date precum struct şi union.
```
$ gdb a.out
(gdb) break f
Breakpoint 1 at 0x80483d6: file exemplul-6.c, line 8.
(gdb) run
Starting program: a.out

Breakpoint 1, f (a=1, b=2) at exemplul-6.c:8
8	c=a+b; 
(gdb) print a
$1 = 1
(gdb) print b
$2 = 2
(gdb) print c
$3 = 1073792080 
(gdb) next
9	return c; 
(gdb) print c
$4 = 3
(gdb) finish
Run till exit from #0	f (a=1, b=2) at exemplul-6.c:9
0x08048409 in main () at exemplul-6.c:16
16	*bug=f(1, 2); Value returned is $5 = 3
(gdb) print bug
$6 = 0x0
```

## Resurse utile

1. GNU Make Manual
2. GDB Documentation
3. Visual C++ Express
4. Nmake Tool
5. Building and Linking with Libraries
6. Dynamic Link Library
7. Creating and Using DLLs
8. Dynamic Libraries

Note
1. ^ info make "Using Variables"
2. ^ http://www.gnu.org/software/make/manual/make.html#Using-Variables
3. ^ info make "Implicit Rules"
4. ^ http://www.gnu.org/software/make/manual/make.html#Implicit-Rules
5. ^ Nmake Reference
6. ^ Programul MSDNAA
