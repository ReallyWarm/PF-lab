#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>
#include <time.h>

#define S_COUNT 40
#define MAX_BKEY 5
#define SCREEN_X 80
#define SCREEN_Y 25

struct gameInput {
    char bkey[MAX_BKEY];
    COORD mpos;
    bool lmb;
    bool rmb;
    bool mmv;
};

HANDLE wHnd;
SMALL_RECT windowSize = { 0, 0, SCREEN_X-1, SCREEN_Y-1 };

CHAR_INFO buffer[SCREEN_X * SCREEN_Y];
COORD bufferSize = { SCREEN_X,SCREEN_Y };
COORD characterPos = { 0, 0 };

HANDLE rHnd;
DWORD fdwMode;

int setConsole(int x, int y);
int setMode();
bool handleInput(struct gameInput* input, INPUT_RECORD* eventBuffer, DWORD numEvents, DWORD numEventsRead);
void clearBuffer();
void bufferToConsole();
void initStar(COORD star[S_COUNT]);
void starFall(COORD star[S_COUNT]);
void collision(COORD star[S_COUNT], COORD mpos, int* live);
void starToBuffer(COORD star[S_COUNT]);
void shipToBuffer(COORD mpos, int color);
void liveToBuffer(int num);

int main()
{
    bool play = true;
    int loop_count = 0;
    struct gameInput input;
    input.mpos.X = 40; input.mpos.Y = 24;
    COORD star[S_COUNT];
    memset(star, -1, sizeof(star));

    int live = 10;
    int color = 7;

    INPUT_RECORD* eventBuffer;
    DWORD numEvents = 0;
    DWORD numEventsRead = 0;

    system("cls");
    srand(time(NULL));
    setConsole(SCREEN_X, SCREEN_Y);
    setMode();
    initStar(star);
    while (play)
    {
        GetNumberOfConsoleInputEvents(rHnd, &numEvents);
        if (numEvents != 0) {
            play = handleInput(&input, eventBuffer, numEvents, numEventsRead);
        }
        if (live <= 0) {
            play = false;
        }

        collision(star, input.mpos, &live);
        if (loop_count >= 5) {
            starFall(star);
            loop_count = 0;
        }
        loop_count++;

        for (int i = 0; input.bkey[i] != '\0'; i++)
        {
            if (input.bkey[i] == 'c')
                color = rand() % (15+1 - 1) + 1;
        }
        if (input.lmb == true)
            color = rand() % (15+1 - 1) + 1;

        clearBuffer();
        starToBuffer(star);
        shipToBuffer(input.mpos, color);
        liveToBuffer(live);
        bufferToConsole();
        Sleep(50);
    }
    
    system("cls");
    return 0;
}

int setConsole(int x, int y)
{
    wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
    SetConsoleScreenBufferSize(wHnd, bufferSize);
    return 0;
}

int setMode()
{
    rHnd = GetStdHandle(STD_INPUT_HANDLE);
    fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
    SetConsoleMode(rHnd, fdwMode);
    return 0;
}

bool handleInput(struct gameInput* input, INPUT_RECORD* eventBuffer, DWORD numEvents, DWORD numEventsRead)
{   
    bool play = true;
    eventBuffer = (INPUT_RECORD*) malloc(numEvents * sizeof(*eventBuffer));
    if (eventBuffer == NULL) {
        printf("Out of Memory!");
        return false;
    }

    memset(input->bkey, '\0', MAX_BKEY);
    input->lmb = false;
    input->rmb = false;
    input->mmv = false;

    int count = 0;
    ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
    for (DWORD i = 0; i < numEventsRead; ++i) {
        if (eventBuffer[i].EventType == KEY_EVENT && eventBuffer[i].Event.KeyEvent.bKeyDown == true ) {
            if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
                play = false;
            }
            if (count < MAX_BKEY) {
                input->bkey[count] = eventBuffer[i].Event.KeyEvent.uChar.AsciiChar;
                count++;
            }
        }
        else if (eventBuffer[i].EventType == MOUSE_EVENT) {
            input->mpos.X = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
            input->mpos.Y = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;

            if (eventBuffer[i].Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) {
                input->lmb = true;
            }
            else if (eventBuffer[i].Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) {
                input->rmb = true;
            }
            else if (eventBuffer[i].Event.MouseEvent.dwEventFlags & MOUSE_MOVED) {
                input->mmv = true;
            }
        }
    }
    free(eventBuffer);
    
    return play;
}

void clearBuffer()
{
    for (int y = 0; y < SCREEN_Y; ++y) {
        for (int x = 0; x < SCREEN_X; ++x) {
            buffer[x + SCREEN_X * y].Char.AsciiChar = ' ';
            buffer[x + SCREEN_X * y].Attributes = 7;
        }
    }
}

void bufferToConsole()
{
    WriteConsoleOutputA(wHnd, buffer, bufferSize, characterPos, &windowSize);
}

void initStar(COORD star[S_COUNT])
{
    int x, y;
    for (int i = 0; i < S_COUNT; i++)
    {
        x = rand() % 80;
        y = rand() % 25;
        star[i].X = x, star[i].Y = y;
    }
}

void starFall(COORD star[S_COUNT])
{
    for (int i = 0; i < S_COUNT; i++) {
        if (star[i].Y >= SCREEN_Y-1) {
            star[i].X = rand() % SCREEN_X;
            star[i].Y = 1;
        }
        else {
            star[i].Y = star[i].Y+1;
        }
    }

}

void collision(COORD star[S_COUNT], COORD mpos, int* live)
{
    for (int i = 0; i < S_COUNT; i++) {
        if ((star[i].X == mpos.X-2 || star[i].X == mpos.X-1 || 
            star[i].X == mpos.X || star[i].X == mpos.X+1 ||
            star[i].X == mpos.X+2 ) && star[i].Y == mpos.Y) {
            star[i].Y = SCREEN_Y+1;
            *live -= 1;
        }
    }
}

void starToBuffer(COORD star[S_COUNT])
{
    for (int i = 0; i < S_COUNT; i++) {
        buffer[star[i].X + SCREEN_X * star[i].Y].Char.AsciiChar = '*';
        buffer[star[i].X + SCREEN_X * star[i].Y].Attributes = 7;
    }
}

void shipToBuffer(COORD mpos, int color)
{
    buffer[mpos.X-2 + SCREEN_X * mpos.Y].Char.AsciiChar = '<';
    buffer[mpos.X-2 + SCREEN_X * mpos.Y].Attributes = color;
    buffer[mpos.X-1 + SCREEN_X * mpos.Y].Char.AsciiChar = '-';
    buffer[mpos.X-1 + SCREEN_X * mpos.Y].Attributes = color;
    buffer[mpos.X + SCREEN_X * mpos.Y].Char.AsciiChar = 'O';
    buffer[mpos.X + SCREEN_X * mpos.Y].Attributes = color;
    buffer[mpos.X+1 + SCREEN_X * mpos.Y].Char.AsciiChar = '-';
    buffer[mpos.X+1 + SCREEN_X * mpos.Y].Attributes = color;
    buffer[mpos.X+2 + SCREEN_X * mpos.Y].Char.AsciiChar = '>';
    buffer[mpos.X+2 + SCREEN_X * mpos.Y].Attributes = color;
}

void liveToBuffer(int num)
{   
    int digit = 0; 
    for (int temp = num; temp > 0; temp /= 10)
    {
        buffer[SCREEN_X-1 - digit].Char.AsciiChar = (temp % 10) + '0';
        digit++;
    }
    buffer[SCREEN_X-1].Char.AsciiChar = (num % 10) + '0';
}