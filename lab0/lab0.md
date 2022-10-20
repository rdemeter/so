#Shell scripting

Shell-ul este principala interfaţă de comunicare între utilizator ẟi sistemul de operare. Deẟi, în mod intuitiv, shell-ul este identificat cu o interfaţă în linia de comandă, poate fi ẟi o interfaţă grafică. Exemplu este Explorer-ul sistemului de operare Windows.

În cele ce urmează vom studia interfaţa de tip CLI (Command Line Interface) oferită de sistemele de operare Unix. Deẟi cu o curbă de învăţare mai mare decat o interfaţă grafică, CLI permite un control mult mai bun al sistemului. Mai mult, shell-ul dispune de un limbaj de programare. Un program shell, denumit script shell, este folosit pentru a îmbina mai multe comenzi ẟi diverse structuri de control pentru a obţine o nouă funcţionalitate sau pentru automatizarea sarcinilor. În acest fel un script shell este un instrument esenţial pentru sarcinile administrative ẟi alte rutine repetitive care nu necesită funcţionalităţi ale unor limbaje de programare avansate.

În continuare ne vom referi la Bash (Bourne Again SHell). Există ẟi alte shell-uri pe sisteme Unix precum tcsh, zsh, ash, etc. De curând, Microsoft oferă PowerShell pe sistemele Windows. PowerShell are o abordarea orientată pe obiecte ẟi un set de funcţionalităţi care acoperă nevoile de administrare ale unui sistem Windows.

##Cel mai simplu script shell

Un script simplu care doar afișează mesajul "Hello, World!" este următorul:

```
#!/bin/bash

#	afiseaza mesaj
echo "Hello, World!"
exit 0
```
Execuţia acestuia este următoarea:
```
razvan@asgard:~/so/shell$ chmod +x hello.sh
razvan@asgard:~/so/shell$ ./hello.sh Hello, World!
```
Se observă că este necesar ca fișierul să fie executabil pentru a putea fi interpretat. ẞirul #! de la începutul fișierului poartă denumirea de shebang. Acesta indică sistemului ce program va fi invocat pentru a interpreta scriptul. Exemple pot fi:

#!/bin/sh

#!/bin/bash

#!/usr/bin/perl

#!/usr/bin/python

#!/usr/bin/awk -f

Spre exemplu, următorul script se șterge pe sine:

#!/bin/rm -f

aici putem scrie orice ... oricum se va șterge
