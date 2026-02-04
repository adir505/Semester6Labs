//Test file for lexical analyzer
#include <stdio.h>
#include <stdlib.h>

int two_sum(int x, int y) {
    return x + y;
}

int main() {
    printf("Hello, World!\n");
    int a,b;
    a = 5;
    b = two_sum(a, 10);
    for(int i=0; i< b; i++){
        printf("%d\n", i);
    }
    return 0;
}