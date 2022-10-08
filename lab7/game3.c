#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <pthread.h>

#define STAR_NUM 20
#define BULLET_NUM 5

void gotoxy(int x, int y);
void setCursor(int visible);
void setColor(int fg, int bg);
char readCursor(int x, int y);
void drawShip(int x, int y);
void eraseShip(int x, int y);
void drawBullet(int x, int y);
void drawStar(int quantity, int star[quantity][2]);
void erasePoint(int x, int y);
void drawScore(int score);
void randStar(int quantity, int star[quantity][2], int xmin, int xmax, int ymin, int ymax);
int collision(int quanB, int bullet[quanB][2], int quanS, int star[quanS][2], int* bullet_num);
void* beepS(void* threadid) { Beep(800, 200); }
void* beepC(void* threadid) { Beep(400, 200); }

void drawTest(const int* test) 
{
    setColor(7, 0);
    for (int i = 0; i <= 20; i++)
    {
        gotoxy(0, i);
        printf("%d", i);
    }
    gotoxy(0, 21);
    printf("123456789|123456789|123456789|123456789|123456789|123456789|123456789|123456789|");
    gotoxy(0, 22);
    printf("%d", *test);
}

int main() {
    char key = '.', move = 'S', shoot = 'F';
    int score = 0;
    int pos_x = 38, pos_y = 20;
    int bullet[BULLET_NUM][2], bullet_num = 0;
    memset(bullet, -1, sizeof(bullet));
    int star[STAR_NUM][2];
    memset(star, -1, sizeof(star));
    pthread_t thread_id[2];

    srand(time(NULL));
    setCursor(0);
    system("cls");
    randStar(STAR_NUM, star, 10, 70, 2, 5);
    drawShip(pos_x, pos_y);

    do {   
        // drawTest(&bullet_num);
        drawStar(STAR_NUM, star);
        // KEY BOARD
        if (kbhit()) {
            key = getch();
            if (key == 'a') { move = 'L'; }
            if (key == 'd') { move = 'R'; }
            if (key == 's') { move = 'S'; }
            if (key == ' ') { if (bullet_num < BULLET_NUM) shoot = 'T'; }
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
            for (int i = 0; i < BULLET_NUM; i++)
            {   
                if (bullet[i][0] == -1) {
                    bullet[i][0] = pos_x + 2, bullet[i][1] = pos_y - 1;
                    bullet_num += 1;
                    pthread_create(&thread_id[0], NULL, beepS, NULL);
                    shoot = 'F';
                    break;
                }
            }
        }
        // DISPLAY BULLETS
        for (int i = 0; i < BULLET_NUM; i++)
        {
            if (bullet[i][1] > 1) {
                erasePoint(bullet[i][0], bullet[i][1]);
                drawBullet(bullet[i][0], --bullet[i][1]); 
                int ncolli = 0;
                if(ncolli = collision(BULLET_NUM, bullet, STAR_NUM, star, &bullet_num)) 
                { 
                    score += ncolli; 
                    pthread_create(&thread_id[1], NULL, beepC, NULL);
                }
                randStar(STAR_NUM, star, 10, 70, 2, 5);
            }
            else if (bullet[i][1] <= 1 && bullet[i][1] != -1) {   
                erasePoint(bullet[i][0], bullet[i][1]);
                bullet[i][0] = -1, bullet[i][1] = -1;
                bullet_num -= 1;
            }
        }
        drawScore(score);

        Sleep(100);
    } while (key != 'x');

    pthread_exit(NULL);
    setCursor(1);
    setColor(7, 0);
    return 0;
}

void gotoxy(int x, int y)
{
    COORD c = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE) , c);
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

char readCursor(int x, int y){
    HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[2]; 
    COORD c = {x,y}; 
    DWORD num_read;
    if(!ReadConsoleOutputCharacter(hStd,(LPTSTR)buf,1,c,(LPDWORD)&num_read))
        return '\0';
    else
        return buf[0];
}

void drawShip(int x, int y) 
{
    gotoxy(x, y);
    setColor(2, 4);
    printf("<-w->");
    setColor(7, 0);
}

void eraseShip(int x, int y) 
{
    gotoxy(x, y);
    printf("     ");
}

void drawBullet(int x, int y) 
{
    gotoxy(x, y);
    printf("^");
}

void drawStar(int quantity, int star[quantity][2]) 
{
    for (int i = 0; i < quantity; i++)
    {
        if (star[i][0] != -1)
        {
            gotoxy(star[i][0], star[i][1]);
            printf("*");
        }
    }
}

void erasePoint(int x, int y) 
{
    gotoxy(x, y);
    printf(" ");
}

void drawScore(int score)
{   
    int temp = score, digit = 0;
    while (temp > 9)
    {
        temp /= 10;
        digit += 1;
    }

    for (int i = 0; i <= digit; i++) 
        erasePoint(80 - i, 0);
    gotoxy(80 - digit, 0);
    printf("%d", score);
}

void randStar(int quantity, int star[quantity][2], int xmin, int xmax, int ymin, int ymax)
{   
    int x, y;
    char check_pass = 'F';
    for (int i = 0; i < quantity; i++, check_pass = 'F')
    {
        if (star[i][0] == -1 && star[i][1] == -1)
        {
            while (check_pass == 'F') 
            {
                x = (rand() % (xmax-xmin)) + xmin;
                y = (rand() % (ymax-ymin)) + ymin;
                check_pass = 'T';

                for (int j = 0; j < quantity; j++)
                    if (star[j][0] == x && star[j][1] == y) 
                        { check_pass = 'F'; break; }
            }
            star[i][0] = x, star[i][1] = y;
        }
    }
}

int collision(int quanB, int bullet[quanB][2], int quanS, int star[quanS][2], int* bullet_num)
{
    int collision_time = 0;
    for (int i = 0; i < quanB; i++)
    {
        for (int j = 0; j < quanS; j++)
        {
            if (bullet[i][0] == star[j][0] && bullet[i][1] == star[j][1] && bullet[i][0] != -1 && bullet[i][1] != -1)
            {
                erasePoint(star[j][0], star[j][1]);
                erasePoint(bullet[i][0], bullet[i][1]+1);
                star[j][0] = -1, star[j][1] = -1;
                bullet[i][0] = -1, bullet[i][1] = -1;
                *bullet_num -= 1;
                collision_time += 1;
            }
        }
    }
    return collision_time;
}