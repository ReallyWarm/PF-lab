#include <stdio.h>

char changecase(char c) { return c ^= 0b00100000; }
// uppercase don't have bit 1 at position 6 but lowercase have.

int main() {
    char inp[256], *ptr;
    ptr = inp;
    scanf("%s", inp);

    while (*ptr != '\0')
    {
        if ((*ptr >= 'a' && *ptr <= 'z') || (*ptr >= 'A' && *ptr <= 'Z'))
            printf("%c", changecase(*ptr));
        ptr++;
    }

    return 0;
}

/* TEST CASE
 * aB$Cde%f#
 */