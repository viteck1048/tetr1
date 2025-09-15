#ifndef MY_CONIO_H
#define MY_CONIO_H

#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>

#undef puts
#define puts my_puts

typedef int HANDLE;
typedef int DWORD;
typedef char CHAR;
typedef short int WORD;
typedef struct {
	int X, Y;
} COORD;

int cpglobal = 1251;

// ===== getch =====
int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt);
    ch = getchar();
//    tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
    return ch;
}

int _getch() {
	return getch();
}

// ===== getche =====
int getche() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ICANON;
    newt.c_lflag |= ECHO;
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &newt);
    ch = getchar();
//    tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldt);
    return ch;
}

int _getche() {
	return getche();
}

// ===== kbhit =====

int kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int _kbhit() {
	return kbhit();
}

// ===== gotoxy =====
void gotoxy(int x, int y) {
	if(x < 0 || y < 0 || x > 1000 || y > 1000)
		return;
    printf("\033[%d;%dH", y + 1, x + 1);
}

void gotoyx(int y, int x) {
	gotoxy(x, y);
}

void SetConsoleCursorPosition(int non, COORD cor) {
	gotoxy(cor.X, cor.Y);
}

// ===== clrscr =====
void clrscr() {
    printf("\033[2J\033[1;1H");
}

void cls() {
	clrscr();
}

int GetStdHandle(int non) {
	return non;
}

#define STD_OUTPUT_HANDLE 111

int SetConsoleOutputCP(int cp) {
	cpglobal = cp;
	return 0;
}

// ===== SetConsoleTextAttribute =====
// Функція для встановлення кольору тексту консолі (Linux еквівалент Windows API)
int SetConsoleTextAttribute(int handle, int _color) {
	int color = _color & 0xFF;

    if (color == 15 || color == 0) {
		// Скидаємо до стандартного кольору
		printf("\033[0m");
		return 0;
	}
	
	else {
		// Використовуємо 256-колірну палітру ANSI для всіх кольорів
		printf("\033[38;5;%dm", color);
		return 0;
	}
	
	// За замовчуванням - білий колір
	printf("\033[37m");
	return 0;
}

// ===== HideConsoleCursor =====
// Функція для вимкнення видимості курсора (Linux еквівалент Windows API)
int HideConsoleCursor() {
	printf("\e[?25l"); // вимкнення видимості курсора
	return 0;
}

// ===== ShowConsoleCursor =====
// Функція для увімкнення видимості курсора (Linux еквівалент Windows API)
int ShowConsoleCursor() {
	printf("\e[?25h"); // увімкнення видимості курсора
	return 0;
}

typedef struct {
    unsigned char cp;
    const char* utf8;
} cpmap_t;

static const cpmap_t cp437_map[] = {
    {219, "\xE2\x96\x88"}, // █ full block
    {220, "\xE2\x96\x84"}, // ▄ lower half
    {223, "\xE2\x96\x80"}, // ▀ upper half
    {196, "\xE2\x94\x80"}, // ─
    {179, "\xE2\x94\x82"}, // │
    {218, "\xE2\x94\x8C"}, // ┌
    {191, "\xE2\x94\x90"}, // ┐
    {192, "\xE2\x94\x94"}, // └
    {217, "\xE2\x94\x98"}, // ┘
    {195, "\xE2\x94\x9C"}, // ├
    {180, "\xE2\x94\xA4"}, // ┤
    {194, "\xE2\x94\xAC"}, // ┬
    {193, "\xE2\x94\xB4"}, // ┴
    {197, "\xE2\x94\xBC"} // ┼
};

static const cpmap_t cp860_map[] = {
    {219, "\xE2\x96\x88"}, // █ full block
    {220, "\xE2\x96\x84"}, // ▄ lower half
    {223, "\xE2\x96\x80"}, // ▀ upper half
    {176, "\xE2\x96\x91"}, // ░ light shade
    {177, "\xE2\x96\x92"}, // ▒ medium shade
    {178, "\xE2\x96\x93"}, // ▓ dark shade

    // одинарні лінії
    {196, "\xE2\x94\x80"}, // ─
    {179, "\xE2\x94\x82"}, // │
    {218, "\xE2\x94\x8C"}, // ┌
    {191, "\xE2\x94\x90"}, // ┐
    {192, "\xE2\x94\x94"}, // └
    {217, "\xE2\x94\x98"}, // ┘
    {195, "\xE2\x94\x9C"}, // ├
    {180, "\xE2\x94\xA4"}, // ┤
    {194, "\xE2\x94\xAC"}, // ┬
    {193, "\xE2\x94\xB4"}, // ┴
    {197, "\xE2\x94\xBC"}, // ┼

    // подвійні лінії
    {205, "\xE2\x95\x90"}, // ═
    {186, "\xE2\x95\x91"}, // ║
    {201, "\xE2\x95\x94"}, // ╔
    {187, "\xE2\x95\x97"}, // ╗
    {200, "\xE2\x95\x9A"}, // ╚
    {188, "\xE2\x95\x9D"}, // ╝
    {204, "\xE2\x95\x9E"}, // ╠
    {185, "\xE2\x95\xA3"}, // ╣
    {203, "\xE2\x95\xA4"}, // ╦
    {202, "\xE2\x95\xA7"}, // ╩
    {206, "\xE2\x95\xAC"}  // ╬
};


static const int cp437_map_size = sizeof(cp437_map) / sizeof(cpmap_t);

static const int cp860_map_size = sizeof(cp860_map) / sizeof(cpmap_t);

const char* cp_to_utf8(unsigned char c) {
	if(cpglobal == 437) {
		for (int i = 0; i < cp437_map_size; i++) {
			if (cp437_map[i].cp == c) return cp437_map[i].utf8;
		}
	}
	else if(cpglobal == 860) {
		for (int i = 0; i < cp860_map_size; i++) {
			if (cp860_map[i].cp == c) return cp860_map[i].utf8;
		}
	}
	return "?"; // fallback
}

int my_putchar(int c) {
    if(c > 0 && c < 128) {
		return putchar(c);
	}
	else {
        printf("%s", cp_to_utf8(c));
        return 0;
		//return putchar((int)cp_to_utf8((unsigned char)c));
	}
}

int my_puts(const char* str) {
    printf("\e[?25l");
	char buf[1024];
    buf[0] = 0;

    for (int i = 0; str[i]; i++) {
        unsigned char c = (unsigned char)str[i];
        if (c < 128) {
            size_t len = strlen(buf);
            buf[len] = c;
            buf[len + 1] = 0;
        } else {
		strcat(buf, cp_to_utf8(c));
        }
    }
	size_t len = strlen(buf);
	buf[len] = '\n';
 	buf[len + 1] = 0;
	int ret = fputs(buf, stdout);
	printf("\e[?25h");
	return ret;
}


char stringConsoleOutput[1024];

bool WriteConsoleOutputCharacterA(int h, char* arr, int len, COORD cor, int* w) {
	int i = 0;
	for(; i < len; i++)
		stringConsoleOutput[i] = *(arr + i);
	for(; i < 1024; i++)
		stringConsoleOutput[i] = 0;
	return 0;
}


bool WriteConsoleOutputAttribute(int h, WORD* arr, int len, COORD cor, int* w) {
	for(int i = 0; i < len; i++) {
		SetConsoleTextAttribute(h, *(arr + i));
		my_putchar(stringConsoleOutput[i]);
	}
	putchar('\n');
	SetConsoleTextAttribute(h, 15);
	return 0;
}

#endif // MY_CONIO_H

