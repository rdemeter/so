#!/bin/bash

echo "read the input until the number is smaller then 10"

echo -n "insert a number: "
read num;
while ((num > 9))
do
    echo -n "wrong number, try again: "
    read num;
done

echo "correct number! num = $num"

exit
