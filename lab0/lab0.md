# Shell scripting

- [Shell scripting](#shell-scripting)
  * [Cel mai simplu script shell](#cel-mai-simplu-script-shell)
- [Operatori shell](#operatori-shell)
  * [Concatenarea comenzilor](#concatenarea-comenzilor)
  * [Înlănțuirea comenzilor](#Înlănțuirea-comenzilor)
  * [Redirectări](#Redirectări)
- [Variabile](#variabile)
- [Argumente în linia de comandă](#Argumente-în-linia-de-comandă)
- [Caractere speciale](#caractere-speciale)
  * [spațiu](#spa-iu)
  * [backslash](#backslash)
  * [ghilimele](#ghilimele)
  * [apostrof](#apostrof)
  * [dollar - expansion](#dollar---expansion)
    + [expandarea unui parametru](#expandarea-unui-parametru)
    + [substitutia unei comenzi](#substitutia-unei-comenzi)
    + [expansiune aritmetică](#expansiune-aritmetic-)
- [Structuri de control](#structuri-de-control)
  * [if](#if)
  * [case](#case)
  * [for](#for)
  * [while](#while)
- [Functii shell](#functii-shell)
- [Pattern matching](#pattern-matching)
- [Comenzi utile](#comenzi-utile)
  * [echo](#echo)
  * [cat, tac, head, tail](#cat--tac--head--tail)
  * [read](#read)
  * [find](#find)
  * [dd](#dd)
- [Filtre de text](#filtre-de-text)
  * [head, tail](#head--tail)
  * [grep](#grep)
  * [tr](#tr)
  * [sort](#sort)
  * [uniq](#uniq)
  * [wc](#wc)
  * [cut](#cut)
- [Exemple](#exemple)
- [Exerciţii](#Exerciţii)
- [Link-uri utile](#link-uri-utile)

Shell-ul este principala interfață de comunicare între utilizator și sistemul de operare. Deși, în mod intuitiv, shell-ul este identificat cu o interfață în linia de comandă, poate fi și o interfaţă grafică. Exemplu este File Explorer-ul sistemului de operare Microsoft Windows.

În cele ce urmează vom studia interfaţa de tip CLI (Command Line Interface) oferită de sistemele de operare Unix. Deși cu o curbă de învăţare mai mare decât o interfaţă grafică, CLI permite un control mult mai bun al sistemului. Mai mult, shell-ul dispune de un limbaj de programare. Un program shell, denumit script shell, este folosit pentru a îmbina mai multe comenzi și diverse structuri de control pentru a obţine o nouă funcţionalitate sau pentru automatizarea sarcinilor. În acest fel un script shell este un instrument esenţial pentru sarcinile administrative și alte rutine repetitive care nu necesită funcţionalităţi ale unor limbaje de programare avansate.

În continuare ne vom referi la Bash (Bourne Again SHell). Există și alte shell-uri pe sisteme Unix precum tcsh, zsh, ash, etc. De curând, Microsoft oferă PowerShell pe sistemele Windows. PowerShell are o abordarea orientată pe obiecte și un set de funcţionalităţi care acoperă nevoile de administrare ale unui sistem Windows.

## Cel mai simplu script shell

Un script simplu care doar afișează mesajul "Hello, World!" este următorul:

```
#!/bin/bash

#afiseaza mesaj
echo "Hello, World!"
exit 0
```
Execuţia acestuia este următoarea:
```
$ chmod +x hello.sh
$ ./hello.sh
Hello, World!
```
Se observă că este necesar ca fișierul să fie executabil pentru a putea fi interpretat. Șirul #! de la începutul fișierului poartă denumirea de shebang. Acesta indică sistemului ce program va fi invocat pentru a interpreta scriptul. Exemple pot fi:
```
#!/bin/sh
#!/bin/bash
#!/usr/bin/perl
#!/usr/bin/python
#!/usr/bin/awk -f
```
Spre exemplu, următorul script se șterge pe sine:
```
#!/bin/rm -f

aici putem scrie orice ... oricum se va șterge
```
Un script poate fi rulat prin precizarea explicită a interpretorului în linia de comandă:

```
$ bash hello.sh
Hello, World!
```

În această situaţie nu este nevoie ca scriptul sa fie executabil și nici nu este nevoie de prezenţa liniei #!/bin/bash.

Caracterul # semnifică începutul unui comentariu care durează pană la sfarșitul liniei.

Comanda exit este folosită pentru a indica valoarea de retur a scriptului. Este implicit 0 (cu alte cuvinte nu era necesar să apară în script).


# Operatori shell

Shell-ul prezintă o serie de operatori folositi pentru îmbinarea comenzilor.


## Concatenarea comenzilor

Următorii operatori sunt folosiți pentru concatenarea diverselor comenzi:

-	command1 ; command2 - comenzile sunt executate secvenţial (una dupa alta)
-	command1 && command2 - command2 este executată numai dacă command1 are valoare de retur 0
-	command1 || comand2 - command2 este executată numai dacă command1 are valoare de retur diferita de 0


## Înlănțuirea comenzilor

Inlănțuirea comenzilor se realizează folosind operatorul | (pipe). În această situatie ieșirea unei comenzi devine intrarea pentru cealaltă comandă.

Câteva exemple sunt prezentate în continuare:
```
$ last -30 | grep Tue							
razvan	pts/2	:0.0	Tue Jan	2	20:42 - down	(05:12)
razvan	pts/2	:0.0	Tue Jan	2	20:35 - 20:41	(00:06)
razvan	pts/1	:0.0	Tue	Jan	2	20:34 -	21:23	(00:48)
razvan	pts/0	:0.0	Tue	Jan	2	20:27 -	down	(05:27)
wtmp begins Tue Nov 14 04:22:33
```

```
$ last -30 | grep Tue | tr -s '	'	
razvan pts/2 :0.0 Tue Jan 2 20:42	- down (05:12)
razvan pts/2 :0.0 Tue Jan 2 20:35	- 20:41 (00:06)
razvan pts/1 :0.0 Tue Jan 2 20:34	- 21:23 (00:48)
razvan pts/0 :0.0 Tue Jan 2	20:27	- down (05:27)
wtmp begins Tue Nov 14 04:22:33
```

```
$ last -30 | grep Tue | tr -s '	'	| head -4
razvan pts/2 :0.0 Tue Jan 2	20:42	- down (05:12)
razvan pts/2 :0.0 Tue Jan 2	20:35	- 20:41 (00:06)
razvan pts/1 :0.0 Tue Jan 2 20:34 - 21:23 (00:48)
razvan pts/0 :0.0 Tue Jan 2 20:27 - down (05:27)
```

```
$ last -30 | grep Tue | tr -s ' ' | head -4 | cut -d ' ' -f 2
pts/2
pts/2
pts/1
pts/0
```

```
$ last -30 | grep Tue | tr -s ' ' | head -4 | cut -d ' ' -f 2 | uniq
pts/2
pts/1
pts/0
```

```
$ last -30 | grep Tue | tr -s ' ' | head -4 | cut -d ' ' -f 2 | uniq | wc -l 3
```

## Redirectări

Comenzilor le pot fi redirectate, respectiv, intrarea standard, ieșirea standard și eroarea standard dintr-un fișier. O parte din operatorii folosiți pentru redirectare sunt:

-	\> - redirectarea ieșirii standard
-	2> - redirectarea erorii standard
-	2>&1 - redirectarea erorii standard in ieșirea standard. Efectiv, unificarea stderr cu stdout.
-	< - redirectarea intrării standard

Exemple:
```
$ ls -l > ls.out
```
```
$ strace ls 2> strace.out
```
```
$ grep "alpha" < file.txt
```

# Variabile

Ca orice limbaj, shell-ul permite utilizarea de variabile. Spre deosebire de limbajele cunoscute, variabilele shell nu au tipuri. O variabila poate fi evaluată atât ca număr cât și ca șir de caractere.

Exemple de definire de variabile:

```
var1=2
var2=4asa
var3='abcd'
my_var="asdf"
my_other_var="a 1 3 4"
new_var=$var1
new_var2=${var2}var3
```
ATENTIE! Sintaxa shell este foarte strictă; NU este permis sa existe spatii intre numele variabilei si caracterul = sau intre caracterul = si valoarea variabilei.

Se observă ca valoarea unei variabile este referită prin folosirea simbolului $.

Exemple de folosire de variabile:
```
$ echo $var1
2
```
```
$ echo $var12
```
```
$ echo ${var1}2
22
```
```
$ echo "$var1"
2
```
```
$ echo "$var1"2
22
```
```
$ echo $var1$my_other_var
2a 1 3 4
```
```
$ echo "$var1 $my_other_var"
2 a 1 3 4
```

# Argumente în linia de comandă

Un script poate primi argumente în linia de comandă. Argumentele sunt referite respectiv folosind parametrii poziționali: $1, $2, ... $0 este numele scriptului (echivalent cu argv[0] din C).

Numărul de argumente din linia de comandă este dat de '$#'. '$#' va fi 0 daca nu avem argumente in linia de comandă (echivalentul C - argc - ar fi avut valoarea 1 in acest caz).

$@ poate fi folosit pentru obținerea întregii liste de argumente separate prin spațiu.

Exemplu:
```
#!/bin/sh
echo "Scriptul are $# parametri"
echo "Numele scriptului este $0"

if test $# -ge 1; then
    echo "Primul parametru este $1"
fi

if test $# -ge 2; then
    echo "Cel de-al doilea parametru este $2"
fi

echo "Lista de parametri este $@"
```
Rularea scriptului este:
```
$ chmod +x test.bash
$ ./test.bash alfa "beta gamma"
Scriptul are 2 parametri
Numele scriptului este ./test.bash
Primul parametru este alfa
Cel de-al doilea parametru este beta gamma
Lista de parametri este alfa beta gamma
```

# Caractere speciale

Un set de caractere au semnificație specială in cadrului shell-ului.

## spațiu

Caracterul spațiu (blank) este separator pentru argumentele in linia de comandă sau pentru elementele unei liste. Dacă se dorește transmiterea ca parametru a unui argument ce conține spatiu acesta trebuie citat (quoted):
```
$ ls my\ dir
$ ls "my dir"
$ ls 'my dir'
```

## backslash

Caracterul **backslash** forțeaza caracterul ce-l precede să-și păstreze semnificația literală; cu alte cuvinte, este folosit pentru a intârzia (a escapa) acel caracter:

```
$ echo $var1
test
```
```
$ echo \$var1
$var1
```

## ghilimele

Un șir intre ghilimele (double quotes ") va păstra semnificația literală a caracterelor ce-l compun cu excepția caracterelor ' (apostrof) și $ (dollar). Caracterul \ (backslash) își păstrează semnificația specială numai în cazul în care este urmat de $, ', ``, \ sau newline.


## apostrof

Un șir între caractere apostrof (single quotes) va păstra semnificatia literală a tuturor caracterelor ce-l compun (nu există excepții).

## dollar - expansion

Caracterul dollar este folosit in mai multe situatii in ceea ce se numește expansion. Este folosit pentru a recupera valoarea unei variabile, pentru a stoca într-o variabila ieșirea unei funcții, etc.

### expandarea unui parametru

Se înlocuiește un parametru (variabila) sau se înlocuiesc parametrii poziționali. $? se traduce in valoarea de retur a ultimei comenzi executate.
```
$ ls -l | grep 126
-rwxr-xr-x 1 razvan razvan 126 2007-01-06 21:42 pos2.sh 
$ echo $?
0
```
```
$ ls -l | grep 1267 
$ echo $?
1
```

### substitutia unei comenzi

Ieșirea unei comenzi inlocuiește comanda efectivă:
```
$ ls -l | wc -l 11
$ var=$(ls -l | wc -l)
$ echo $var
11
```

### expansiune aritmetică

Se realizează evaluarea unei expresii aritmetice cu furnizarea rezultatului:
```
$ num="1+2*3"
$ echo $num
1+2*3
$ num=$((1+2*3))
$ echo $num
7
```
# Structuri de control

Ca orice limbaj de programare, shell-ul are un set de structuri de control pentru a permite lucrul cu cicluri și cu decizii.


## if

Sintaxa pentru **if** este următoarea:
```
if CONDITIE1; then
    comenzi
elif CONDITIE2; then
    comenzi
else
    comenzi
fi
```
Structurile **elif** și **else** sunt opționale.

Condiția poate aparea în două formate:

[ conditie_efectiva ]

sau

test conditie_efectiva

Pentru condițiile posibile consultați pagina de manual a utilitarului test.

ATENTIE! Dacă folosiți prima variantă este nevoie de spatiu după [ si inainte de ].

Exemple:
```
if test $i -eq 3; then
    echo "valoarea este 3"
else
    echo "valoarea este diferita de 3"
fi

if test "$var" = "alfa" -o "$var" = "beta"; then
    echo "valoarea este alfa sau beta"
fi

if test -f "$i"; then
    echo "$i este un fisier"
elif test -d "$i"; then
    echo "$i este un director"
else
    echo "$i nu este nici fisier nici director"
fi
```

## case

Sintaxa pentru **case** este următoarea:
```
case VARIABILA in
    pattern1) comenzi ;;
    pattern2) comenzi ;;
esac
```
Se poate folosi ca pattern * pe post de default.

Exemplu:
```
#!/bin/sh
echo -n "Enter the name of an animal: "
read ANIMAL
echo -n "The $ANIMAL has "

case $ANIMAL in
    horse | dog | cat) echo -n "four";;
    man | kangaroo ) echo -n "two";;
    *) echo -n "an unknown number of";;
esac

echo " legs."
```

## for

Sintaxa pentru **for** este următoarea:
```
for VARIABILA in LISTA; do
    comenzi
done
```
LISTA este o inșiruire de elemente separate prin spații. Variabila va lua pe rând aceste valori. Dacă se doresc variabile numerice în stilul C se poate folosi construcția (( ... )).

Exemple:
```
for i in *; do
    if test -f "$i"; then
        echo "$i este fisier"
    fi
done
```
```
for i in "1 2 3"; do
    sum=$(($sum + $i))
done

echo "suma este $sum"
```
```
for i in $(seq 1 10); do
    sum=$(($sum + $i))
done

echo "suma primelor 10 numere este $sum"
```
```
for ((i = 0; i < 100; i += 2)); do
    sum=$(($sum + $i))
done

echo "suma numerelor pare pana la 100 este $sum"
```

## while

Sintaxa pentru **while** este următoarea:
```
while CONDITIE; do
    comenzi
done
```
Conditia are același format ca la if.

Exemplu:
```
i=1
prod=1
while test $i -le 30; do
    prod=$(($prod * $i))
    i=$(($i + 5))
done

echo "produsul este $prod"
```

# Functii shell

Ca orice alt limbaj de programare, shell-ul permite lucrul cu funcții proceduri. Sintaxa unei funcții este:
```
[function] nume_functie ()
{
    comenzi
}
```
Identificatorul function este optional. Sintaxa de apel este simplă: numele funcției urmat de eventualii parametri.

ATENTIE! In cadrul unei funcții argumentele vor fi referite tot ca parametri pozitionali ($1, $2, ...) astfel incât dacă dorim să referim într-o funcție argumentele în linia de comandă va trebui să-i transmitem ca parametri la apelul funcției.

Exemplu:
```
#!/bin/sh
function fun1 ()
{
    echo "Aceasta este functia fun1"
}

function fun2 ()
{
    echo "Aceasta este functia fun2"
    echo "Argumentele functiei sunt $1 $2"
}

fun1
fun2 a b
fun2 alfa
```

# Pattern matching

In interactiunea cu sistemul de fișiere se dorește selectarea rapidă a mai multor fișiere după câteva caracteristici de nume comune. Operația efectuată de shell se numește pattern matching. Există următoarele caractere speciale:

-	'*' - se potriveste cu orice șir de caracter, inclusiv șirul vid
-	? - se potriveste cu un singur caracter
-	[...] - se potriveste cu unul din caracterele din set; poate fi de genul [abc] sau [a-gA-G] sau [0-5h-zH-Z]

Pentru următoarele pattern-uri trebuie activată optiunea shopt -s extglob.

-	{sir1,sir2,sir3,..} - se potriveste cu unul dintre sirurile dintre acolade

-	?(lista_sabloane) - se potriveste cu o aparitie sau nici una a unuia dintre sabloane
-	*(lista_sabloane) - se potriveste cu nici o aparitie sau mai multe a unuia dintre multe sabloane
-	+(lista_sabloane) - se potriveste cu o aparitie sau mai multe a unuia dintre sabloane
-	@(lista_sabloane) - se potriveste cu exact un sablon din lista
-	!(lista_sabloane) - se potriveste cu toate sabloanele din lista mai putin unul

Exemplu:
```
$ ls *.sh fun.sh hello.sh pos2.sh pos.sh 
$ ls ?h* shell2.lyx shell.lyx shell.tex 
$ ls *{e,y}x lab8.lyx shell2.lyx shell.lyx shell.tex 
$ ls *[a-h]e* hello.sh shell2.lyx shell.lyx shell.tex
```

# Comenzi utile

O serie de comenzi (interne sau externe) sunt utile în crearea de scripturi shell.

## echo

Comanda **echo** este folosită pentru a afișa un șir de caractere, o variabilă la ieșire standard:
```
$ echo alfa
alfa
```
```
$ echo $PATH
/home/razvan/bin:/usr/local/bin:/usr/bin:/bin:/usr/bin/X11:/usr/games
$ echo '$PATH' $PATH
```

## cat, tac, head, tail

Comanda **cat** afișează conţinutul unui fișier sau al intrării standard. Comanda **tac** afișează conţinutul unui fișier inversat. Comanda **head** afișează începutul unui fișier sau al intrării standard, în timp ce comanda **tail** afișează sfârșitul acestuia:

```
$ cat /etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
```
```
$ tac /etc/passwd
bogdan:x:1006:1005:,,,:/home/students/bogdan:/bin/bash
monica:x:1005:1005:,,,:/home/students/monica:/bin/bash
lucian:x:1004:1004:,,,:/home/lucian:/bin/bash
```
```
$ head -n 3 /etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
```
```
$ tail -n -3 /etc/passwd
lucian:x:1004:1004:,,,:/home/lucian:/bin/bash
monica:x:1005:1005:,,,:/home/students/monica:/bin/bash
bogdan:x:1006:1005:,,,:/home/students/bogdan:/bin/bash
```

## read

Comanda **read** este folosită pentru citirea de informaţii de la intrarea standard:
```
$ read a
alfa
```
```
$ echo $a
alfa
```
O folosire frecventă este în combinaţie cu while pentru citirea linie cu linie a conţinutului unui fișier:
```
$ cat out.txt | while read a; do echo "am citit $a"; done
am citit a
am citit b
am citit c
```

## find

Comanda **find** este o comandă fundamentală pentru parcurgerea intrărilor dintr-o ierarhie a sistemului de fișiere. Câteva exemple de utilizare sunt prezentate în continuare:

-	cautarea headerelor de sistem care încep cu șirul std:
```
$ find /usr/include/ -type f -name 'std*'
/usr/include/c++/4.1.2/ext/stdio_sync_filebuf.h 
/usr/include/c++/4.1.2/ext/stdio_filebuf.h 
/usr/include/c++/4.1.2/stdexcept 
/usr/include/stdio.h
/usr/include/linux/stddef.h
/usr/include/stdlib.h
/usr/include/bits/stdio-lock.h
/usr/include/bits/stdio.h
/usr/include/bits/stdio2.h
/usr/include/bits/stdio_lim.h
/usr/include/stdint.h
/usr/include/nptl/bits/stdio-lock.h
/usr/include/stdio_ext.h
```
-	căutarea șirului mutex în headerele de sistem care conţin șirul lock
```
$ find /usr/include/ -name '*lock*' -exec grep -H mutex {} \;
/usr/include/linux/seqlock.h: * This can be used when code [...]
/usr/include/linux/seqlock.h: * own mutexing.
/usr/include/linux/lockdep.h: * rwlocks, mutexes and rwsems) [...]
/usr/include/linux/lockdep.h:# define mutex_acquire(l, s, t, i) [...]
```
Opţiunea exec este folosită pentru a rula o comandă pentru fișierele găsite. Șirul {} este special și se înlocuiește cu numele fișierului găsit de find. Caracterul ; (citat cu ajutorul backslash) indică încheierea comenzii de executat.

## dd

Utilitarul **dd** din Linux poate fi folosit pentru a:
- Clona un disc
- Clona o partiție
- Face a backup și a restaura întregul hard disk sau partiție
- Șterge conținutul hard diskului

syntaxa:
```
$ sudo dd if=source-disk of=destination-disk [option]

Unde
if: utilizat pentru specificarea unui fișier de intrare
source-disc: este discul sursă de unde vor fi clonate fișierele
of: folosit pentru specificarea unui fișier de ieșire
destination-disk: Este discul de destinație pe care doriți să plasați fișierele copiate
option: progresul, viteza de transfer al fișierului, formatul fișierului etc.
```
Mai întâi, se execută comanda **lsblk** pentru a vizualiza toate discurile disponibile pe sistem, sau **fdisk -l**.

Pentru a clona un întreg disc /dev/sdb în /dev/sdc, vom folosi următoarea comandă:
```
$ sudo dd if=/dev/sdb of=/dev/sdc status=progress
```
De exemplu, pentru a clona o partiție /dev/sdb2 în /dev/sdc2, comanda ar fi:
```
$ sudo dd if=/dev/sdb2 of=/dev/sdc2 status=progress
```
Următoarea comandă dd elimină datele din /dev/hdX:
```
$ dd if=/dev/zero of=/dev/hdX bs=512 count=1
```

# Filtre de text

Variabilele, structurile de control și procedurile sunt întâlnite în toate limbajele de programare. Ce face insa un script shell indicat pentru sarcini administrative și repetitive este posibilitatea de îmbinare a comenzilor simple, de lucru cu fișierele sistemului pentru a obține informatiile dorite și pentru a adăuga o nouă funcționalitate. De obicei aceste sarcini necesita o procesare sofisticată. În aceste situații se folosesc filtrele de text.

Comenzi folosite ca și filtre de text sunt head, tail, grep, sort, uniq, tr, cut.


## head, tail

Cele doua comenzi sunt folosite pentru afișarea numai a primelor sau a ultimelor linii de text din cadrul unui fișier. Sintaxa lor este asemanatoare:
```
head [-n lines] files

tail [-n lines] files
```
Primul argument, dacă există, afiseaza primele, respectiv ultimele n linii din text. Lipsa acestuia impune n = 10.

Exemple de comenzi sunt:
```
$ ls -l

total 44

drwxr-xr-x	2	razvan razvan 4096 Nov	16	00:01 lab1
drwxr-xr-x	3	razvan razvan 4096 Oct	12	00:02 lab2
-rwxr-xr-x	1	razvan razvan  937 Oct	19	00:53 lab2html.sh
drwxr-xr-x	3	razvan razvan 4096 Oct	19	00:50 lab3
drwxr-xr-x	3	razvan razvan 4096 Oct	26	01:09 lab4
drwxr-xr-x	3	razvan razvan 4096 Nov	2	09:47 lab5
drwxr-xr-x	3	razvan razvan 4096 Nov	12	19:56 lab6
drwxr-xr-x	2	razvan razvan 4096 Nov	16	12:43 lab7
drwxr-xr-x	2	razvan razvan 4096 Nov	20	12:34 lab8
drwxr-xr-x	5	razvan razvan 4096 Oct	17	19:52 solab
drwxr-xr-x	2	razvan razvan 4096 Nov	12	20:06 tema1
```
```
$ ls -l | head -3
total 44				
drwxr-xr-x	2	razvan razvan 4096 Nov	16	00:01 lab1
drwxr-xr-x	3	razvan razvan 4096 Oct	12	00:02 lab2
```
```
$	ls	-l | tail -4
drwxr-xr-x	2	razvan razvan 4096 Nov	16	12:43 lab7
drwxr-xr-x	2	razvan razvan 4096 Nov	20	12:34 lab8
drwxr-xr-x	5	razvan razvan 4096 Oct	17	19:52 solab
drwxr-xr-x	2	razvan razvan 4096 Nov	12	20:06 tema1
```
În cazul comenzi **tail** o opțiune utilă este -f care menține fișierul de vizualizat deschis pe masura ce programele scriu in el, spre exemplu pentru a vizualiza un fișier de log pe măsura ce informațiile sunt scrise în el:
```
$ tail -f /var/log/apache/access_log
```

## grep

Comanda **grep** permite localizarea liniilor intr-un fișier care conține o expresie căutată. Sintaxa de baza este
```
$ grep word file
```
file este numele fișierului în care vrem să gasim cuvântul word. Un exemplu de utilizare este:
```
$ grep latex lab2html.sh latex2html "$BASE.tex" 2>&1 > /dev/null latex "$BASE.tex" 2>&1 > /dev/null
```
De multe ori dorim să realizăm căutarea în mod case-insensitive (adică să nu conteze faptul ca se caută UNIX sau unix). Pentru aceasta folosim optiunea -i.

Când nu se precizează un fișier se folosește intrarea standard, grep devenind ideal pentru lucrul cu pipes. De exemplu:
```
$ last	| head -100 | grep tty
root	tty1	Wed	Nov	16	11:00 -	down	(00:00)
razvan	tty1	Wed	Nov	16	00:07 -	00:08	(00:01)
```
O altă opțiune utilă este -v. Aceasta permite căutarea acelor linii care NU conțin cuvantul transmis ca parametru. Spre exemplu, dacă dorim afișarea utilizatorilor care nu au ca și director de baza un director de forma /home/nume, vom folosi comanda:
```
$ grep -v /home /etc/passwd
root:x:0:0:root:/root:/bin/bash
daemon:x:1:1:daemon:/usr/sbin:/bin/sh
bin:x:2:2:bin:/bin:/bin/sh
sys:x:3:3:sys:/dev:/bin/sh
sync:x:4:65534:sync:/bin:/bin/sync
games:x:5:60:games:/usr/games:/bin/sh
man:x:6:12:man:/var/cache/man:/bin/sh
lp:x:7:7:lp:/var/spool/lpd:/bin/sh
mail:x:8:8:mail:/var/mail:/bin/sh
```

Un alt exemplu de folosire este parsarea output-ului comenzii ps:
```
$ ps -af | grep lyx
razvan	3767	3719	0	12:19	pts/0	00:00:05	lyx
razvan	4219	3719	0	12:53	pts/0	00:00:00	grep lyx
```
```
$ ps -af | grep lyx | grep -v grep
razvan	3767	3719	0 12:19 pts/0	00:00:05 lyx
```
Optiunea -n permite afisarea numarului liniei care continea cuvantul cautat.

Putem de asemenea să afișăm numai fișierele care conțin acel cuvânt (fără afișarea liniilor). Pentru aceasta folosim opțiunea -l (listare). De obicei este folosită în conjuncție cu opțiunea -R pentru căutarea recursivă in cadrul unei structuri de directoare:
```
$ grep -r -l "#include <sys/wait.h>" .

./lab2/lab2.lyx
./lab2/lab2.tex
./lab2/wait_zombie.c
./lab2/simple_exec.c
./solab/lab2/lab2.lyx
./solab/lab2/lab2.tex
./solab/lab2/wait_zombie.c
./solab/lab2/simple_exec.c
./lab6/lab6.lyx
./lab6/mmap.c
./lab6/lab6.tex
```

## tr

Comanda **tr** (transliterate) este folosita pentru a translata caracterele dintr-un set de caractere intr-un alt set de caractere. Sintaxa de baza este:
```
tr 'set1' 'set2'
```
Spre exemplu, daca am dori să aflăm numărul de cuvinte dintr-un text, am translata toate caracterele speciale în spații cu o comanda de forma:
```
$ tr '!?":'\[\]{}(),.' ' ' < file
```
Caracterele [ si ] au fost escapate folosind \. Daca dorim sa translatam literele mari in litere mici, folosim:
```
$ tr 'A-Z' 'a-z' < file
```
In cazul în care setul set2 are mai puține caractere decât setul set1 acestea vor fi multiplicate pentru a ajunge la aceeasi dimensiune.

Urmatorul pas ar fi eliminarea spatiilor redundante. Optiunea -s (squeeze) inlocuieste o succesiune de doua sau mai multe caractere cu unul singur. Un exemplu este:
```
$ echo shell programming | tr -s 'lm' shel programing
```

## sort

Comanda **sort** este utilizată pentru sortarea liniilor alfabetic. Un exemplu de utilizare este:
```
$ echo -en "alfa\nomega\ngamma\nbeta\n"
alfa
omega
gamma
beta
```
```
$ echo -en "alfa\nomega\ngamma\nbeta\n" | sort
alfa
beta
gamma
omega
```
O optiune utilă in cazul sort este sortarea după valoarea numerică a șirurilor. Pentru aceasta folosim -n. De exemplu:
```
$ echo -en "10\n43\n4\n9\n123\n5\n" | sort 10
123
4
43
5
9
```
```
$ echo -en "10\n43\n4\n9\n123\n5\n" | sort -n
4
5
9
10
43
123
```
De multe ori output-ul apare intr-o forma in care elementele de sortat sunt intr-o altă coloană (nu prima, cea folosita implicit de sort). Pentru aceasta se poate folosi opțiunea -k (key). Sintaxa, în cazul folosirii acestei optiuni, este:
```
sort -k start, end file
```
Aici start este coloana de start a cheii, iar end este coloana de stop a cheii. Prima coloana este cea dupa care se face sortarea, iar, in cazul in care sunt doua sau mai multe elemente egale, se face deosebirea intre acestea folosind coloana urmatoare din cheie, etc.

Exemplu de utilizare este:
```
sort -rn -k 2,2 file	; sorteaza dupa a doua coloana
```

## uniq

Se pot elimina duplicatele folosind optiunea -u la sort. Totusi, de multe ori este util sa afisam de cate ori apare un cuvânt. Pentru aceasta vom folosi comanda **uniq** cu optiunea -c. Atentie! uniq face eliminarea duplicatelor numai daca liniile sunt sortate. Exemple de utilizare:
```
$ echo -en "alfa\nbeta\nbeta\nalfa\nbeta\ngamma\nalfa\n" | uniq
alfa
beta
alfa
beta
gamma
alfa
```
```
$ echo -en "alfa\nbeta\nbeta\nalfa\nbeta\ngamma\nalfa\n" | sort | uniq
alfa
beta
gamma
```
```
$ echo -en "alfa\nbeta\nbeta\nalfa\nbeta\ngamma\nalfa\n" | sort | uniq -c
3	alfa
3	beta
1	gamma

```

## wc

Comanda **wc** (word count) este folosita pentru a contoriza numarul de linii, de cuvinte sau de caractere dintr-un text sau de la intrarea standard. Pentru aceasta i se pot specifica optiunile -c, -w, -l.


## cut

Comanda **cut** selecteaza numai anumite părți (coloane) ale fișierului de intrare sau ale intrării standard.

Sintaxa cea mai folosită este:
```
cut -d delim -f fields
```
Folosindu-se delimitatorul delim se vor selecta numai câmpurile fields. Exemple de utilizare sunt:
```
$ ls -l

total 44					
drwxr-xr-x	2	razvan razvan 4096 Nov	16	00:01 lab1
drwxr-xr-x	3	razvan razvan 4096 Oct	12	00:02 lab2
-rwxr-xr-x	1	razvan razvan  937 Oct	19	00:53 lab2html.sh
drwxr-xr-x	3	razvan razvan 4096 Oct	19	00:50 lab3
drwxr-xr-x	3	razvan razvan 4096 Oct	26	01:09 lab4
drwxr-xr-x	3	razvan razvan 4096 Nov	2	09:47 lab5
drwxr-xr-x	3	razvan razvan 4096 Nov	12	19:56 lab6
drwxr-xr-x	2	razvan razvan 4096	Nov	16	12:43 lab7
drwxr-xr-x	2	razvan razvan 4096	Nov	20	14:38 lab8
drwxr-xr-x	5	razvan razvan 4096	Oct	17	19:52 solab
drwxr-xr-x	2	razvan razvan 4096	Nov	12	20:06 tema1
```

```
$ ls -l | cut -d ' ' -f 1,9

total

drwxr-xr-x 00:01
drwxr-xr-x 00:02
-rwxr-xr-x 19
drwxr-xr-x 00:50
drwxr-xr-x 01:09
drwxr-xr-x 2
drwxr-xr-x 19:56
drwxr-xr-x 12:43
drwxr-xr-x 14:38
drwxr-xr-x 19:52
drwxr-xr-x 20:06
```
```
$ ls -l | tr -s ' ' | cut -d ' ' -f 1,9

total

drwxr-xr-x lab1
drwxr-xr-x lab2
-rwxr-xr-x lab2html.sh
drwxr-xr-x lab3
drwxr-xr-x lab4
drwxr-xr-x lab5
drwxr-xr-x lab6
drwxr-xr-x lab7
drwxr-xr-x lab8
drwxr-xr-x solab
drwxr-xr-x tema1
```

Comenzi din aceeasi categorie sunt **paste** si **join**.

# Exemple

In continuare sunt prezentate cateva exemple de script-uri shell.
```
#!/bin/bash
#	Afisarea numelui de utilizator si a home directory-ului pentru
#	utilizatorii care se afla in ultimele N login-uri in sistem, unde
#	N este primit ca parametru
#	Informatiile dorite sunt furnizate de /etc/passwd si de last -20

function usage ()
{
    echo "Usage: $0 num_login"
    exit 1
}

if test $# -ne 1; then
    usage
fi

#se obtine lista utilizatorilor

users=$(last -$1 | head -$(($1-2)) | cut -d ' ' -f 1 | sort | uniq | tr '\n' ' ')

for i in $users; do

home_dir=$(cat /etc/passwd | grep "$i" | cut -d ':' -f 6)

#no home dir

if test -z $home_dir; then
    echo "User: $i Home dir: NONE"
else
    echo "User: $i Home dir: $home_dir"
fi

done

exit 0
```

```
#!/bin/bash
#	Sa se mute copieze in directorul bkup toate fisierele modificate
#	cu mai mult de 30 de zile in urma pentru utilizatorul curent

if ! test -d ~/bkup; then
    mkdir ~/bkup
fi

#	cauta fisirele dorite
#	do not try this if u have a lot of old files and little disk space ;) 
find ~ -type f -ctime +30 -exec cp -u {} ~/bkup \;

exit 0
```

# Exerciţii

1.	Creaţi și rulaţi un script shell care afișează mesajul Hello, World!.
-	folosiţi apostrof (') pentru citare (! este văzut ca un caracter special)

2.	Afișati numele utilizatorilor din sistem care încep cu litera 's'. 
-	nu e nevoie de script shell; se poate face cu un one-liner
-	folositi /etc/passwd, grep și cut

3.	Afișaţi separate prin tab numele și shell-ul utilizatorilor din sistem care NU au home-ul în /home/...
-	nu e nevoie de script shell
-	folosiţi /etc/passwd, grep, cut, tr

4.	Creaţi un script shell care să afiẟeze suma memoriei ocupate de procesele din sistem după cum reiese din ieșirea comenzii ps -e -o rss. Comparaţi cu rezultatul oferit prin rularea comenzii free.
-	folosiţi opţiuni tail pentru a afiẟa toate liniile mai puţin prima (prima este antetul RSS) (man tail)
-	la tail este utila aici forma cu + a argumentului; daca nu gasiti in man nimic despre asta info tail
-	folosiţi expandare aritmetica

5.	Folosiţi un one-liner pentru a contoriza toate fișierele cu extensia .sh din ierarhia /etc.
-	folosiţi find

6.	Creaţi un script shell pentru a număra toate fiẟierele din /etc pentru care comanda file spune că sunt scripturi Shell:
$ file test.bash test.bash: Bourne shell script text executable
Puteţi să faceţi scriptul un one-liner?
-	va trebui să folosiţi find în combinaţie cu for

7.	Descărcaţi folosind wget arhiva cu sursele nucleului Linux 1.0: (ftp://ftp.eu.kernel.org/pub/linux/kernel/v1.0/linux-1.0.tar.bz2). Dezarhivaţi (tar xjf ...). Creaţi un script shell care să contorizeze numărul de linii din fișierele sursă.
-	fișierele sursă se consideră cele cu extensia .c sau .h

# Link-uri utile

-	http://www.linuxcommand.org/index.php
-	http://www.comptechdoc.org/os/linux/programming/script/linux_pgscript.html
-	http://tldp.org/LDP/abs/html/index.html
-	http://wiki.lug.ro/mediawiki/index.php/Tutorial_Shell_Scripting
-	http://www.gnu.org/software/bash/manual/bash.html (disponibil si in format info - $ info bash)
