#! /bin/bash

function f1 {
    echo "function f1 was called!"
}

if [ $# -ne 1 ]; then
    echo "wrong usage!"
    echo "usage: $0 1/0"
else
    if [  $1 = 0 ]; then
        echo "no funtion call!"
    elif [ $1 = 1 ]; then
        f1
    else
        echo "wrong parameter!"
    fi
fi
exit 0
