#include <stdio.h>
#define expand_macro(a) printf ("variabila %s are valoarea %d\n", #a, a)

int main (void)
{
    int my_uber_var = 12345;
    expand_macro (my_uber_var);
    return 0;
}
