#include <stdio.h>
#include <stdlib.h>

int main() {
    int* ptr = (int *)malloc(sizeof(int));
    ptr = 0x0;

    *ptr = 5;

    return 0;
}