#include <stdio.h>

int isPrime(int num)
{   
    int i = 2;
    while(i*i <= num) 
    {
        if (num % i == 0) {
            printf("is not Prime");
            return 0;
        }
        i += 1;
    }
    printf("is Prime");
    return 1;
}

int main() {
    int inp, prime;
    scanf("%d", &inp);
    prime = isPrime(inp);

    return 0;
}