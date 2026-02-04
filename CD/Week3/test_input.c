#include <stdio.h>

int add(int x, int y) {
    int result;
    result = x + y;
    return result;
}

float multiply(float a, float b) {
    float product;
    product = a * b;
    return product;
}

int main() {
    int num1, num2, sum;
    float f1, f2;
    char ch;
    
    num1 = 10;
    num2 = 20;
    sum = add(num1, num2);
    
    return 0;
}
