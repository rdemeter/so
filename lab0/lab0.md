# Shell scripting

Shell-ul este principala interfață de comunicare între utilizator și sistemul de operare. Deși, în mod intuitiv, shell-ul este identificat cu o interfață în linia de comandă, poate fi și o interfaţă grafică. Exemplu este Explorer-ul sistemului de operare Windows.

În cele ce urmează vom studia interfaţa de tip CLI (Command Line Interface) oferită de sistemele de operare Unix. Deși cu o curbă de învăţare mai mare decat o interfaţă grafică, CLI permite un control mult mai bun al sistemului. Mai mult, shell-ul dispune de un limbaj de programare. Un program shell, denumit script shell, este folosit pentru a îmbina mai multe comenzi ẟi diverse structuri de control pentru a obţine o nouă funcţionalitate sau pentru automatizarea sarcinilor. În acest fel un script shell este un instrument esenţial pentru sarcinile administrative ẟi alte rutine repetitive care nu necesită funcţionalităţi ale unor limbaje de programare avansate.

În continuare ne vom referi la Bash (Bourne Again SHell). Există ẟi alte shell-uri pe sisteme Unix precum tcsh, zsh, ash, etc. De curând, Microsoft oferă PowerShell pe sistemele Windows. PowerShell are o abordarea orientată pe obiecte ẟi un set de funcţionalităţi care acoperă nevoile de administrare ale unui sistem Windows.

## Cel mai simplu script shell

Un script simplu care doar afișează mesajul "Hello, World!" este următorul:

```
#!/bin/bash

#	afiseaza mesaj
echo "Hello, World!"
exit 0
```
Execuţia acestuia este următoarea:
```
$ chmod +x hello.sh
$ ./hello.sh Hello, World!
```
Se observă că este necesar ca fișierul să fie executabil pentru a putea fi interpretat. Șirul #! de la începutul fișierului poartă denumirea de shebang. Acesta indică sistemului ce program va fi invocat pentru a interpreta scriptul. Exemple pot fi:

#!/bin/sh
#!/bin/bash
#!/usr/bin/perl
#!/usr/bin/python
#!/usr/bin/awk -f

Spre exemplu, următorul script se șterge pe sine:

#!/bin/rm -f

aici putem scrie orice ... oricum se va șterge

Un script poate fi rulat prin precizarea explicită a interpretorului în linia de comandă:

```
$ bash hello.sh
Hello, World!
```

În această situaţie nu este nevoie ca scriptul sa fie executabil și nici nu este nevoie de prezenţa liniei #!/bin/bash.

Caracterul # semnifică începutul unui comentariu care durează pană la sfarẟitul liniei.

Comanda exit este folosită pentru a indica valoarea de retur a scriptului. Este implicit 0 (cu alte cuvinte nu era necesar să apară în script).


#Operatori shell

Shell-ul prezinta o serie de operatori folositi pentru imbinarea comenzilor.


##Concatenarea comenzilor

Următorii operatori sunt folositi pentru concatenarea diverselor comenzi:

·	command1 ; command2 - comenzile sunt executate secvenţial (una dupa alta)

·	command1 && command2 - command2 este executată numai dacă command1 are valoare de retur 0
·	command1 || comand2 - command2 este executată numai dacă command1 are valoare de retur diferita de 0


## Inlantuirea comenzilor

Inlantuirea comenzilor se realizeaza folosind operatorul | (pipe). In aceasta situatie iesirea unei comenzi devine intrarea pentru cealalta.

Cateva exemple sunt prezentate in continuare:
```
$ last -30 | grep Tue							
razvan	pts/2	:0.0	Tue Jan	2	20:42 - down	(05:12)
razvan	pts/2	:0.0	Tue Jan	2	20:35 - 20:41	(00:06)
razvan	pts/1	:0.0	Tue	Jan	2	20:34 -	21:23	(00:48)
razvan	pts/0	:0.0	Tue	Jan	2	20:27 -	down	(05:27)
wtmp begins Tue Nov 14 04:22:33	2006
```

