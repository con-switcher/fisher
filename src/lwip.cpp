#include <stdio.h>
#include <stdlib.h>

#include "lwip/ip.h"

extern "C" {
    extern int a;
    extern int say() ;
};
int main(int argc, char **argv)
{

    int b = say();

    printf("%d\n", a);
    return 0;
}