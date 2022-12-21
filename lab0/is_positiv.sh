#!/bin/sh
#
# Script pentru a observa daca argumentul este pozitiv sau negativ
#
if [ $# -eq 0 ]; then
    echo "$0 : Trebuie sa introduceti un intreg"
exit 1
fi

if test $1 -ge 0; then
    echo " $1 este un numar pozitiv"
else
    echo " $1 este un numar negativ"
fi
