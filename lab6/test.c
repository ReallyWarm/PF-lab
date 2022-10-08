#include <stdio.h>

int main() {
    int bullet[5][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}, i = 0, c = 0;

    while (i < 5) 
    {   
        if (bullet[i][0] == -1 || bullet[i][1] == -1)
        {
            printf("%d\n", bullet[i][0]);
            bullet[i][0] = i;
            bullet[i][1] = 5;
        }
        printf("%d %d\n", bullet[i][0], bullet[i][1]);
        i += 1;
    }

    while (c < 5) 
    {   
        for (i = 0; i < 5; i++)
        {
            if (bullet[i][1] > 0)
            {
                bullet[i][1] -= 1;
            }
            else
            {
                bullet[i][0] = -1;
                bullet[i][1] = -1;
                c += 1;
            }
            printf("%d %d %d %d\n", bullet[i][0], bullet[i][1], i, c);
        }
    }

    return 0;
}