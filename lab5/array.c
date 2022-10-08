#include <stdio.h>
#include <string.h>

int main() {
    char inp[20], out[20];
    int c = 0;
    scanf("%s", &inp);

    for (int i = 0; i < strlen(inp); i++)
    {
        if (inp[i] >= 'A' && inp[i] <= 'Z') 
        { 
            out[c] = inp[i]; 
            c += 1;
        }
    }
    out[c] = '\0';

    printf("%s", out);

    return 0;
}