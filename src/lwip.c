#include <stdio.h>
#include <stdlib.h>

#include "lwip/ip.h"


int main(int argc, char **argv)
{
    extern int a;
    extern int say() ;
    int b = say();

    printf("%d\n", a);
    return 0;
}