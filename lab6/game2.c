#include <stdio.h>
#include <windows.h>
#include <conio.h>

void drawShip(int x, int y);
void eraseShip(int x, int y);
void drawBullet(int x, int y);
void eraseBullet(int x, int y);
void setCursor(int visible);
void setColor(int fg, int bg);

void drawTest(const int* test) 
{
    setColor(7, 0);
    COORD r = {0, 21};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , r);
    printf("123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|");
    r.Y = 22;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , r);
    printf("%d", *test);
}

int main() {
    char key = '.', move = 'S', shoot = 'F';
    int pos_x = 38, pos_y = 20;
    int bullet[5][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}}, 
        bullet_count = 0;
    setCursor(0);
    system("cls");

    drawShip(pos_x, pos_y);

    do {   
        /* drawTest(&bullet_count); */
        // KEY BOARD
        if (kbhit()) {
            key = getch();
            if (key == 'a') { move = 'L'; }
            if (key == 'd') { move = 'R'; }
            if (key == 's') { move = 'S'; }
            if (key == ' ') { if (bullet_count < 5) shoot = 'T'; }
            fflush(stdin);
        }
        // MOVING
        if (move == 'L' && pos_x > 0) {   
            eraseShip(pos_x, pos_y);
            drawShip(--pos_x, pos_y);
        }
        if (move == 'R' && pos_x < 80 - 5) {
            eraseShip(pos_x, pos_y);
            drawShip(++pos_x, pos_y);
        }
        // SHOOTING
        if (shoot == 'T') {
            for (int i = 0; i < 5; i++)
            {   
                if (bullet[i][0] == -1 || bullet[i][1] == -1) {
                    bullet[i][0] = pos_x + 2;
                    bullet[i][1] = pos_y - 1;
                    bullet_count += 1;
                    shoot = 'F';
                    break;
                }
            }
        }
        // DISPLAY BULLETS
        for (int i = 0; i < 5; i++)
        {
            if (bullet[i][1] > 0) {
                if (bullet[i][1] + 1 != 20)
                    eraseBullet(bullet[i][0], bullet[i][1] + 1);
                drawBullet(bullet[i][0], bullet[i][1]--);
            }
            else if (bullet[i][1] == 0) {   
                eraseBullet(bullet[i][0], bullet[i][1] + 1);
                bullet[i][0] = -1;
                bullet[i][1] = -1;
                bullet_count -= 1;
            }
        }

        Sleep(100);
    } while (key != 'x');

    setCursor(1);
    setColor(7, 0);

    return 0;
}

void drawShip(int x, int y) 
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , c);
    setColor(2, 4);
    printf("<-w->");
}

void eraseShip(int x, int y) 
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , c);
    setColor(7, 0);
    printf("     ");
}

void drawBullet(int x, int y) 
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , c);
    setColor(7, 0);
    printf("^");
}

void eraseBullet(int x, int y) 
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , c);
    setColor(7, 0);
    printf(" ");
}

void setCursor(int visible)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = 20;
    SetConsoleCursorInfo(console, &lpCursor);
}

void setColor(int fg, int bg)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, bg*16+fg);
}
