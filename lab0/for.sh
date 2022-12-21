#! /bin/bash

echo -n "insert a number: "
read num;
while (( $num < 2 || $num > 10 ))
do
    echo -n "wrong input! try again: "
    read num;
done

for (( i = 0 ; i <= $num ; i++ ))
do
    echo $i
done

exit 0 
