#! /bin/bash

echo -n "insert first number: "
read a;
echo -n "insert second number: "
read b;
echo "choose option"
echo "1 - add"
echo "2 - sub"
echo "3 - multiply"
echo "other - exit"

read opcio;
case $opcio in
    1)echo "Sum: " $((a+b));;
    2)echo "Diff: " $((a-b));;
    3)echo "Product: " $((a*b));;
    *)echo "wrong input! Exit!"
    exit;;
esac
exit 0
