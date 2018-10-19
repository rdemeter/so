#include <stdio.h> 

int f(int a, int b) 
{
    int c; 

    c = a + b;

    return c;
}

int main()
{
    char *bug = 0; 

    *bug = f(1, 2);

    return 0;
}