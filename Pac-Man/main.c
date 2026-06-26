#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <conio.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define currentMapByte(a) ((uint8_t*)&map)[a]

#define getCoord(a, b) (((coords >> (6 * (a - 'a') ) ) >> b) & 0b111)
#define setCoord(a, b, c) coords = (coords & (~((uint32_t)0b111 << (6 * (a - 'a') + b)))) | (c << (6 * (a - 'a') + b))

#define getMovement (coords >> 30)
#define setMovement(a) coords = (coords & ~(((uint32_t)0b11) << 30)) | (a << 30)

#define _COORD_X 3
#define _COORD_Y 0

#define WALL '#'
#define NON_WALL ' '
#define COIN '.'

#define PACMAN 'a'
#define BLINKY 'b'
#define PINKY 'c'
#define INKY 'd'
#define CLYDE 'e'

#define UP 0b00
#define LEFT 0b01
#define RIGHT 0b10
#define DOWN 0b11

#define ARROW_UP 72
#define ARROW_LEFT 75
#define ARROW_RIGHT 77
#define ARROW_DOWN 80

int main(int argc, char* argv[]) {
    uint64_t map = 18411139144890810879ULL | 103213957120ULL;
    uint32_t coords = DOWN << 30;
    uint8_t coin = 5 << 5 | 5 << 2;

    srand(time(NULL));
    SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING);

    setCoord(PACMAN, _COORD_X, 1); setCoord(PACMAN, _COORD_Y, 1);
    setCoord(BLINKY, _COORD_X, 6); setCoord(BLINKY, _COORD_Y, 1);
    setCoord(PINKY, _COORD_X, 2); setCoord(PINKY, _COORD_Y, 3);
    setCoord(INKY, _COORD_X, 1); setCoord(INKY, _COORD_Y, 6);
    setCoord(CLYDE, _COORD_X, 6); setCoord(CLYDE, _COORD_Y, 6);

    while (1) {
        if (_kbhit() && 0xE0 == _getch()) {
            switch (_getch()) {
            case ARROW_UP:
                setMovement(UP);
                break;
            case ARROW_LEFT:
                setMovement(LEFT);
                break;
            case ARROW_RIGHT:
                setMovement(RIGHT);
                break;
            case ARROW_DOWN:
                setMovement(DOWN);
                break;
            }
        }

        switch (getMovement) {
        case DOWN:
            if (!((currentMapByte(getCoord(PACMAN, _COORD_Y) + 1) >> (getCoord(PACMAN, _COORD_X))) & 1)) {
                setCoord(PACMAN, _COORD_Y, (getCoord(PACMAN, _COORD_Y)) + 1);
                Beep(400, 30);
            }
            break;
        case UP:
            if (!((currentMapByte(getCoord(PACMAN, _COORD_Y) - 1) >> (getCoord(PACMAN, _COORD_X))) & 1)) {
                setCoord(PACMAN, _COORD_Y, (getCoord(PACMAN, _COORD_Y)) - 1);
                Beep(800, 30);
            }
            break;
        case LEFT:
            if (!((currentMapByte((getCoord(PACMAN, _COORD_Y))) >> (getCoord(PACMAN, _COORD_X) - 1)) & 1)) {
                setCoord(PACMAN, _COORD_X, (getCoord(PACMAN, _COORD_X) - 1));
                Beep(500, 30);
            }
            break;
        case RIGHT:
            if (!((currentMapByte((getCoord(PACMAN, _COORD_Y))) >> (getCoord(PACMAN, _COORD_X) + 1)) & 1)) {
                setCoord(PACMAN, _COORD_X, (getCoord(PACMAN, _COORD_X) + 1));
                Beep(600, 30);
            }
            break;
        }

        for (uint8_t i = BLINKY; i <= CLYDE; i++) {
            if (!((currentMapByte(getCoord(i, _COORD_Y) + 1) >> (getCoord(i, _COORD_X))) & 1) && (rand() % 4 == DOWN))
                setCoord(i, _COORD_Y, (getCoord(i, _COORD_Y)) + 1);

            else if (!((currentMapByte(getCoord(i, _COORD_Y) - 1) >> (getCoord(i, _COORD_X))) & 1) && (rand() % 4 == UP))
                setCoord(i, _COORD_Y, (getCoord(i, _COORD_Y)) - 1);

            else if (!((currentMapByte((getCoord(i, _COORD_Y))) >> (getCoord(i, _COORD_X) - 1)) & 1) && (rand() % 4 == LEFT))
                setCoord(i, _COORD_X, (getCoord(i, _COORD_X) - 1));

            else if (!((currentMapByte((getCoord(i, _COORD_Y))) >> (getCoord(i, _COORD_X) + 1)) & 1) && (rand() % 4 == RIGHT))
                setCoord(i, _COORD_X, (getCoord(i, _COORD_X) + 1));
        }

        if ((getCoord(PACMAN, _COORD_X)) == (coin >> 5) && (getCoord(PACMAN, _COORD_Y)) == ((coin >> 2) & 0b111)) {
            coin++;
            if (3 == (coin & 0b11)) goto GameOver;
            do coin = (1 + rand() % 6) << 5 | (1 + rand() % 6) << 2 | coin & 0b11;
            while ((currentMapByte( coin >> 5 ) >> ((coin >> 2) & 0b111)) & 1);
        }

        for (uint8_t i = BLINKY; i <= CLYDE; i++)
            if (getCoord(PACMAN, _COORD_Y) == getCoord(i, _COORD_Y) && getCoord(PACMAN, _COORD_X) == getCoord(i, _COORD_X)) goto GameOver;

        printf("Score: %i\n", coin & 0b11);
        for (uint8_t y = 0; y < sizeof(map); y++) {
            for (uint8_t x = 0; x < 8; x++) {
                if (getCoord(PACMAN, _COORD_X) == x && getCoord(PACMAN, _COORD_Y) == y) printf("\033[33m%c\033[37m", PACMAN);
                else if (getCoord(BLINKY, _COORD_X) == x && getCoord(BLINKY, _COORD_Y) == y) printf("\033[31m%c\033[37m", BLINKY);
                else if (getCoord(PINKY, _COORD_X) == x && getCoord(PINKY, _COORD_Y) == y) printf("\033[35m%c\033[37m", PINKY);
                else if (getCoord(INKY, _COORD_X) == x && getCoord(INKY, _COORD_Y) == y) printf("\033[36m%c\033[37m", INKY);
                else if (getCoord(CLYDE, _COORD_X) == x && getCoord(CLYDE, _COORD_Y) == y)  printf("\033[32m%c\033[37m", CLYDE);
                else if (x == (coin >> 5) && y == ((coin >> 2) & 0b111)) printf("\033[37m%c", COIN);
                else putchar((currentMapByte(y) >> x) & 1 ? WALL : NON_WALL);
            }
            putchar('\n');
        }
        Sleep(270);
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (COORD){ 0, 0 });
    }

GameOver:
    system("cls");
    printf(3 == (coin & 0b11) ? "\033[36mYou Win!\nScore: %i" : "\033[31mGame Over!\nScore: %i", coin & 0b11);
    return 0;
}