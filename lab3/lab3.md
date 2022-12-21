
# Procese

- [Procese](#procese)
  * [Procese în Linux](#Procese-în-Linux)
  * [Crearea unui proces](#crearea-unui-proces)
  * [Aşteptarea terminării unui proces](#Aşteptarea-terminării-unui-proces)
  * [Terminarea unui proces](#terminarea-unui-proces)
- [Depanarea unui proces](#depanarea-unui-proces)
- [Pipe-uri](#pipe-uri)
  * [Pipe-uri anonime](#pipe-uri-anonime)
  * [Pipe-uri cu nume](#Pipe-uri-cu-nume)

Un concept cheie în orice sistem de operare este procesul. Un proces este un program în execuţie. Procesele formează unitatea primitivă prin care sistemul de operare alocă resurse utilizatorilor. Orice proces are un spaţiu de adrese şi unul sau mai multe fire de execuţie. Putem avea mai multe procese ce execută același program, dar oricare două procese sunt complet independente.

Spaţiile de adrese, regiștrii generali, PC (contor program), SP (indicator stivă), tabelele de fisiere deschise, lista de semnale (blocate, ignorate sau care asteaptă să fie trimise procesului), handler-ele pentru semnale, informatiile referitoare la sistemele de fișiere (directorul rădăcină, directorul curent), toate acestea NU sunt partajate, ci aparțin fiecărui proces în parte. Aceste informații necesare pentru rularea programului sunt ținute de sistemul de operare într-o structură numită Process Control Block, câte una pentru fiecare proces existent în sistem.

În momentul lansării în execuţie al unui program, în sistemul de operare se va crea un proces pentru alocarea resurselor necesare rulării programului respectiv. Fiecare sistem de operare pune la dispoziţie apeluri de sistem pentru crearea unui proces, terminarea unui proces, așteptarea terminării unui proces precum şi apeluri pentru duplicarea descriptorilor de resurse între procese ori închiderea acestor descriptori.

În general un proces rulează într-un mediu specificat printr-un set de variabile de mediu. O variabilă de mediu este o pereche NUME=valoare. Un proces poate să verifice sau să seteze valoarea unei variabile de mediu printr-o serie de apeluri de bibliotecă.

Pe sisteme de 32 de biți fiecare proces are un spaţiu de adrese de 4 GiB din care 2 (sau în anumite configuraţii 3) GiB sunt disponibili pentru alocare procesului, iar ceilalți 2 (respectiv 1) GiB fiind rezervat sistemului de operare (codul kernelului și al driverelor, date, cache-uri, etc.). Așadar fiecare proces "vede" sistemul de operare în spațiul său de adrese însă nu poate accesa zona respectivă decât prin intermediul apelurilor de sistem (comutând procesorul în modul de lucru privilegiat). Pe sisteme de 64 de biți spaţiul total de adrese este de 16 EiB, iar pe sisteme de 16 biți de doar 64 KiB (procesoarele x86 pe 16 biți puteau adresa 220 = 1 MiB de memorie folosind la adresare doi regiștri de 16 biți întrucât, deși era procesor pe 16 biți, avea 20 de linii de adresă).

## Procese în Linux

Apelurile de sistem puse la dispoziție de Linux pentru gestionarea proceselor sunt: fork  și exec pentru crearea unui proces și respectiv modificarea imaginii unui proces, wait și waitpid pentru așteptarea terminării unui proces și exit pentru terminarea unui proces. Pentru copierea descriptorilor de fișier Linux pune la dispoziţie apelurile de sistem dup și dup2. Pentru citirea, modificarea ori ștergerea unei variabile de mediu, biblioteca standard C pune la dispoziţie apelurile getenv, setenv, unsetenv precum şi un pointer la tabela de variabile de mediu environ.

Rularea unui program executabil

Modul cel mai simplu prin care se poate crea un nou proces este prin folosirea funcţiei de bibliotecă system:
```
int system(const char* command);
```
Apelul acestei funcţii are ca efect execuţia ca o comandă shell a comenzii reprezentate prin șirul de caractere command. Să luăm ca exemplu următorul program C:

Exemplu 1. sys1.c
```
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

În UNIX singura modalitate de creare a unui proces este prin apelul de sistem fork:
```
pid_t fork(void);
```
![image](https://github.com/rdemeter/so/blob/master/lab3/figs/fork_system_call.png)

Efectul este crearea unui nou proces - procesul copil, copie a celui care a apelat fork - procesul părinte. Copilul primeşte un nou PID de la sistemul de operare. Secvența clasică de creare a unui proces este prezentată în continuare:

Exemplu 3. ex1_fork.c
```
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
```
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
```
pid_t wait(int *status);
```
Este echivalentă cu: waitpid(-1, &status, 0);

Pentru a folosi wait sau waitpid trebuie incluse header-ele sys/types.h şi sys/wait.h.

## Terminarea unui proces

Pentru terminarea procesului curent, Linux pune la dispoziție apelul de sistem exit. 
```
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

Pe majoritatea sistemelor de operare pe care a fost portat, gdb nu poate detecta când un proces realizează o operație fork(). Atunci când programul este pornit, depanarea are loc exclusiv în procesul inițial, procesele copii nefiind atașate debugger-ului. În acest caz, singura soluție este introducerea unor întârzieri în execuția procesului nou creat (de exemplu, prin apelul de sistem sleep()), care să ofere programatorului suficient timp pentru a atașa manual gdb-ul la respectivul proces, presupunând că i-a aflat PID-ul în prealabil. Pentru a atașa debugger-ul la un proces deja existent, se folosește comanda attach, în felul următor:
```
(gdb) attach PID
```
Această metodă este destul de incomodă și poate cauza chiar o funcționare anormală a aplicației depanate, în cazul în care necesitățile de sincronizare între procese sunt stricte (de exemplu operații cu time-out).

Din fericire, pe un număr limitat de sisteme, printre care si Linux, gdb permite depanarea comodă a programelor care creează mai multe procese prin fork() și vfork(). Pentru ca gdb să urmărească activitatea proceselor create ulterior, se poate folosi comanda set follow-fork-mode, în felul următor:
```
(gdb) set follow-fork-mode mode
```
unde mode poate lua valoarea parent, caz în care debugger-ul continuă depanarea procesului părinte, sau valoarea child, și atunci noul proces creat va fi depanat în continuare. Se poate observa că în această manieră debugger-ul este atașat la un moment dat doar la un singur proces, neputând urmări mai multe simultan.

Cu toate acestea, gdb poate ține evidența tuturor proceselor create de către programul depanat, deși în continuare numai un singur proces poate fi rulat prin debugger la un moment dat. Comanda set detach-on-fork realizează acest lucru:
```
(gdb) set detach-on-fork mode
```
unde mode poate fi on, atunci când gdb se va atașa unui singur proces la un moment dat (comportament implicit), sau off, caz în care gdb se atașează la toate procesele create în timpul execuției, și le suspendă pe acelea care nu sunt urmărite, în funcție de valoarea setării follow-fork-mode.

Comanda info forks afișează informații legate de toate procesele aflate sub controlul gdb la un moment dat:
```
(gdb) info forks
```
De asemenea, comanda fork poate fi utilizată pentru a seta unul din procesele din listă drept cel activ (care este urmărit de debugger).
```
(gdb) fork fork-id
```
unde fork-id este identificatorul asociat procesului, așa cum apare în lista afișată de comanda info forks.

Atunci când un anumit proces nu mai trebuie urmărit, el poate fi înlăturat din listă folosind comenzile detach fork şi delete fork:
```
(gdb) detach fork fork-id
(gdb) delete fork fork-id
```
Diferenţa dintre cele două comenzi este că detach fork lasă procesul să ruleze independent, în continuare, în timp ce delete fork îl încheie.

Pentru a ilustra aceste comenzi într-un exemplu concret, să considerăm programul următor:

Exemplu 5. forktest.c
```
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

# Pipe-uri

Pipe-urile (canalele de comunicaţie) sunt mecanisme primitive de comunicare între procese. Un pipe poate conţine o cantitate limitată de date. Accesul la aceste date este de tip FIFO (datele se scriu la un capăt al pipe-ului şi sunt citite de la celălalt capăt). Sistemul de operare garantează sincronizarea între operaţiile de citire şi scriere la cele două capete.

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/pipe.png)

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
```
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
```
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
```
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

Exercițiu: test.c
Scrieți secvența afișată după rularea următorului program:
```
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
![image](https://github.com/rdemeter/so/blob/master/lab3/figs/ex1_fork_pids.png)

Exercitiu: [https://www.geeksforgeeks.org/fork-practice-questions/](https://www.geeksforgeeks.org/fork-practice-questions/)

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/ex2_fork_pids.png)

Exercitiu: [https://www.geeksforgeeks.org/fork-practice-questions/](https://www.geeksforgeeks.org/fork-practice-questions/)

![image](https://github.com/rdemeter/so/blob/master/lab3/figs/ex3_fork_pids.png)


Exercițiu: Folosind pipe se creează o comunicație unidirecțională între procese. Să se implementeze o comunicație bidirecțională între două procese (folosind două pipe-uri).

Exercițiu: forks.c
Să se studieze următoarea aplicație:
http://www.cs.cmu.edu/afs/cs/academic/class/15213-f01/src/forks.c

