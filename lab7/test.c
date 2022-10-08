#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

void drawShip(int x, int y);
void eraseShip(int x, int y);
void drawBullet(int x, int y);
void eraseBullet(int x, int y);
void setCursor(int visible);
void setColor(int fg, int bg);
void drawStar(int x, int y); 
void eraseStar(int x, int y); 
char cursor(int x, int y); 
void printScore(int x, int y, int z);

int main() { 
    srand(time(NULL));
    int bullet[5][2] = {{-1,-1},{-1,-1},{-1,-1},{-1,-1},{-1,-1}};
    char key;
    int bullet_count = 0;
    char move = 'S', shoot = 'F';
    int pos_x = 38, pos_y = 20;
    int g = 0; 
    int Score = 0;
    setCursor(0);
    system("cls"); 
    drawShip(pos_x, pos_y);  
    printScore(1,1,Score);
        int pos_star[20][2];
    for(int i =0;i<20;i++)  
    { 
        pos_star[i][0] = (rand()% (5 - 0 + 1)) +5;
    } 
    for(int i =0;i<20;i++) 
    { 
        pos_star[i][1] = (rand()% (80 - 20 + 1)) +20;
    }  
    for(int i =0;i<20;i++) 
    { 
        drawStar(pos_star[i][1],pos_star[i][0]);
    }

    do {
        printScore(1,1,Score);
        if (_kbhit()) {
            key = _getch();
            if (key == 'a') { move = 'L'; }
            if (key == 'd') { move = 'R'; }
            if (key == 's') { move = 'S'; }
            if (key == ' ') {  shoot = 'T'; }
            fflush(stdin);
       
        } 
        if(move == 'L' && pos_x >0) 
        { 
            eraseShip(pos_x,pos_y);
            drawShip(--pos_x,pos_y); 
        }
        if(move == 'R' && pos_x <150) 
        { 
            eraseShip(pos_x,pos_y);
            drawShip(++pos_x,pos_y); 
        } 
        if(move == 'S') 
        { 
            drawShip(pos_x,pos_y);
        } 
            if (shoot == 'T') {
            for (int i = 0; i < 5; i++)
            {
                if (bullet[i][0] == -1 || bullet[i][1] == -1) {
                    bullet[i][0] = pos_x + 3;
                    bullet[i][1] = pos_y - 1;
                    shoot = 'F';
                    break;
                }
            }
        }
        for (int i = 0; i < 5; i++)
        {
            if (bullet[i][1] > 0) {
                if (bullet[i][1] + 1 != 20)
                    eraseBullet(bullet[i][0], bullet[i][1] + 1); 
                    if(cursor(bullet[i][0], bullet[i][1]-1) == '*') 
                    { 
                         eraseStar(bullet[i][0], bullet[i][1]-1);  
                         eraseBullet(bullet[i][0], bullet[i][1] -1);
                         Beep(7000,500);
                         bullet[i][1] = -1; 
                         bullet[i][0] = -1; 
                         Score++;
                    }
                       
                    else
                        drawBullet(bullet[i][0], bullet[i][1]--);
            }
            else if (bullet[i][1] == 0) {
                eraseBullet(bullet[i][0], bullet[i][1] + 1);
                bullet[i][0] = -1;
                bullet[i][1] = -1;
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
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setColor(2, 4);
    printf("_<-O->_");
}

void eraseShip(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setColor(7, 0);
    printf("        ");
}

void drawBullet(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setColor(7, 0);
    printf("^");
}

void eraseBullet(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setColor(7, 0);
    printf(" ");
}

void eraseStar(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setColor(7, 0);
    printf(" ");
} 

void drawStar(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setColor(7, 0);
    printf("*");
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
    SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
char cursor(int x, int y){
HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
char buf[2]; COORD c = {x,y}; DWORD num_read;
if(
!ReadConsoleOutputCharacter(hStd,(LPTSTR)buf,1,c,(LPDWORD)&num_read) )

return '\0';
else
return buf[0];
}
void printScore(int x, int y, int z) 
{ 
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    setColor(7, 0);
    printf("--- %d ---",z);
}