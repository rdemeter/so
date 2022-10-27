#include <stdio.h> 
int suma(int a, int b, int c);

int main() {
    printf("1 + 2 fac %d\n", suma(1, 2, 0));
    return 0;
}

int suma(int a, int b, int c) {
    return a + b + c;
}
