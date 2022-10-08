#include <stdio.h>
#include <string.h>

#define NUM_DATA 5

struct player
{
    char name[20];
    int level;
    int score;
};

int main() {
    struct player players[NUM_DATA];
    // Make random players data
    strcpy(players[0].name, "Abby12"); players[0].level = 10; players[0].score = 150;
    strcpy(players[1].name, "Birdie"); players[1].level = 20; players[1].score = 300;
    strcpy(players[2].name, "Cahoot"); players[2].level = 30; players[2].score = 400;
    strcpy(players[3].name, "DieOfWorks"); players[3].level = 45; players[3].score = 800;
    strcpy(players[4].name, "End_Turn"); players[4].level = 50; players[4].score = 750;

    FILE *file = fopen("output.txt", "w");
    if (!file) {
        printf("Couldn't open output file!\n");
        return 1;
    }

    for (int i = 0; i < NUM_DATA; i++) 
    {
        fprintf(file,"%s %d %d\n", players[i].name, players[i].level, players[i].score);
    }
    fclose(file);

    return 0;
}