```
$ last -30 | grep Tue | tr -s '	'	
razvan pts/2 :0.0 Tue Jan 2 20:42	- down (05:12)
razvan pts/2 :0.0 Tue Jan 2 20:35	- 20:41 (00:06)
razvan pts/1 :0.0 Tue Jan 2 20:34	- 21:23 (00:48)
razvan pts/0 :0.0 Tue Jan 2	20:27	- down (05:27)
wtmp begins Tue Nov 14 04:22:33

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

$ last -30 | grep Tue | tr -s ' ' | head -4 | cut -d ' ' -f 2 | uniq | wc -l 3


## Redirectari

Comenzilor le pot fi redirectate, respectiv, intrarea standard, iesirea standard si eroarea standard dintr-un fisier. O parte din operatorii folositi pentru redirectare sunt:

·	> - redirectarea iesirii standard

·	2> - redirectarea erorii standard
·	2>&1 - redirectarea erorii standard in iesirea standard. Efectiv, unificarea stderr cu stdout.
·	< - redirectarea intrarii standard

Exemple:

$ ls -l > ls.out

$ strace ls 2> strace.out

$ grep "alpha" < file.txt


# Variabile

Ca orice limbaj, shell-ul permite utilizarea de variabile. Spre deosebire de limbajele cunoscute, variabilele shell nu au tipuri. O variabila poate fi evaluata atat ca numar cat si ca sir de caractere.

Exemple de definire de variabile:

var1=2
var2=4asa
var3='abcd'
my_var="asdf"
my_other_var="a 1 3 4"
new_var=$var1
new_var2=${var2}var3

ATENTIE! Sintaxa shell este foarte stricta; NU este permis sa existe spatii intre numele variabilei si caracterul = sau intre caracterul = si valoarea variabilei.

Se observa ca valoarea unei variabile este referita prin folosirea simbolului $.

Exemple de folosire de variabile:

$ echo $var1
2

$ echo $var12

$ echo ${var1}2
22

$ echo "$var1"
2

$ echo "$var1"2 22

$ echo $var1$my_other_var 2a 1 3 4

$ echo "$var1 $my_other_var"

2 a 1 3 4


# Argumente in linia de comanda

Un script poate primi argumente in linia de comanda. Argumentele sunt referite respectiv folosind parametrii pozitionali: $1, $2, ... $0 este numele scriptului (echivalent cu argv[0] din C).

Numarul de argumente din linia de comanda este dat de $#. $# va fi 0 daca nu avem argumente in linia de comanda (echivalentul C - argc - ar fi avut valoarea 1 in acest caz).

$@ poate fi folosit pentru obtinerea intregii liste de argumente separate prin spatiu.

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

$ chmod +x test.bash
$ ./test.bash alfa "beta gamma"
Scriptul are 2 parametri
Numele scriptului este ./test.bash
Primul parametru este alfa
Cel de-al doilea parametru este beta gamma
Lista de parametri este alfa beta gamma

## shift

Comanda builtin shift este folosita pentru deplasarea parametrilor pozitionali cu valoarea primita ca parametru (sau 1 daca nu este prezenta). Astfel daca se primeste valoarea N, parametrii pozitionali de la N+1 la $# vor fi redumiti la $1, $2, ... $#-N+1

Exemplu:

$ cat pos2.sh

#!/bin/sh
if test $# -ge 2; then
    echo "Parametrii inainte de shift sunt $@"
    shift
    echo "Parametrii dupa shift sunt $@"
fi

$ ./pos2.sh a b c
Parametrii inainte de shift sunt a b c
Parametrii dupa shift sunt b c

# Caractere speciale

Un set de caractere au semnificatie speciala in cadrului shell-ului.

## spatiu

Caracterul spatiu (blank) este separator pentru argumentele in linia de comanda sau pentru elementele unei liste. Dacă se doreste transmiterea ca parametru a unui argument ce conține spatiu acesta trebuie citat (quoted):

$ ls my\ dir
$ ls "my dir"
$ ls 'my dir'

## backslash

Caracterul backslash forteaza caracterul ce-l precede sa-si pastreze semnificatia literala; cu alte cuvinte, este folosit pentru a intarzia (a escapa) acel caracter:

$ echo $var1
test

$ echo \$var1
$var1


## ghilimele




