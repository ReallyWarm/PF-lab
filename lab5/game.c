#include <stdio.h>
#include <windows.h>
#include <conio.h>

void drawShip(int x, int y);
void eraseShip(int x, int y);
void drawRuler();

int main() {
    char key = ' ';
    int pos_x = 38, pos_y = 20;

    system("cls");
    // drawRuler();
    drawShip(pos_x, pos_y);
    
    do 
    {
        if (_kbhit()) {
            key = _getch();
            if (key == 'a' && pos_x > 0) {   
                eraseShip(pos_x, pos_y);
                drawShip(--pos_x, pos_y);
            }
            if (key == 'd' && pos_x < 80 - 5) {
                eraseShip(pos_x, pos_y);
                drawShip(++pos_x, pos_y);
            }
            if (key == 'w' && pos_y > 0) {   
                eraseShip(pos_x, pos_y);
                drawShip(pos_x, --pos_y);
            }
            if (key == 's' && pos_y < 20) {   
                eraseShip(pos_x, pos_y);
                drawShip(pos_x, ++pos_y);
            }
            fflush(stdin);
        }
        Sleep(100);
    } while (key != 'x');

    /* // move left to right
    for (int x = 0; x < 80 - 4; x++)
    {
        eraseShip(x-1,20);
        drawShip(x,20);
        Sleep(100);
    }
    */

    return 0;
}

void drawShip(int x, int y) {
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , c);
    printf("<-w->");
}

void eraseShip(int x, int y) {
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , c);
    printf("     ");
}

void drawRuler() {
    COORD r = {0, 21};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , r);
    printf("123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|");
}