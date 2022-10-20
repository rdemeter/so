#!/bin/sh

# 0 - user was found
# 1 - wrong argument
# 2 - user not found

if test "$#" -eq 1
then
    if who -a | tr -s ' ' | cut -d ' ' -f 1 | grep "^$1$" > /dev/null
    then
        echo "User $1 is logged-in."
    else
        echo "User $1 is not logged-in."
        exit 2
    fi
else
    echo "Usage: $0 <user>"
    exit 1
fi
