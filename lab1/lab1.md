# Introducere în system programming

- [Introducere în system programming](#introducere-în-system-programming)
  * [GCC](#gcc)
  * [Utilizare GCC](#utilizare-gcc)
  * [Opţiuni](#Opţiuni)
  * [Activarea avertismentelor](#Activarea-avertismentelor)
  * [Alte opţiuni](#Alte-opţiuni)
- [Compilarea din mai multe fişiere](#Compilarea-din-mai-multe-fişiere)
- [Preprocesorul. Opţiuni de preprocesare](#Preprocesorul-Opţiuni-de-preprocesare)
- [Opţiuni pentru preprocesor la apelul gcc](#Opţiuni-pentru-preprocesor-la-apelul-gcc)
- [Debugging folosind directive de preprocesare](#Debugging-folosind-directive-de-preprocesare)
- [Linker-ul. Opţiuni de link-editare. Biblioteci](#Linker-ul-Opţiuni-de-link-editare-Biblioteci)
- [Biblioteci](#biblioteci)
  * [Crearea de biblioteci](#crearea-de-biblioteci)
  * [Crearea unei biblioteci statice](#crearea-unei-biblioteci-statice)
  * [Crearea unei biblioteci partajate](#crearea-unei-biblioteci-partajate)
- [Resurse utile](#resurse-utile)

Laboratoarele de Sisteme de Operare au drept scop aprofundarea interfeţelor de programare oferite de sistemele de operare (system API). Laboratorul este un laborator de system programming. Un laborator va aborda un set de concepte şi va conţine un scurt breviar teoretic, o prezentare a API-ului asociat cu explicaţii şi exemple şi un set de exerciţii pentru acomodarea cu acesta. Pentru a oferi o arie de cuprindere cât mai largă, laboratoarele au ca suport familia de sisteme de operare Unix.

În cadrul acestui laborator (laboratorul de introducere), va fi prezentat mediului de lucru care va fi folosit în cadrul laboratorului de Sisteme de Operare. Laboratorul foloseşte ca suport de programare limbajul C/C++. Pentru GNU/Linux se va folosi suita de compilatoare GCC. De asemenea, pentru compilarea incrementală a surselor se va folosi GNU make (Linux). Exceptând apelurile de bibliotecă standard, API-ul folosit va fi POSIX.

## GCC

GCC este suita de compilatoare implicită pe majoritatea distribuţiilor Linux. GCC este unul din primele pachete software dezvoltate de organizaţia "Free Software Fundation" în cadrul proiectului GNU (Gnu's Not Unix).

GCC, acronimul de la "GNU Compiler Collection", este un compilator multi-frontend, multi-backend cu suport pentru limbajele C, C++, Objective-C, Fortran, Java, Ada, Go și D.

La numărul impresionant de limbaje de mai sus se adaugă şi numărul mare de platforme suportate atât din punctul de vedere al arhitecturii hardware (i386, alpha, vax, m68k, sparc, HPPA, arm, MIPS, PowerPC, etc.) cât şi al sistemelor de operare (GNU/Linux, DOS, Windows 9x/NT/2000, *BSD, Solaris, Tru64, VMS, etc.). La ora actuală, GCC-ul este cel mai portat compilator.

În cadrul laboratoarelor de Sisteme de Operare ne vom concentra asupra facilităţilor oferite de compilator pentru limbajul C/C++. GCC suportă standardele ANSI, ISO C, ISO C99, ... inclusiv C++23, POSIX dar şi multe extensii folositoare care nu sunt incluse în nici unul din standarde. Vezi standarde C/C++: https://gcc.gnu.org/projects/cxx-status.html

## Utilizare GCC

O vedere de ansamblu asupra procesului de compilare este prezentată în imaginea de mai jos. 


![Image](https://github.com/rdemeter/so/blob/master/lab1/fig1.png?raw=true)

- Analiza lexicală: textul sursă este preluat sub forma unei secvențe de caractere care sunt grupate apoi în entități numite atomi; atomilor li se atribuie coduri lexicale, astfel ca, la ieșirea acestei faze, programul sursă apare ca o secvența de asemenea coduri. Exemple de atomi: cuvinte cheie, identificatori, constante numerice, semne de punctuație etc. 
- Analiza sintactică: are ca scop gruparea atomilor rezultați în urma analizei lexicale în structuri sintactice. O structură sintactică poate fi văzută ca un arbore ale carui noduri terminale reprezintă atomi, în timp ce nodurile interioare reprezintă șiruri de atomi care formează o entitate logică. Exemple de structuri sintactice: expresii, instrucțiuni, declarații etc.\
Pe durata analizei sintactice, de obicei are loc și o analiză semantică, ceea ce înseamnă efectuarea unor verificări legate de: 
    - compatibilitatea tipurilor datelor cu operațiile în care ele sunt implicate 
    - respectarea regulilor de vizibilitate impuse de limbajul sursă.
- Generarea de cod intermediar: în această fază are loc transformarea arborelui sintactic într-o secvență de instrucțiuni simple, similare macroinstrucțiunilor unui limbaj de asamblare.
- Optimizarea de cod: este o fază opțională, al cărei rol este modificarea unor porțiuni din codul intermediar generat, astfel încât programul rezultat să satisfacă anumite criterii de performanță vizând timpul de execuție și/sau spațiul de memorie ocupat.

- Generarea codului final: presupune transformarea instrucțiunilor codului intermediar (eventual optimizat) în instrucțiuni mașină (sau de asamblare) pentru calculatorul țintă (cel pe care se va executa programul compilat).\
\
În afară de acțiunile enumerate mai sus, procesul de compilare mai include următoarele: 
- Gestionarea tabelei de simboluri: tabela de simboluri (TS) este o structură de date destinată păstrării de informații despre simbolurile (numele) care apar în programul sursă; compilatorul face referire la aceasta tabelă aproape în toate fazele compilării. 
- Tratarea erorilor: un compilator trebuie să fie capabil să recunoască anumite categorii de erori care pot să apară în programul sursă; tratarea unei erori presupune detectarea ei, emiterea unui mesaj corespunzator şi revenirea din eroare, adică, pe cât posibil, continuarea procesului de compilare până la epuizarea textului sursă, astfel încât numarul de compilări necesare eliminării tuturor erorilor dintr-un program să fie cât mai mic. Practic, există erori specifice fiecarei faze de compilare.

GCC foloseşte pentru compilarea de programe C/C++ comanda gcc, respectiv g++. O invocare tipică este pentru compilarea unui program dintr-un singur fişier sursă:
Exemplu hello.c
```c
#include <stdio.h>
int main() {
    printf("Hello, world!\n);
    return 0;
}
```
```console
$ gcc hello.c
$ ./a.out
Hello, world!
```
Comanda gcc hello.c a fost folosită pentru compilarea fişierului sursă hello.c. Rezultatul a fost obţinerea executabilului a.out care a fost rulat.\
Executabilul a.out este executabilul implicit obţinut de gcc. Dacă se doreşte obţinerea unui executabil cu alt nume se poate folosi opţiunea -o:
```console
$ gcc hello.c -o hello
$ ./hello
Hello, world!
```
Comanda de mai sus a produs executabilul hello.
La fel se poate folosi g++ pentru compilarea unui fişier sursă C++:
```console
$ g++ hello.cpp -o hello_cpp
$ ./hello_cpp
Hello, world!
```
## Opţiuni
După cum s-a observat, la o rulare a comenzii gcc/g++ se obţine din fişierul sursă un executabil. Folosind diverse opţiuni, putem opri compilarea la una din fazele intermediare:
- -E - se realizează doar preprocesarea fişierului sursă
    - gcc -E hello.c , se va obține fișierul preprocesat hello.i
- -S - se realizează inclusiv faza de compilare
    - gcc -S hello.c , se va obține fișierul în limbaj de asamblare hello.s
- -c - se realizează inclusiv faza de asamblare
    - gcc -c hello.c , se va obține fișierul obiect hello.o

La opţiunile de mai sus se poate folosi opţiunea -o pentru specificarea fişierului de ieşire:
```
$ gcc -c hello.c -o my_obj_hello.o
```
## Activarea avertismentelor
În mod implicit, o rulare a gcc oferă puţine avertismente utilizatorului. Pentru a activa afişarea de avertismente se foloseşte opţiunea -W cu sintaxa -Woptiune-warning. optiune-warning poate lua mai multe valori posibile printre care return-type, switch, unused-variable, uninitialized, implicit, all. Folosirea opţiunii -Wall înseamnă afişarea tuturor avertismentelor care pot cauza inconsistenţe la rulare.
Considerăm ca fiind indispensabilă folosirea opţiunii -Wall pentru a putea detecta încă din momentul compilării posibilele erori. O cauză importantă a apariţiilor acestor erori o constituie sintaxa foarte permisivă a limbajului C. Sperăm ca exemplul de mai jos să justifice utilitatea folosirii opţiunii -Wall:

Exemplu 1. intro-01.c
```c
#include <stdio.h>

int main() {
    printf("1 + 2 fac %d\n", suma(1, 2));
}

int suma(int a, int b, int c) {
    return a + b + c;
}
```
În exemplul mai sus, programatorul a uitat că funcţia definită de el pentru adunare primeşte trei parametri şi nu doi. Dacă programul se compilează fără opţiunea -Wall, nu se vor genera erori sau avertismente, dar rezultatul nu va fi cel aşteptat:
```console
$ gcc intro-01.c
$ ./a.out
1+2 fac -1073743413
```
Programul s-a compilat fără erori, pentru că funcţia suma a fost declarată implicit de compilator (în C, în mod normal, funcţiile trebuie să fie declarate înainte de a fi folosite). O funcţie declarat implicit are prototipul:
```c
int function(...);
```
În prototipul de mai sus se poate recunoaşte operatorul ... (se citeşte elipses) care precizează faptul că funcţia are un număr variabil de parametri. Dacă se compilează acelaşi program folosind optiunea -Wall, programatorul va avea cel puţin ocazia să afle că funcţia a fost declarată implicit (şi, în cazul de faţă, şi faptul că a uitat să întoarcă un rezultat din funcţia main):
```console
$ gcc intro-01.c -Wall
exemplul-1.c: In function `main':
exemplul-1.c:5: warning: implicit declaration of function `suma'
exemplul-1.c:6: warning: control reaches end of non-void function
```
Soluţia este crearea unei declaraţii pentru funcţia suma şi apelul corespunzător al acesteia:

Exemplu 2. intro-02.c
```c
#include <stdio.h> 
int suma(int a, int b, int c);

int main() {
    printf("1 + 2 fac %d\n", suma(1, 2, 0));
return 0;
}

int suma(int a, int b, int c) {
    return a + b + c;
}
```
```console
$ gcc -Wall intro-02.c
$ ./a.out
1 + 2 fac 3
```
Exemplul prezentat oferă doar una din erorile posibile pe care GCC le detectează atunci când se foloseşte optiunea -Wall. În concluzie, folosiți opţiunea -Wall. În aceeaşi categorie, mai există opţiunea -Wextra (echivalent cu opţiunea -W), mult mai agresivă.

## Alte opţiuni

Alte opţiuni utile sunt:
- -Lcale - această optiune instruieşte compilatorul să caute și în directorul cale bibliotecile pe care trebuie să le folosească programul; opţiunea se poate specifica de mai multe ori, pentru a adăuga mai multe directoare
- -lbiblioteca - instruieşte compilatorul că programul are nevoie de biblioteca biblioteca. Fişierul ce conţine biblioteca va fi denumit libbiblioteca.so sau libbiblioteca.a.
- -Icale - instruieşte compilatorul să caute fişierele antet (headere) şi în directorul cale; opţiunea se poate specifica de mai multe ori, pentru a adăuga mai multe directoare
- -Onivel-optimizări - instruieşte compilatorul ce nivel de optimizare trebuie aplicat; 
    - -O0 va determina compilatorul să nu optimizeze codul generat;
    - -O3 va determina compilatorul să optimizeze la maxim codul generat;
    - -O2 este pragul de unde compilatorul va începe să insereze direct în cod funcţiile inline în loc să le apeleze;
    - -Os va optimiza programul pentru a reduce dimensiunea codului generat, și nu pentru viteză.
- -g - dacă se foloseşte această opţiune compilatorul va genera în fişierele de ieşire informaţii care pot fi apoi folosite de un debugger (informaţii despre fişierele sursă şi o mapare între codul maşină şi liniile de cod ale fişierelor sursă)

Paginile de ajutor ale GCC (man gcc, info gcc) oferă o listă cu toate opţiunile posibile ale GCC.


# Compilarea din mai multe fişiere

Exemplele de până acum tratează programe scrise într-un singur fişier sursă. În realitate, aplicaţiile sunt complexe şi scrierea întregului cod într-un singur fişier îl face greu de menţinut şi greu de extins. În acest sens aplicaţia este scrisă în mai multe fişiere sursă denumite module. Un modul conţine, în mod obişnuit, funcţii care îndeplinesc un rol comun. 
Următoarele fişiere sunt folosite ca suport pentru a exemplifica modul de compilare a unui program provenind din mai multe fişiere sursă:

Exemplu 3. intro-03-main.c
```c
#include <stdio.h>
#include "intro-03-util.h"

int main(void) {
    f1 ();
    f2 ();

    return 0;
}
```
Exemplu 3. intro-03-util.h
```c
#ifndef _INTRO_03_UTIL_H
#define _INTRO_03_UTIL_H   1

void f1(void);
void f2(void);

#endif
```
Exemplu 3. intro-03-f1.c
```c
#include "intro-03-util.h"
#include <stdio.h>

void f1(void) {
    printf("Fisierul curent este %s\n", __FILE__);
}
```
Exemplu 3. intro-03-f2.c
```c
#include "intro-03-util.h"
#include <stdio.h>

void f2(void) {
    printf("Va aflati la linia %d din fisierul %s\n", __LINE__, __FILE__);
}
```
În programul de mai sus se apelează, respectiv, funcţiile f1 şi f2 în funcţia main pentru a afişa diverse informaţii. Pentru compilarea acestora se transmit toate fişierele C ca argumente comenzii gcc:
```console
$ gcc -Wall intro-03-main.c intro-03-f1.c intro-03-f2.c -o intro-03
$ ./intro-03
Fisierul curent este intro-03-f1.c
Va aflati la linia 5 din fisierul intro-03-f2.c
```
Executabilul de ieşire a fost denumit intro-03; pentru acest lucru s-a folosit opţiunea -o. 

Se observă folosirea fişierului header intro-03-util.h pentru declararea funcţiilor f1 şi f2. Declararea unei funcţii se realizează prin precizarea antetului. Fişierul header este inclus în fişierul intro-03-main.c pentru ca acesta să aibă cunoştinţă de formatul de apel al funcţiilor f1 şi f2. Funcţiile f1 şi f2 sunt definite, respectiv, în fişierele intro-03-f1 şi intro-03-f2. Codul acestora este integrat în executabil în momentul link-editării.

În general în obţinerea unui executabil din surse multiple se obişnuieşte compilarea fiecărei surse până la modulul obiect şi apoi link-editarea acestora:
```console
$ gcc -Wall -c intro-03-f1.c
$ gcc -Wall -c intro-03-f2.c
$ gcc -Wall -c intro-03-main.c
$ gcc intro-03-f1.o intro-03-f2.o intro-03-main.o -o intro-03-m
$ ./intro-03-m
Fisierul curent este intro-03-f1.c
Va aflati la linia 5 din fisierul intro-03-f2.c
```
Se observă obţinerea executabilului intro-03-m prin legarea modulelor obiect. Această abordare are avantajul eficienţei. Dacă se modifică fişierul sursă intro-03-f2.c atunci doar acesta va trebui compilat şi refăcută link-editarea. Dacă s-ar fi obţinut un executabil direct din surse atunci s-ar fi compilat toate cele trei fişiere şi apoi refăcută link-editarea. Timpul consumat ar fi mult mai mare, în special în perioada de dezvoltare când fazele de compilare sunt dese şi se doreşte compilarea doar a fişierelor sursă modificate.

Scăderea timpului de dezvoltare prin compilarea numai a surselor care au fost modificate este motivaţia de bază pentru existenţa utilitarelor de automatizare precum **make** sau **nmake**.

Un lucru important în utilizarea header-elor pentru aplicaţii cu mai multe fişiere este folosirea directivelor de procesare #ifndef, #define, #endif prezentate în secţiunea următoare. Un fişier header tipic va avea structura:
```c
#ifndef _NUME_HEADER_H	/* numele fisierului header scris cu majuscule */
#define _NUME_HEADER_H	1

/* includere de alte header-e */
/* macrodefiniţii */
/* declaraţii de structuri */
/* declaraţii de funcţii */

#endif
```
Aceste directive de preprocesare au rolul de a proteja declaraţiile din header în cazul în care acesta este inclus de mai multe ori. Astfel, la prima includere nu va fi definit _NUME_HEADER_H (#ifndef), drept pentru care se defineşte _NUME_HEADER_H (#define) şi se prelucrează diversele declaraţii. La următoarea includere _NUME_HEADER_H va fi deja definit (#ifndef) şi nu va mai fi prelucrată partea de declaraţii, evitându-se astfel generarea unor erori de genul "multiple declaration". De remarcat că, pentru fişiere antet diferite este necesar ca simbolurile declarate la început, după modelul de mai sus, să fie diferite.

Directivele de preprocesare \_\_FILE\_\_ şi \_\_LINE\_\_ sunt expandate de preprocesor la numele fişierului, respectiv numărul liniei. Asemănător sunt expandate \_\_DATE\_\_ in formatul „MMM DD YYY” şi \_\_TIME\_\_ in formatul „HH:MM:SS”

# Preprocesorul. Opţiuni de preprocesare

Preprocesorul este prima componentă apelată în momentul folosirii comenzii gcc. Preprocesorul pe distribuţiile Linux este GNU CPP. După CPP se apelează compilatorul efectiv (GCC), apoi asamblorul (GAS) şi apoi linker-ul (GNU LD). Rolul CPP este acela de prelucrare a directivelor şi a operatorilor de preprocesare.

Directivele de preprocesare cele mai întâlnite sunt:
- #include pentru includerea de fişiere (de obicei header) într-un alt fişier
- #define, #undef folosite pentru definirea, respectiv anularea definirii de macrouri

```c
#define MY_MACRO 1	/* macro simplu */
#undef MY_MACRO

/* macro cu parametri; parantezele sunt importante! */
#define my_macro(a,b)	((a) + (b))

#define my_func_substit(a,b,c)	\	/* macro substituent de funcţie */
do {	\
  int i;	\
  c = 0;	\
  for (i = a; i < b; i++)	\
  c += i;	\
} while (0)	\
```
• #if, #ifdef, #ifndef, #else, #elif, #endif folosite pentru compilare condiţionată

```c
#define ON	1
#define OFF	0
#define DEBUG	ON

#if DEBUG == ON
/* C code ... do stuff */
#else
/* C code ... do some other stuff */
#endif
```
- \_\_FILE\_\_    , \_\_LINE\_\_ , \_\_func\_\_ sunt înlocuite cu numele fişierului, linia curentă în fişier şi numele funcţiei

- operatorul # este folosit pentru a înlocui o variabilă transmisă unui macro cu numele acesteia


Exemplu 4. intro-04.c
```c
#include <stdio.h>
#define expand_macro(a) printf ("variabila %s are valoarea %d\n", #a, a)

int main (void) {
    int my_uber_var = 12345;
    expand_macro (my_uber_var);
    return 0;
} 
```
```console
$ gcc -Wall intro-04.c
$ ./a.out
variabila my_uber_var are valoarea 12345
```
- operatorul ## (token paste) este folosit pentru concatenarea între un argument al macrodefiniţiei şi un
alt şir de caractere sau între două argumente ale macrodefiniţiei.

# Opţiuni pentru preprocesor la apelul gcc

Preprocesorului îi pot fi transmise opţiuni prin parametri transmişi comenzii gcc. Pentru aceasta se pot utiliza opţiunile -I sau -D.

Opţiunea -I este utilă pentru a preciza locul în care se află fişierele incluse. Astfel, daca fişierul header
utils.h se află în directorul includes/, utilizatorul poate include fişierul în forma
```c
#include "utils.h"
```
dar va trebui să precizeze calea către fişier folosind opţiunea -I:
```
$ gcc -Iincludes [...]
```
Opţiunea -D este utilă pentru a defini macrouri în linia de comandă:
```
$ gcc -D DEBUG   [...]
$ gcc -D SIMPLE_MACRO=10 [...]	; echivalent cu #define SIMPLE_MACRO	10
```
Opţiunea -U este utilă pentru a anula definirea unui macro.

# Debugging folosind directive de preprocesare

De multe ori, un dezvoltator va dori să poată activa sau dezactiva foarte facil afişarea de mesaje suplimentare
(de informare sau de debug) în sursele sale. Metoda cea mai simplă pentru a realiza acest lucru este prin intermediul unui macro:
```c
#define DEBUG	1

#ifdef DEBUG
/* afisare mesaje debug */
#endif
```
Dacă se foloseşte opţiunea -D în linia de comandă, atunci definiţia macroului DEBUG poate fi eliminată:
```c
$ gcc -DDEBUG [...]
```
Folosirea perechii de directive #ifdef, #endif prezintă dezavantajul încărcării codului. Se poate încerca modularizarea afişării mesajelor de debug printr-o construcţie de forma:
```c
#ifdef DEBUG
#define Dprintf(msg)	printf(msg)
#else
#define Dprintf(msg)	/* do nothing */
#endif
```

Problema este folosirea mai multor argumente la printf. Acest lucru poate fi rezolvat cu \_\_VA_ARGS\_\_ și operatorul ##.

Exemplu: intro_debug.c, cu afişarea fişierului şi liniei unde s-a apelat macroul
```c
#include <stdio.h>
#ifdef DEBUG
#define Dprintf(fmt, ...) printf("%s:%d "fmt, __FILE__ , __LINE__ , ## __VA_ARGS__)
#else
#define Dprintf(fmt, ...)       /* do nothing */
#endif

int main()
{
    Dprintf(" mesaj1\n");
    Dprintf(" %s\n", "mesaj2");
    return 0;
}
````

Compilați intro_debug.c cu și fără -DDEBUG
```console
$gcc -Wall -DDEBUG intro_debug.c -o intro_debug
./intro_debug
./intro_debug.c:11 mesaj1
./intro_debug.c:12 mesaj2

$gcc -Wall intro_debug.c -o intro_debug
./intro_debug
```

# Linker-ul. Opţiuni de link-editare. Biblioteci

Linker-ul este folosit pentru a "unifica" mai multe module obiect şi biblioteci şi a obţine un executabil sau o bibliotecă. Linker-ul are rolul de a rezolva simbolurile nedefinite dintr-un modul obiect prin inspectarea celor existente într-un altul. Erorile de linker apar ca urmare a lipsei unui simbol, ca în exemplul de mai jos:

Exemplu 5. intro-05-main.c
```c
void f(void);

int main (void) {
    f();
    return 0;
}
```
Exemplu 5. intro-05-f.c
```c
#include <stdio.h>

void f(void) {
    printf ("Hello, World!\n");
}
```
```console
$ gcc -Wall intro-05-main.c
/tmp/ccVBU35X.o: In function `main':
intro-05-main.c:(.text+0x12): undefined reference to `f'
collect2: ld returned 1 exit status
$ gcc -Wall intro-05-main.c intro-05-f.c
$ ./a.out
Hello, World!
```
La o primă rulare a apărut eroare pentru că linker-ul nu a găsit funcţia f. Includerea intro-05-f.c în lista de fişiere compilate a rezolvat această problemă.

Linker-ul pe distribuţiile Linux este GNU LD. Executabilul asociat este ld. De obicei comanda gcc apelează în spate ld pentru a efectua link-editarea modulelor obiect. Opţiunile de linking sunt de obicei transmise comenzii gcc. Opţiunile cele mai utile sunt cele care sunt legate de biblioteci.

# Biblioteci

O bibliotecă este o colecţie de funcţii precompilate. În momentul în care un program are nevoie de o funcţie, linker-ul va apela respectiva funcţie din bibliotecă. Numele fişierului reprezentând biblioteca trebuie să aibă prefixul lib:
```console
$ ls -l /usr/lib/libm.*
-rw-r--r-- 1 root root 481574 Jul 30 23:41 /usr/lib/libm.a
lrwxrwxrwx 1 root root	14 Aug 25 20:20 /usr/lib/libm.so -> /lib/libm.so.6
```
Biblioteca matematică este denumită libm.a sau libm.so. În Linux bibliotecile sunt de două tipuri:
- statice, au de obicei, extensia .a
- dinamice, au extensia .so

Detalii despre crearea bibliotecilor se găsesc în secţiunea următoare.

Legarea se face folosind opţiunea -l transmisă comenzii gcc. Astfel, dacă se doreşte folosirea unor funcţii din math.h, trebuie legată biblioteca matematică:

Exemplu 6. intro-06.c
```c
#include <stdio.h>
#include <math.h>

#define RAD	(M_PI / 4)

int main (void) {
    printf ("sin = %g, cos = %g\n", sin (RAD), cos (RAD));
    return 0;
}
```
```console
$ gcc -Wall intro-06.c
/tmp/ccRqG57V.o: In function `main':
intro-06.c:(.text+0x1b): undefined reference to `cos'
intro-06.c:(.text+0x2c): undefined reference to `sin' collect2: ld returned 1 exit status
```
```console
$ gcc -Wall intro-06.c -lm
$ ./a.out
sin = 0.707107, cos = 0.707107
```
Se observă că, în primă fază, nu s-au rezolvat simbolurile cos şi sin. După legarea bibliotecii matematice, programul s-a compilat şi a rulat fără probleme.

## Crearea de biblioteci

Pentru crearea de biblioteci vom folosi exemplul 3. Vom include modulele obiect rezultate din fişierele sursă intro-03-f1.c şi intro-03-f2.c într-o bibliotecă pe care o vom folosi ulterior pentru obţinerea executabilului final.

Primul pas constă în obţinerea modulelor obiect asociate:
```console
$ gcc -Wall -c intro-03-f1.c
$ gcc -Wall -c intro-03-f2.c
```
## Crearea unei biblioteci statice

O bibliotecă statică este o arhivă ce conține fișiere obiect creată cu ajutorul utilitarului ar.
```console
$ ar rc libintro.a intro-03-f1.o intro-03-f2.o
$ gcc -Wall intro-03-main.c -o intro-lib -lintro
/usr/bin/ld: cannot find -lintro
collect2: ld returned 1 exit status
```
Aruncați o privire în pagina de manual a utilitarului ar și interpretați parametrii rc de mai sus.

Linker-ul returnează eroare precizând că nu găseşte biblioteca libintro. Aceasta deoarece linker-ul nu a fost configurat să caute şi în directorul curent. Pentru aceasta se foloseşte opţiunea -L, urmată de directorul în care trebuie căutată biblioteca (în cazul nostru este vorba de directorul curent):
```console
$ gcc -Wall intro-03-main.c -o intro-lib -lintro -L.
$ ./intro-lib
Fisierul curent este intro-03-f1.c
Va aflati la linia 5 din fisierul intro-03-f2.c
```

## Crearea unei biblioteci partajate

Spre deosebire de o bibliotecă statică despre care am văzut că nu este nimic altceva decât o arhivă de fișiere obiect, o bibliotecă partajată este ea insăși un fișier obiect. Crearea unei biblioteci partajate se realizează prin intermediul linker-ului. Opțiunea -shared indică compilatorului să creeze un obiect partajat și nu un fișier executabil. Este, de asemenea, indicată folosirea opţiunii -fPIC:
```console
$ gcc -shared -fPIC intro-03-f1.o intro-03-f2.o -o libintro_shared.so
$ gcc -Wall intro-03-main.c -o intro-lib -lintro_shared -L.
$ ./intro-lib
./intro-lib: error while loading shared libraries: libintro_shared.so:
cannot open shared object	file: No such file or directory
```
 
La rularea executabilului se poate observa că nu se poate încărca biblioteca partajată. Cauza este deosebirea dintre bibliotecile statice şi bibliotecile partajate. În cazul bibliotecilor statice codul funcţiei de bibliotecă este copiat în codul executabil la link-editare. De partea cealaltă, în cazul bibliotecilor partajate, codul este încărcat în memorie în momentul rulării.

Astfel, în momentul rulării unui program, loader-ul (programul responsabil cu încărcarea programului în memorie), trebuie să ştie unde să caute biblioteca partajată pentru a o încărca în memorie în cazul în care aceasta nu a fost încărcată deja. Loader-ul foloseşte câteva căi predefinite (/lib, /usr/lib, etc) şi de asemenea locaţii definite în variabila de mediu LD_LIBRARY_PATH:
```console
$ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
$ ./intro-lib
Fisierul curent este intro-03-f1.c
Va aflati la linia 5 din fisierul intro-03-f2.c
```
În exemplul de mai sus variabilei de mediu LD_LIBRARY_PATH i-a fost adăugată calea către directorul curent rezultând în posibilitatea rulării programului. LD_LIBRARY_PATH va rămâne modificată cât timp va rula consola curentă. Pentru a face o modificare a unei variabile de mediu doar pentru o instanță a unui program se face atribuirea noii valori înaintea comenzii de execuție:
```console
$ LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.  ./intro-lib
Fisierul curent este intro-03-f1.c
Va aflati la linia 5 din fisierul intro-03-f2.c
$ ./intro-lib
./intro-lib: error while loading shared libraries: libintro_shared.so:
cannot open shared object	file: No such file or directory
```
# Resurse utile
1. GCC Online Documentation
2. The C Preprocessor
3. GNU C Library
4. Program Library HOWTO
5. Using LD, the GNU Linker
6. GNU Make Manual
7. GDB Documentation
9. Nmake Tool
10. Building and Linking with Libraries
11. Dynamic Link Library
13. Dynamic Libraries

