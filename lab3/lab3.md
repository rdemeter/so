
# Procese

- [Procese](#procese)
  * [Planificarea proceselor](#Planificarea-proceselor)
    * [First Come First Serve](#First-Come-First-Serve)
    * [Shortest Job First](#Shortest-Job-First)
    * [Shortest Remaining Time First](#Shortest-Remaining-Time-First)
    * [Round Robin](#Round-Robin)
  * [Procese în Linux](#Procese-în-Linux)
  * [Crearea unui proces](#crearea-unui-proces)
  * [Aşteptarea terminării unui proces](#Aşteptarea-terminării-unui-proces)
  * [Terminarea unui proces](#terminarea-unui-proces)
- [Depanarea unui proces](#depanarea-unui-proces)
- [Exerciții](#exerciții)

Un concept cheie în orice sistem de operare este procesul. Un **proces** este un program în execuţie. Procesele formează unitatea primitivă prin care sistemul de operare alocă resurse utilizatorilor. Orice proces are un spaţiu de adrese şi unul sau mai multe fire de execuţie. Putem avea mai multe procese ce execută același program, dar oricare două procese sunt complet independente.

Spaţiile de adrese, regiștrii generali, PC (contor program), SP (indicator stivă), tabelele de fisiere deschise, lista de semnale (blocate, ignorate sau care asteaptă să fie trimise procesului), handler-ele pentru semnale, informatiile referitoare la sistemele de fișiere (directorul rădăcină, directorul curent), toate acestea NU sunt partajate, ci aparțin fiecărui proces în parte. Aceste informații necesare pentru rularea programului sunt ținute de sistemul de operare într-o structură numită **Process Control Block**, câte una pentru fiecare proces existent în sistem.

În momentul lansării în execuţie al unui program, în sistemul de operare se va crea un proces pentru alocarea resurselor necesare rulării programului respectiv. Fiecare sistem de operare pune la dispoziţie apeluri de sistem pentru crearea unui proces, terminarea unui proces, așteptarea terminării unui proces precum şi apeluri pentru duplicarea descriptorilor de resurse între procese ori închiderea acestor descriptori.

În general un proces rulează într-un mediu specificat printr-un set de variabile de mediu. O variabilă de mediu este o pereche NUME=valoare. Un proces poate să verifice sau să seteze valoarea unei variabile de mediu printr-o serie de apeluri de bibliotecă.

Pe sisteme de 32 de biți fiecare proces are un spaţiu de adrese de 4 GiB din care 2 (sau în anumite configuraţii 3) GiB sunt disponibili pentru alocare procesului, iar ceilalți 2 (respectiv 1) GiB fiind rezervat sistemului de operare (codul kernelului și al driverelor, date, cache-uri, etc.). Așadar fiecare proces "vede" sistemul de operare în spațiul său de adrese însă nu poate accesa zona respectivă decât prin intermediul apelurilor de sistem (comutând procesorul în modul de lucru privilegiat). Pe sisteme de 64 de biți spaţiul total de adrese este de 16 EiB, iar pe sisteme de 16 biți de doar 64 KiB (procesoarele x86 pe 16 biți puteau adresa 220 = 1 MiB de memorie folosind la adresare doi regiștri de 16 biți întrucât, deși era procesor pe 16 biți, avea 20 de linii de adresă).

## Schimbarea de context

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/contextswitch.png?raw=true)

Context switching este mecanismul de înlocuire al unui proces pe procesor cu alt proces. Se salvează procesul/contextul vechi, se restaurează procesul/contextul nou.
O schimbare de context înseamnă overhead, adică mai multe schimbări de context: mai mult overhead, iar mai puține schimbări de context: mai puțină interactivitate

## Planificarea proceselor

Fiecare proces trece prin diferite stări în ciclul său de viață:
- new - un proces este în stare nouă atunci când un program prezent în memoria secundară este inițiat pentru execuție.
- ready - un proces trece de la starea nouă la starea gata după ce este încărcat în memoria principală și este gata de execuție. În stare gata, procesul așteaptă execuția sa de către procesor. În mediul de multiprogramare, multe procese pot fi prezente în starea gata.
- run - un proces trece de la starea pregătită la starea de rulare după ce i se atribuie CPU pentru execuție.
- terminate - un proces trece de la starea de rulare la starea de terminare după finalizarea execuției sale. După intrarea în starea de terminare, contextul (PCB) al procesului este șters de sistemul de operare.
- block or wait - un proces trece de la starea de rulare la starea de blocare sau de așteptare dacă necesită o operație I/O sau o resursă blocată în timpul execuției sale. După ce operațiunea I/O este finalizată sau resursa devine disponibilă, procesul trece la starea gata.

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/states.png?raw=true)

Numărul minim de stări prin care trece obligatoriu un proces este 4. Aceste stări sunt stare nouă, stare gata, stare de rulare și stare de terminare. Cu toate acestea, dacă un proces necesită și operarea I/O, atunci numărul minim de stări este 5.

Când procesul este prezent în sistem, fie va aștepta CPU-ul în starea gata, fie se va executa pe CPU. Sunt definite următoarele:
- arrival time - timpul de sosire este momentul în care un proces intră în coada de așteptare
- waiting time - timpul de așteptare este cantitatea de timp petrecută de un proces așteptând în coada pregătită pentru obținerea procesorului
- response time - timpul de răspuns este perioada de timp după care un proces primește CPU pentru prima dată după ce a intrat în coada de așteptare.
- burst time - timpul de execuție este timpul necesar unui proces pentru a se executa pe CPU.
- complition time - timpul de finalizare este momentul în care un proces își încheie execuția pe CPU și iese din sistem.
- turnaraund time - timpul de întoarcere este timpul total petrecut de un proces în sistem.

### First Come First Serve

În planificarea FCFS, procesul care ajunge primul în coada de așteptare este mai întâi executat de CPU. În caz de egalitate, procesul cu un ID de proces mai mic este executat mai întâi. Este întotdeauna non-preemptivă (nu se întrerupe de alt proces).

Avantaje - este simplu și ușor de înțeles, poate fi implementat cu ușurință folosind structura de date a cozii.

Dezavantaje - nu ia în considerare prioritatea sau timpul de explozie a proceselor, suferă de efectul de convoi.

**Exemplu:** Se considerară setul de 5 procese ale căror timpi de sosire și timpi de execuție sunt date mai jos:

| PID | Arrival time | Burst time | Exit time | Turn around time | Waiting time |
|-----|:------------:|:----------:|:---------:|:----------------:|:------------:|
| P1  |      3       |     4      |     7     |      7-3=4       |     4-4=0    |
| P2  |      5       |     3      |    13     |     13-5=8       |     8-3=5    |
| P3  |      0       |     2      |     2     |      2-0=2       |     2-2=0    |
| P4  |      5       |     1      |    14     |     14-5=9       |     9-1=8    |
| P5  |      4       |     3      |    10     |     10-4=6       |     6-3=3    |

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/fcfs.png?raw=true)

Se calculează:
Turn Around time = Exit time – Arrival time    și     Waiting time = Turn Around time – Burst time

Average Turn Around time = (4 + 8 + 2 + 9 + 6) / 5 = 29 / 5 = 5.8 unit

Average Waiting time = (0 + 5 + 0 + 8 + 3) / 5 = 16 / 5 = 3.2 unit

### Shortest Job First

Dintre toate procesele disponibile, procesul cu cel mai mic timp de execuție se va executa. Planificare SJF poate fi utilizată atât în modul non-preemptiv (SJF) cât și în cel preemtiv (SRTF). 
 
Dezavantaje - Nu poate fi implementat practic, deoarece timpul de execuție a proceselor nu poate fi cunoscut în avans. Aceasta duce la înfometarea proceselor cu timp de execuție mai mare (starvation).

**Exemplu:** Se considerară setul de 5 procese ale căror timpi de sosire și timpi de execuție sunt date mai jos:

| PID | Arrival time | Burst time | Exit time | Turn around time | Waiting time |
|-----|:------------:|:----------:|:---------:|:----------------:|:------------:|
| P1  |      3       |     1      |     7     |      7-3=4       |     4-1=3    |
| P2  |      1       |     4      |    16     |     16-6=15      |    15-4=11   |
| P3  |      4       |     2      |     9     |      9-4=5       |     5-2=3    |
| P4  |      0       |     6      |     6     |      6-0=6       |     6-6=0    |
| P5  |      2       |     3      |    12     |     12-2=10      |    10-3=7    |

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/sjf.png?raw=true)

Se calculează:
Turn Around time = Exit time – Arrival time    și     Waiting time = Turn Around time – Burst time

Average Turn Around time = (4 + 15 + 5 + 6 + 10) / 5 = 40 / 5 = 8 unit

Average waiting time = (3 + 11 + 3 + 0 + 7) / 5 = 24 / 5 = 4.8 unit

### Shortest Remaining Time First

SRTF este optim și garantează timpul mediu de așteptare minim, dar poate genera un număr mare de schimbări de context (întreruperi). Pentru a alege procesul cu cel mai mic timp rămas de execuție, SRTF necesită cunoașterea sau estimarea acestui timp pentru fiecare proces.

**Exemplu:** Se considerară setul de 5 procese ale căror timpi de sosire și timpi de execuție sunt date mai jos:

| PID | Arrival time | Burst time | Exit time | Turn around time | Waiting time |
|-----|:------------:|:----------:|:---------:|:----------------:|:------------:|
| P1  |      3       |     1      |     4     |      4-3=1       |    1-1=0     |
| P2  |      1       |     4      |     6     |      6-1=5       |    5-4=1     |
| P3  |      4       |     2      |     8     |      8-4=4       |    4-2=2     |
| P4  |      0       |     6      |    16     |     16-0=16      |   16-6=10    |
| P5  |      2       |     3      |    11     |     11-2=9       |    9-3=6     |

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/srtf.png?raw=true)

Se calculează:
Turn Around time = Exit time – Arrival time    și     Waiting time = Turn Around time – Burst time

Average Turn Around time = (1 + 5 + 4 + 16 + 9) / 5 = 35 / 5 = 7 unit

Average waiting time = (0 + 1 + 2 + 10 + 6) / 5 = 19 / 5 = 3.8 unit

### Round Robin

Planificare Round Robin utilizează un algoritm în care fiecare proces primește o porție mică de timp CPU în ordine circulară, permițând astfel ca procesele scurte să nu aștepte prea mult.

**Exercițiu:** Se consideră setul de 5 procese a căror oră de sosire și timp de execuție sunt date mai jos. Dacă politica de planificare a CPU este Round Robin cu cuantum de timp = 2 unități, calculați timpul mediu de așteptare și timpul mediu de întoarcere.

| PID | Arrival time | Burst time | Exit time | Turn around time | Waiting time |
|-----|:------------:|:----------:|:---------:|:----------------:|:------------:|
| P1  |      0       |     5      |     13    |     13-0=13      |    13-5=8    |
| P2  |      1       |     3      |     12    |     12-1=11      |    11-3=8    |
| P3  |      2       |     1      |      5    |      5-2=3       |     3-1=2    |
| P4  |      3       |     2      |      9    |      9-3=6       |     6-2=4    |
| P5  |      4       |     3      |     14    |     14-4=10      |    10-3=7    |

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/rr.png?raw=true)

Se calculează:
Turn Around time = Exit time – Arrival time    și     Waiting time = Turn Around time – Burst time

Average Turn Around time = (13 + 11 + 3 + 6 + 10) / 5 = 43 / 5 = 8.6 unit

Average waiting time = (8 + 8 + 2 + 4 + 7) / 5 = 29 / 5 = 5.8 unit

## Procese în Linux

Apelurile de sistem puse la dispoziție de Linux pentru gestionarea proceselor sunt: **fork** și **exec** pentru crearea unui proces și respectiv modificarea imaginii unui proces, **wait** și **waitpid** pentru așteptarea terminării unui proces și **exit** pentru terminarea unui proces. Pentru copierea descriptorilor de fișier Linux pune la dispoziţie apelurile de sistem **dup** și **dup2**. Pentru citirea, modificarea ori ștergerea unei variabile de mediu, biblioteca standard C pune la dispoziţie apelurile **getenv**, **setenv**, **unsetenv** precum şi un pointer la tabela de variabile de mediu **environ**.

Modul cel mai simplu prin care se poate crea un nou proces este prin folosirea funcţiei de bibliotecă **system**:
```c
int system(const char* command);
```
Apelul acestei funcţii are ca efect execuţia ca o comandă shell a comenzii reprezentate prin șirul de caractere command. Să luăm ca exemplu următorul program C:

Exemplu 1. sys1.c
```c
#include <stdlib.h>
int main(int argc, char **argv)
{
    system("ls -la $HOME");
}
```
care este echivalent cu
```
$ sh -c "ls -la $HOME"
```
Implementarea system: se creează un nou proces cu fork; procesul copil execută prin intermediul exec programul sh cu argumentele -c "comanda", timp în care părintele aşteaptă terminarea procesului copil.

## Crearea unui proces

În UNIX singura modalitate de creare a unui proces este prin apelul de sistem **fork()**:
```c
pid_t fork(void);
```
![image](https://github.com/rdemeter/so/blob/master/lab3/figs/fork_system_call.png?raw=true)

[https://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html](https://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/create.html)

Efectul este crearea unui nou proces - procesul copil, copie a celui care a apelat fork - procesul părinte. Copilul primeşte un nou PID de la sistemul de operare. Secvența clasică de creare a unui proces este prezentată în continuare:

Exemplu 3. ex1_fork.c
```c
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
  pid_t pid;
  switch (pid = fork())
  {
    case -1: // fork failed
      printf("fork failed\n");
      exit(-1);
    case 0: // child starts executing here
      ...
    default: // parent starts executing here
      printf("created process with pid=%d, getpid=%d, getppid=%d\n", pid, getpid(), getppid());
      ...
  }
}
```

După cum se observă din comentariile de mai sus, apelul de sistem fork întoarce PID-ul noului proces în procesul părinte și valoarea 0 în procesul copil.

Pentru aflarea PID-ului procesului curent ori al procesului părinte se va apela una din funcţiile de mai jos:
- funcţia **getpid** întoarce PID-ul procesului apelant: pid_t getpid(void);
- funcția **getppid** întoarce PID-ul părinte al procesului apelant:  pid_t getppid(void);

Exemplu 4. ex2_fork.cpp
```c
#include <iostream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>   // Declaration for exit()
using namespace std;

int globalVariable = 2;
int main() {  
  string sIdentifier;  
  int    iStackVariable = 20;
  pid_t pID = fork();  
    if (pID == 0)                // child
    {  
      // Code only executed by child process
      sIdentifier = "Child Process: ";
      globalVariable++;  
      iStackVariable++;  
    }  
    else if (pID < 0)            // failed to fork
    {  
      cerr << "Failed to fork" << endl;
      exit(1);  
      // Throw exception  
    }  
    else                         // parent  
    {  
      // Code only executed by parent process
      sIdentifier = "Parent Process:";
    }  
    // Code executed by both parent and child.
    cout << sIdentifier;
    cout << " Global variable: " << globalVariable;
    cout << " Stack variable: "  << iStackVariable << endl;

    return 0;
}
```
Compilarea si rularea aplicației:
```
$ g++ ex2_fork.cpp -o ex2_fork
$ ./ex2_fork 

Parent Process: Global variable: 2 Stack variable: 20
Child Process:  Global variable: 3 Stack variable: 21
```
Deci, utilizând fork(), procesul copil va avea propria variabilă globală şi propria variabilă de pe stivă, duplicate din procesul părinte!

Funcţia vfork() este acelaşi ca fork(), cu excepţia faptului că nu face o copie a spaţiului de adrese. Memoria este partajată, folosind o copie unică a tuturor variabilelor în memorie. Aceasta este de obicei folosit atunci când se utilizează fork() pentru un proces exec() care după rulare se termină. Funcţia vfork(), de asemenea, execută procesul copil primul şi reia procesul părinte atunci când procesul copil se termină.

Exemplu 4. ex2_vfork.cpp

Se va înlocui apelul funcției fork() cu vfork(), din exempul ex2_fork.cpp.
```
$ g++ ex2_vfork.cpp -o ex2_vfork
$ ./ex2_vfork 

Child Process:  Global variable: 3 Stack variable: 21
Parent Process: Global variable: 3 Stack variable: 21
```
## Aşteptarea terminării unui proces

Familia de funcții wait suspendă execuţia procesului apelant până când procesul (procesele) specificate în argumente fie s-au terminat fie au fost oprite (SIGSTOP).

pid_t waitpid(pid_t pid, int* status, int options);

Valoarile uzuale ale argumentului pid sunt identificatorul unui proces copil (spre exemplu, returnat de fork) sau -1, în cazul în care se doreşte aşteptarea oricărui proces copil.
Parametrul options permite specificarea diferitelor opțiuni.

Funcția va întoarce PID-ul procesului a cărui stare e raportată; infomațiile de stare sunt depuse ca int la adresa indicată prin argumentul status.

Starea procesului interogat se poate afla examinând status cu macrodefiniții precum WEXITSTATUS, care întoarce codul de eroare cu care s-a încheiat procesul copil, evaluând cei mai nesemnificativi 8 biți. Prin convenție, un cod de eroare egal cu 0 semnifică succes.

Există o variantă simplificată care așteaptă orice proces copil să se termine:
```c
pid_t wait(int *status);
```
Este echivalentă cu: waitpid(-1, &status, 0);

Pentru a folosi wait sau waitpid trebuie incluse header-ele sys/types.h şi sys/wait.h.

## Terminarea unui proces

Pentru terminarea procesului curent, Linux pune la dispoziție apelul de sistem exit. 
```c
#include <stdlib.h>
void exit (int status);
```
Acțiunile executate în urma apelului exit:
- procesul apelant se va termina imediat.
- toți descriptorii de fișier ai procesului sunt închiși, copiii procesului sunt "înfiați" de init, iar părintelui procesului îi va fi trimis un semnal SIGCHLD. Procesului părinte îi va fi întoarsă valoarea status ca rezultat al unei funcții de așteptare (wait sau waitpid).
- va şterge toate fişierele create cu tmpfile()
- va scrie bufferele streamurilor deschise şi le va închide.

Notă: Conform ISO C, un program care se termină cu un return x; din main() va avea același comportament ca și unul care apelează exit(x).

Pentru terminarea unui alt proces din sistem, se va trimite un semnal către procesul respectiv prin intermediul apelului de sistem kill. Mai multe detalii despre kill și semnale în laboratorul de semnale.

# Depanarea unui proces

Pe majoritatea sistemelor de operare pe care a fost portat, gdb nu poate detecta când un proces realizează o operație fork(). Atunci când programul este pornit, depanarea are loc exclusiv în procesul inițial, procesele copii nefiind atașate debugger-ului. În acest caz, singura soluție este introducerea unor întârzieri în execuția procesului nou creat (de exemplu, prin apelul de sistem sleep()), care să ofere programatorului suficient timp pentru a atașa manual gdb-ul la respectivul proces, presupunând că i-a aflat PID-ul în prealabil. Pentru a atașa debugger-ul la un proces deja existent, se folosește comanda **attach**, în felul următor:
```
(gdb) attach PID
```
Această metodă este destul de incomodă și poate cauza chiar o funcționare anormală a aplicației depanate, în cazul în care necesitățile de sincronizare între procese sunt stricte (de exemplu operații cu time-out).

Din fericire, pe un număr limitat de sisteme, printre care și Linux, gdb permite depanarea comodă a programelor care creează mai multe procese prin fork() și vfork(). Pentru ca gdb să urmărească activitatea proceselor create ulterior, se poate folosi comanda **set follow-fork-mode**, în felul următor:
```
(gdb) set follow-fork-mode mode
```
unde **mode** poate lua valoarea **parent**, caz în care debugger-ul continuă depanarea procesului părinte, sau valoarea **child**, și atunci noul proces creat va fi depanat în continuare. Se poate observa că în această manieră debugger-ul este atașat la un moment dat doar la un singur proces, neputând urmări mai multe simultan.

Cu toate acestea, gdb poate ține evidența tuturor proceselor create de către programul depanat, deși în continuare numai un singur proces poate fi rulat prin debugger la un moment dat. Comanda **set detach-on-fork** realizează acest lucru:
```
(gdb) set detach-on-fork mode
```
unde **mode** poate fi **on**, atunci când gdb se va atașa unui singur proces la un moment dat (comportament implicit), sau **off**, caz în care gdb se atașează la toate procesele create în timpul execuției, și le suspendă pe acelea care nu sunt urmărite, în funcție de valoarea setării **follow-fork-mode**.

Comanda **info forks** afișează informații legate de toate procesele aflate sub controlul gdb la un moment dat:
```
(gdb) info forks
```
De asemenea, comanda fork poate fi utilizată pentru a seta unul din procesele din listă drept cel activ (care este urmărit de debugger).
```
(gdb) fork fork-id
```
unde **fork-id** este identificatorul asociat procesului, așa cum apare în lista afișată de comanda **info forks**.

Atunci când un anumit proces nu mai trebuie urmărit, el poate fi înlăturat din listă folosind comenzile **detach fork** şi **delete fork**:
```
(gdb) detach fork fork-id
(gdb) delete fork fork-id
```
Diferenţa dintre cele două comenzi este că detach fork lasă procesul să ruleze independent, în continuare, în timp ce delete fork îl încheie.

Pentru a ilustra aceste comenzi într-un exemplu concret, să considerăm programul următor:

Exemplu 5. forktest.c
```c
// forktest.c
1#include <stdio.h>
2#include <sys/types.h>
3#include <sys/wait.h>
4#include <unistd.h>
5
67int main(int argc, char** argv) {
8   pid_t childPID = fork();
9
10  if (childPID < 0) {
11    // An error occured
12    fprintf(stderr, "Could not fork!\n");
13    return -1;
14  } else if (childPID == 0) {
15
16  // We are in the child process
17  printf("The child process is executing...\n");
18  sleep(2);
19
20  } else {
21
22  // We are in the parent process
23  if (wait(NULL) < 0) {
24    fprintf(stderr, "Could not wait for child!\n");
25    return -1;
26  }
27  printf("Everything is done!\n");
28  }
29  return 0;
30 }
```
Dacă vom rula programul cu parametrii impliciţi de depanare, vom constata că gdb va urmări exclusiv execuţia procesului părinte:
```
$ gcc forktest.c -O0 -g3 -o forktest
$ gdb ./forktest
[...]
(gdb) run

Starting program: /home/forktest
The child process is executing...
Everything is done!
Program exited normally.
```
Punem câte un breakpoint în codul asociat procesului părinte, respectiv procesului copil, pentru a evidenţia mai bine acest comportament:
```
(gdb) break 17
Breakpoint 1 at 0x8048497: file forktest.c, line 17.
(gdb) break 27
Breakpoint 2 at 0x80484f0: file forktest.c, line 27.

(gdb) run
Starting program: /home/forktest
The child process is executing...

Breakpoint 2, main () at forktest.c:27
27	printf("Everything is done!\n");
(gdb) continue
Continuing.
Everything is done!

Program exited normally.
```
Setăm debugger-ul să urmărească procesele copii, şi observăm că de data aceasta celălalt breakpoint este atins:
```
(gdb) set follow-fork-mode child
(gdb) run
Starting program: /home/forktest
[Switching to process 6217]

Breakpoint 1, main () at forktest.c:17
17	printf("The child process is executing...\n");
(gdb) continue
Continuing.
The child process is executing...

Program exited normally.
Everything is done!
```
Observăm că ultimele două mesaje au fost inversate, față de cazul precedent: debugger-ul încheie procesul copil, apoi procesul părinte afișează mesajul de final (Everything is done!).

## Exerciții

**Exercițiu:** test.c

Scrieți secvența afișată după rularea următorului program și desenați arborele de procese:
```c
#include <stdio.h>
#include <unistd.h>
int main(void)
{
    printf("A\n");
    fork();
    printf("B\n");
    fork();
    printf("C\n");
    fork();
    printf("D\n");
    return 0;
}
```
![image](https://github.com/rdemeter/so/blob/master/lab3/figs/ex1_fork_pids.png?raw=true)

**Exercițiu:** [https://www.geeksforgeeks.org/fork-practice-questions/](https://www.geeksforgeeks.org/fork-practice-questions/)

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/ex2_fork_pids.png?raw=true)

Explicaţie:
1. Va crea două procese, unul părinte P (are ID de proces al procesului copil) și celălalt este copilul C1 (ID de proces = 0).
2. În instrucțiunea if am folosit operatorul SAU ( || ) și în acest caz a doua condiție este evaluată când prima condiție este falsă.
3. Procesul părinte P va returna un întreg pozitiv, astfel încât să execute direct instrucțiunea și să creeze încă două procese (unul părinte P și celălalt este copilul C2). Procesul copil C1 va returna 0, așa că verifică a doua condiție și a doua condiție creează din nou încă două procese (unul părinte C1 și celălalt este copilul C3).
4. C1 returnează un întreg pozitiv, astfel încât va crea în continuare încă două procese (unul părinte C1 și celălalt este copilul C4). Copilul C3 returnează 0, așa că va imprima direct 1.
5. Se va rula comanda **pstree -ps** pentru verificare și aprofundare.

**Exercițiu:** [https://www.geeksforgeeks.org/fork-practice-questions/](https://www.geeksforgeeks.org/fork-practice-questions/)

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/ex3_fork_pids.png?raw=true)

**Exercițiu:** Folosind pipe se creează o comunicație unidirecțională între procese. Să se implementeze o comunicație bidirecțională între două procese (folosind două pipe-uri).

**Exercițiu:** forks.c

Să se studieze următoarea aplicație:
http://www.cs.cmu.edu/afs/cs/academic/class/15213-f01/src/forks.c

