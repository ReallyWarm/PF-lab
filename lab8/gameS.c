#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>
#include <pthread.h>

#define STAR_NUM 20
#define BULLET_NUM 5
#define SCREEN_X 80
#define SCREEN_Y 25

struct player {
    int x;
    int y;
    char movement;
    char shooting;
};
struct bullet { 
    int x; 
    int y; 
};
struct star { 
    int x; 
    int y; 
};

void setConsole(int screen_x, int screen_y);
void gotoxy(int x, int y);
void setCursor(int visible);
void setColor(int fg, int bg);
char readCursor(int x, int y);
void drawShip(int x, int y);
void eraseShip(int x, int y);
void drawBullet(int x, int y);
void drawStar(int maxS, struct star* stars);
void erasePoint(int x, int y);
void drawScore(int score, int screen_x);
void randStar(int maxS, struct star* stars, int xmin, int xmax, int ymin, int ymax);
int collision(int maxB, struct bullet* bullets, int maxS, struct star* stars, int* bullet_num);
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
    char key = '.';
    int score = 0;
    int bullet_num = 0;

    struct player p;
    p.x = 38; p.y = 20; p.movement = 'S'; p.shooting = 'F';
    struct bullet bullets[BULLET_NUM]; 
    memset(bullets, -1, sizeof(bullets));
    struct star stars[STAR_NUM];
    memset(stars, -1, sizeof(stars));
    
    pthread_t thread_id[2];
    CHAR_INFO consoleBuffer[SCREEN_X * SCREEN_Y];

    setConsole(SCREEN_X, SCREEN_Y);
    setCursor(0);
    srand(time(NULL));
    system("cls");
    randStar(STAR_NUM, stars, 10, 70, 2, 5);
    drawShip(p.x, p.y);

    do {   
        // drawTest(&bullet_num);
        drawStar(STAR_NUM, stars);
        // KEY BOARD
        if (kbhit()) {
            key = getch();
            if (key == 'a') { p.movement = 'L'; }
            if (key == 'd') { p.movement = 'R'; }
            if (key == 's') { p.movement = 'S'; }
            if (key == ' ') { if (bullet_num < BULLET_NUM) p.shooting = 'T'; }
            fflush(stdin);
        }
        // MOVING
        if (p.movement == 'L' && p.x > 0) {   
            eraseShip(p.x, p.y);
            drawShip(--p.x, p.y);
        }
        else if (p.movement == 'R' && p.x < 80 - 5) {
            eraseShip(p.x, p.y);
            drawShip(++p.x, p.y);
        }
        else {
            p.movement == 'S';
        }
        // SHOOTING
        if (p.shooting == 'T') {
            for (int i = 0; i < BULLET_NUM; i++)
            {   
                if (bullets[i].x == -1) {
                    bullets[i].x = p.x + 2, bullets[i].y = p.y - 1;
                    pthread_create(&thread_id[0], NULL, beepS, NULL);
                    bullet_num += 1;
                    p.shooting = 'F';
                    break;
                }
            }
        }
        // DISPLAY BULLETS
        for (int i = 0; i < BULLET_NUM; i++)
        {
            if (bullets[i].y > 1) {
                erasePoint(bullets[i].x, bullets[i].y);
                drawBullet(bullets[i].x, --bullets[i].y); 
                int ncolli = 0;
                if(ncolli = collision(BULLET_NUM, bullets, STAR_NUM, stars, &bullet_num)) 
                { 
                    score += ncolli; 
                    pthread_create(&thread_id[1], NULL, beepC, NULL);
                }
                randStar(STAR_NUM, stars, 10, 70, 2, 5);
            }
            else if (bullets[i].y <= 1 && bullets[i].y != -1) {   
                erasePoint(bullets[i].x, bullets[i].y);
                bullets[i].x = -1, bullets[i].y = -1;
                bullet_num -= 1;
            }
        }
        drawScore(score, SCREEN_X);

        Sleep(100);
    } while (key != 'x');

    system("cls");
    setCursor(1);
    setColor(7, 0);
    pthread_exit(NULL);
    return 0;
}

void setConsole(int screen_x, int screen_y)
{
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD bufferSize = { screen_x, screen_y };
    SMALL_RECT windowSize = { 0, 0, screen_x - 1, screen_y - 1 };
    SetConsoleWindowInfo(console, TRUE, &windowSize);
    SetConsoleScreenBufferSize(console, bufferSize);
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
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(console, bg*16+fg);
}

char readCursor(int x, int y){
    HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
    char buf[2]; 
    COORD c = {x,y}; 
    DWORD num_read;
    if(!ReadConsoleOutputCharacter(console,(LPTSTR) buf, 1, c, (LPDWORD) &num_read))
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

void drawStar(int maxS, struct star* stars) 
{
    for (int i = 0; i < maxS; i++)
    {
        if (stars[i].x != -1) {
            gotoxy(stars[i].x, stars[i].y);
            printf("*");
        }
    }
}

void erasePoint(int x, int y) 
{
    gotoxy(x, y);
    printf(" ");
}

void drawScore(int score, int screen_x)
{   
    int temp = score, digit = 0;
    while (temp > 9)
    {
        temp /= 10;
        digit += 1;
    }

    for (int i = 0; i <= digit; i++) 
    {
        erasePoint(screen_x-1 - i, 0);
    }
    gotoxy(screen_x-1 - digit, 0);
    printf("%d", score);
}

void randStar(int maxS, struct star* stars, int xmin, int xmax, int ymin, int ymax)
{   
    int x, y;
    for (int i = 0; i < maxS; i++)
    {
        while (stars[i].x == -1 && stars[i].y == -1) 
        {
            x = (rand() % (xmax-xmin)) + xmin;
            y = (rand() % (ymax-ymin)) + ymin;
            for (int j = 0; j < maxS; j++) 
            {
                if (stars[j].x == x && stars[j].y == y) { 
                    x = -1; y= -1;
                    break; 
                }
            }
            stars[i].x = x, stars[i].y = y;
        }   
    }
}

int collision(int maxB, struct bullet* bullets, int maxS, struct star* stars, int* bullet_num)
{
    int collision_time = 0;
    for (int i = 0; i < maxB; i++)
    {
        for (int j = 0; j < maxS; j++)
        {
            if (bullets[i].x == stars[j].x && bullets[i].y == stars[j].y && bullets[i].x != -1 && bullets[i].y != -1)
            {
                erasePoint(stars[j].x, stars[j].y);
                erasePoint(bullets[i].x, bullets[i].y+1);
                stars[j].x = -1, stars[j].y = -1;
                bullets[i].x = -1, bullets[i].y = -1;
                *bullet_num -= 1;
                collision_time += 1;
            }
        }
    }

    return collision_time;
}