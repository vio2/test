#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
int main()
{
    printf("Hello World");
    for (int i = 0; i < 200; i++) {
        printf("\"%s\", ", strerror(i));
    }

    return 0;
}