#!/bin/bash
echo "This is a simple calculator program!"
echo -n "insert first number:"
read a;
echo -n "insert second number:"
read b;
echo "Sum = $((a+b))"
echo "Diff = $((a-b))"
echo "Prod = $((a*b))"
exit
