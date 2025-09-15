#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdlib.h>

#ifdef _WIN32
	#include <Windows.h>
	#include <conio.h>
#else
	#include "my_conio.h"
#endif

#define COLORS
//#define PRESENT_COLORS

#include <stdint.h>

#ifdef _WIN32
unsigned long get_time_ms() {
    return (unsigned long)GetTickCount64(); // мілісекунди від старту системи
}
void showcursor(bool en)
{
   HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
   CONSOLE_CURSOR_INFO info;
   info.dwSize = 100;
   info.bVisible = en == 1 ? TRUE : FALSE;
   SetConsoleCursorInfo(consoleHandle, &info);
}

#else
#include <time.h>
unsigned long get_time_ms() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (unsigned long)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}
void showcursor(bool en)
{
	if(en)
		ShowConsoleCursor();
	else
		HideConsoleCursor();
}

#endif

#include "tetr.h"

struct str_ttr{
	char *str[20];
	int n;
} ttr;
int w = 0, a = 0, s = 0, d = 0;
int mas_rab[V_LIN + 3][H_LIN + 3];
int fig_tec[4][4] = {0};
int fig_b[4][4] = {0};
int x, y;
int fig_t = 0, fig_p = 0, fig_t_n = 0, fig_p_n = 0, fig_c = 0, fig_c_n = 0;
unsigned long takt, prsc;
unsigned long mls = 0, mls_takt = 0;
int fl_s = 0;
HANDLE heh;
COORD cor = {0, 1};
int vaj;
int vaj_mem;
int vaj_mem_2;
int total;
int max_total;
int color_r;

int mas_pr[V_LIN][H_LIN] = {0};/*
	{0, 0, 1, 0, 0, 1, 0, 0},
	{1, 1, 1, 0, 1, 1, 0, 0},
	{0, 0, 0, 0, 0, 1, 0, 1},
	{1, 1, 1, 1, 0, 0, 0, 1},
	{0, 0, 0, 0, 0, 0, 1, 1},
	{1, 0, 1, 1, 0, 0, 0, 0},
	{1, 1, 0, 1, 0, 1, 1, 0},
	{0, 1, 0, 1, 0, 0, 1, 1}
};*/

char mas_pr2[V_LIN][H_LIN * 2] = {0};
char mas_pr3[V_LIN + 1][(H_LIN * 2) + 2 + PRR] = {0};
short int mas_pr4[V_LIN + 1][(H_LIN * 2) + 2 + PRR] = {0};

const int f[7][4][4] = {
	{{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{0, 0, 0, 0}, {1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{0, 1, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{1, 0, 0, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{0, 0, 1, 0}, {1, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	{{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}
};

#ifdef _WIN32
short int ban_color[] = {0x00};
short int mask_color = {0x0f};
#else
short int ban_color[] = {0x00, 0x10, 0x11, 0x12, 0x13, 0x34, 0x35, 0x36, 0x37, 0x58, 0x59, 0x5a, 0x5b, 0xe8, 0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0x14, 0xef};
short int mask_color = {0xff};
#endif


// Функція генерації випадкових кольорів
short int rand_color(int ccc)
{
	short int aaa;

	do {
		if(!ccc) {
			aaa = (rand() % 255) + 1;
		}
		else {
			aaa = ccc;
			ccc = 0;
		}
		int n =  sizeof(ban_color);
		for(int i = 0; i < n; i++)
			if(aaa == ban_color[i])
				aaa = 0;
		aaa &= mask_color;
	}while(!aaa);
	
	return aaa;
}

int my_getch()
#ifdef _WIN32
{
	int res = _getch();
	if (res == 224) {
		res = 256 + _getch();
	}
	return res;
}
#else
{
    int c = _getch();
    if (c == 27) { // ESC
        int c1 = _getch();
        if (c1 == 91) { // '['
            int c2 = _getch();
            switch (c2) {
                case 'A': return 328; // up
                case 'B': return 336; // down
                case 'C': return 333; // right
                case 'D': return 331; // left
            }
        }
        return 27; // просто ESC
    }
    return c;
}
#endif

void rot(int n_fig_p) 
{
	int i, j;
	if(fig_t < 2) {
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++) {
				if(n_fig_p % 2) fig_b[i][j] = f[fig_t][i][j] * fig_c;
				else fig_b[i][j] = f[fig_t][j][i] * fig_c;
			}
	}else{
		/* for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++) 
				if(i == 3 || j == 3)
					fig_b[i][j] = 0; */
		fig_b[1][3] = 0;
		fig_b[3][1] = 0;
		if(fig_t < 5)
			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++) {
					if(!(n_fig_p % 4)) fig_b[i][j] = f[fig_t][i][j] * fig_c;
					else if (n_fig_p % 4 == 1) fig_b[i][j] = f[fig_t][2 - j][i] * fig_c;
					else if (n_fig_p % 4 == 2) fig_b[i][j] = f[fig_t][2 - i][2 - j] * fig_c;
					else fig_b[i][j] = f[fig_t][j][2 - i] * fig_c;
				}
		else
			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++) {
					if(!(n_fig_p % 2)) fig_b[i][j] = f[fig_t][i][j] * fig_c;
					else fig_b[i][j] = f[fig_t][2 - j][i] * fig_c;
				}
	}			
}

void gen(int prnt) {
	static int pr = 0;
	int i, j;
	char next[9] = {0};
//	total++;
	x = 2;
	y = (H_LIN / 2) + 1;
	fig_t = fig_t_n;
	fig_p = fig_p_n;
	fig_c = fig_c_n;
	fig_t_n = (w + a + s + d + fig_t_n + fig_p_n + (rand() % 7)) % 7;
	fig_p_n = (w + a + s + d + fig_t_n + fig_p_n + (takt % 4)) % 4;
	fig_c_n = rand_color(0);
	if(prnt) {
		COORD cor_n {PRR + (H_LIN) * 2 + 5, 1};
		int mem = fig_t;
		int mem_c = fig_c;
		total++;
		pr = 0;
		fig_t = fig_t_n;
		fig_c = fig_c_n;
		rot(fig_p_n);
		SetConsoleCursorPosition(heh, cor_n);
		printf(" NEXT");
		cor_n.Y++;
		for(i = 0; i < 4; i++) {
			cor_n.Y++;
			for(j = 0; j < 4; j++)
				next[j * 2] = next[j * 2 + 1] = fig_b[i][j] != 0 ? -37 : 32;
			SetConsoleCursorPosition(heh, cor_n);
#ifndef COLORS
			puts(next);
#else
			for(int k = 0; k < 8; k++) {
				SetConsoleTextAttribute(heh, fig_b[i][k / 2]);
#ifdef MY_CONIO_H
				my_putchar(next[k]);
#else
				putchar(next[k]);
#endif
			}
			printf("\n");
			SetConsoleTextAttribute(heh, 15);
#endif

		}//_getch();
		fig_t = mem;
		fig_c = mem_c;
		rot(fig_p);
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++)
				fig_tec[i][j] = fig_b[i][j];
		if(fig_tec[0][0] + fig_tec[0][1] + fig_tec[0][2] + fig_tec[0][3] == 0)
			x--;
		SetConsoleCursorPosition(heh, {PRR + (H_LIN) * 2 + 5, 8});
		printf("level %d", vaj_mem_2);
		SetConsoleCursorPosition(heh, {PRR + (H_LIN) * 2 + 5, 10});
		printf("speed %d", (shvydkist - takt) / 40);
			
	}else {
		if(pr)
			return;
		for(j = 1; j < 7; j++) {
			SetConsoleCursorPosition(heh, {(H_LIN) * 2 + 5, j});
			printf("         ");
		}
		pr = 1;
	}
}

void prnt(int ff)
{//getch();
	int i, j;
	SetConsoleCursorPosition(heh, cor);
	for(i = 0; i < V_LIN; i++) {
		for(j = 0; j < H_LIN; j++) {
			if(mas_pr[i][j]) {
				mas_pr2[i][j * 2] = -37;
				mas_pr2[i][(j * 2) + 1] = -37;
				mas_pr4[i][(j * 2) + PRR] = mas_pr[i][j];
				mas_pr4[i][(j * 2) + PRR + 1] = mas_pr[i][j];
				//printf("%02X", mas_pr[i][j]);
			}else{
				mas_pr2[i][j * 2] = 32;
				mas_pr2[i][(j * 2) + 1] = 32;
				mas_pr4[i][(j * 2) + PRR] = 15;
				mas_pr4[i][(j * 2) + PRR + 1] = 15;
				//printf("  ");
			}
		}
		//printf("\n");
	}
	for(i = 0; i <= V_LIN; i++)
		for(j = 0; j <= (H_LIN * 2) + PRR; j++) {
			if(i < V_LIN) {
				if(j < (PRR - 1)) {
					mas_pr3[i][j] = 32;
					mas_pr4[i][j] = 15;
				}
				else if(j == (PRR - 1) || j == ((H_LIN * 2) + PRR)) {
					mas_pr3[i][j] = -70;
					mas_pr4[i][j] = color_r;
				}
				else {
					mas_pr3[i][j] = mas_pr2[i][j - PRR];
					//mas_pr4[i][j] = mas_pr[i][j - PRR];
				}
			}else{
				if(j < (PRR - 1)) {
					mas_pr3[i][j] = 32;
					mas_pr4[i][j] = 15;
				}
				else if(j == (PRR - 1)) {
					mas_pr3[i][j] = -56;
					mas_pr4[i][j] = color_r;
				}
				else if(j == ((H_LIN * 2) + PRR)) {
					mas_pr3[i][j] = -68;
					mas_pr4[i][j] = color_r;
				}
				else {
					mas_pr3[i][j] = -51;
					mas_pr4[i][j] = color_r;
				}
			}
		}/*
				if(true) {
					titry();
			//		ttr.n--;
					strcpy(mas_pr3[2], ttr.str[0]);
				}
		//*/
	// Кольоровий вивід замість puts()
#ifndef COLORS
	for(i = 0; i <= V_LIN; i++) 
		puts(mas_pr3[i]);
#else
	showcursor(0);
	DWORD written;
	for(i = 0; i <= V_LIN; i++){
		CHAR* m3 = (CHAR*)mas_pr3[i];
		WORD* m4 = (WORD*)mas_pr4[i];
		WriteConsoleOutputCharacterA(heh, m3, (H_LIN * 2) + PRR + 1, {0, i}, &written);
		WriteConsoleOutputAttribute(heh, m4, (H_LIN * 2) + PRR + 1, {0, i}, &written);
	}
#endif
	showcursor(1);
	SetConsoleCursorPosition(heh, {PRR, V_LIN + 2});
	if(ff)
		printf("total % 8d", total);
}

int scan()
{
	int q = 0;
	int ff = 0;
	if(_kbhit()) {
		while(true) {
			q = my_getch();
			if(q == 112 || q == 337 || q == 329) continue;
			break;
		}
	}
	if(q == 119 || q == 328) {
		if(!w) ff = 1;
		w = 1;
	}else w = 0;
	if(q == 97 || q == 331) {
		if(!a) ff = 1;
		a = 1;
	}else a = 0;
	if(q == 115 || q == 336) {
		if(!s) ff = 1;
		s = 1;
	}else s = 0;
	if(q == 100 || q == 333) {
		if(!d) ff = 1;
		d = 1;
	}else d = 0;
	return ff;
}

void gen_vaj()
{
	if(vaj_mem_2 > vaj_mem) {
		int j = 1;
		for(; j <= vaj_mem_2; j++)
			for(int i = 1; i <= H_LIN; i++)
				mas_rab[V_LIN - j + 1][i] = (rand() % 3 ? 0 : 1) * rand_color(0);
		vaj = vaj_mem = vaj_mem_2;
		for(; V_LIN - j >= 0; j++)
			for(int i = 1; i <= H_LIN; i++)
				mas_rab[V_LIN - j + 1][i] = 0;
	}
	else if(vaj_mem != vaj) {
		if(vaj > vaj_mem)
			for(int i = 1; i <= H_LIN; i++) {
				mas_rab[V_LIN - vaj + 1][i] = (rand() % 3 ? 0 : 1) * rand_color(0);
			}
		else
			for(int i = 1; i <= H_LIN; i++) {
				mas_rab[V_LIN - vaj][i] = 0;
			}
		vaj_mem_2 = vaj_mem = vaj;
	}
}

void setup(int no_adv)
{
	int i, j;
	vaj = 0;
	vaj_mem = 0;
	if(no_adv || vaj_mem_2 == 1) {
		if(max_total < total)
			max_total = total;
		total = 0;
	}
	SetConsoleOutputCP(860);
	color_r = rand_color(0x1b);
	srand((unsigned int)time(NULL));
	for(i = 0; i < (V_LIN + 3); i++)
		for(j = 0; j < (H_LIN + 3); j++) {
			if(!i || !j || i == (V_LIN + 1) || j == (H_LIN + 1) || i == (V_LIN + 2) || j == (H_LIN + 2)) mas_rab[i][j] = 1;
			else mas_rab[i][j] = 0;
		}
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			fig_tec[i][j] = 0;
	if(!no_adv && vaj_mem_2 != 18)
		vaj_mem_2++;
	SetConsoleCursorPosition(heh, {PRR + (H_LIN) * 2 + 5, 8});
	printf("level %d", vaj_mem_2);
	SetConsoleCursorPosition(heh, {PRR + (H_LIN) * 2 + 5, 10});
	printf("               ");
	SetConsoleCursorPosition(heh, {PRR + (H_LIN) * 2 + 5, 12});
	printf("max %d", max_total);
	gen(0);
	if(vaj_mem_2 > 1)
		do{gen_vaj();}while(prov_lin());
	else
		gen_vaj();
	prnt_podg(0);
	showcursor(0);
	while(no_adv) {
		gen(0);
		gen_vaj();
		prnt_podg(0);
//		prnt(0);
		SetConsoleCursorPosition(heh, {PRR, V_LIN + 2});
		printf("vajkist %-2d       ", vaj);
		if(scan()) {
			if(w && vaj < V_LIN - 4)
				vaj++;
			else if(s && vaj > 0)
				vaj--;
			else if(s || w)
				continue;
			else
				break;
		}
	}
	showcursor(1);
	if(!no_adv)
		_getch();
	takt = shvydkist;
	prsc = pryskorennia;
	gen(1);
	x--;
	fl_s = w = s = a = d = 0;
	prnt_podg(1);
}

int prov_lin()
{
	int brbr = 0;
	int i, j, ii, jj;
	int ff = 0;
	for(i = 1; i <= V_LIN; i++)
		for(j = 1; j <= H_LIN; j++) {
			if(!mas_rab[i][j]) break;
			if(j == H_LIN) {
				total += V_LIN - j + 1;
				brbr++;
				for(ii = i; ii > 1; ii--)
					for(jj = 1; jj < (H_LIN + 1); jj++) mas_rab[ii][jj] = mas_rab[ii - 1][jj];
				for(jj = 1; jj < (H_LIN + 1); jj++) mas_rab[1][jj] = 0;
			}
		}
	switch(brbr) {
		case 0:
			break;
		case 1:
			total += 10;
			break;
		case 2:
			total += 20;
			break;
		case 3:
			total += 40;
			break;
		case 4:
			total += 100;
			break;
		default:
			break;
	}
	/* for(i = 1; i <= V_LIN - 3; i++)
		for(j = 1; j <= H_LIN; j++) 
		//	if(i <= V_LIN - 3)
				ff += mas_rab[i][j];
	if(!ff) {
		for(j = 1; j <= H_LIN; j++) {
			if(!mas_rab[V_LIN][j] && mas_rab[V_LIN - 1][j])
				return 0;
			if(!mas_rab[V_LIN - 1][j] && mas_rab[V_LIN - 2][j])
				return 0;
		}
		return 1;
	} */
	for(j = 1; j <= H_LIN; j++) {
		ff = 1;
		for(i = V_LIN; i > 1; i--) {
			if(ff && !mas_rab[i][j])
				ff = 0;
			else if(!ff && mas_rab[i][j])
				return 0;
		}
	}
	return 1;
}

int prov_krok(int new_x, int new_y)
{
	int i, j;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			if(fig_tec[i][j] && mas_rab[new_x + i - 1][new_y + j - 1]) return 0;
	return 1;
}

int prov_rot(int new_fig_p)
{
	int i, j;
	int ii, jj;
	int ff = 0;
	rot(new_fig_p % 4);
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			if(fig_b[i][j] && mas_rab[x + i - 1][y + j - 1]) 
				if(x == 1) {
					x++;
			//		return prov_rot(new_fig_p);
					if(prov_rot(new_fig_p))
						return 1;
					else {
						x--;
						return 0;
					}
				}
				else {
					if(j == 0) {
						for(ii = 0; ii < 4; ii++)
							for(jj = 0; jj < 4; jj++)						
								if(fig_b[ii][jj] && mas_rab[x + ii - 1][y + jj])
									return 0;
						y++;
						for(i = 0; i < 4; i++)
							for(j = 0; j < 4; j++)
								fig_tec[i][j] = fig_b[i][j];
						return 1;
					}
					else if(fig_t == 1 && new_fig_p % 2 == 1) {
						for(ii = 0; ii < 4; ii++)						
							if(fig_b[1][ii] && mas_rab[x][y + ii - 2])
								break;
						for(jj = 0; jj < 4; jj++)
							if(fig_b[1][jj] && mas_rab[x][y + jj - 3])
								break;
						if(ii == 4) {
							y--;
							for(i = 0; i < 4; i++)
								for(j = 0; j < 4; j++)
									fig_tec[i][j] = fig_b[i][j];
							return 1;
						}
						else if(jj == 4) {
							y -= 2;
							for(i = 0; i < 4; i++)
								for(j = 0; j < 4; j++)
									fig_tec[i][j] = fig_b[i][j];
							return 1;
						}
						else 
							return 0;
					}
					else if(j == 2) {
						for(ii = 0; ii < 3; ii++)
							for(jj = 0; jj < 3; jj++)						
								if(fig_b[ii][jj] && mas_rab[x + ii - 1][y + jj - 2])
									return 0;
						y--;
						for(i = 0; i < 4; i++)
							for(j = 0; j < 4; j++)
								fig_tec[i][j] = fig_b[i][j];
						return 1;
					}
					else
						return 0;
					
				}
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			fig_tec[i][j] = fig_b[i][j];
	return 1;
}

void prnt_podg(int ff)
{
	int i, j;
	for(i = 1; i < (V_LIN + 1); i++)
		for(j = 1; j < (H_LIN + 1); j++) {
			mas_pr[i - 1][j - 1] = mas_rab[i][j];
			if(i >= x - 1 && i <= x + 2 && j >= y - 1 && j <= y + 2)
				mas_pr[i - 1][j - 1] = fig_tec[i - x + 1][j - y + 1] != 0 ? fig_tec[i - x + 1][j - y + 1] : mas_rab[i][j];
		}
	prnt(ff);
}
/*
void titry()
{
	ttr.n = 0;
	free(ttr.str[ttr.n]);
	ttr.str[ttr.n] = (char*)malloc(33);
	fprintf((FILE*)ttr.str[ttr.n], "%d", takt);
}//*/

int loop(int no_adv)
{
	if(scan()) {
		if(s == 1) fl_s = 1;
		else if(a == 1) {
			if(prov_krok(x, y - 1)) {
				y--;
				prnt_podg(1);
			}
		}
		else if(d == 1) {
			if(prov_krok(x, y + 1)) {
				y++;
				prnt_podg(1);
			}
		}
		else if(w == 1) {
			if(prov_rot(fig_p + 1)) {
				fig_p = (fig_p + 1) % 4;
				prnt_podg(1);
			}
		}
	}
//	else prnt(0);
	if(!fl_s && get_time_ms() - mls_takt >= prsc) {
		mls_takt = (unsigned long)get_time_ms();
		if (takt > 40) {
			takt--;
			prsc++;
		}
		else {
			takt = shvydkist;
			prsc = pryskorennia;
		}
	}
	if(fl_s || get_time_ms() - mls >= takt) {
		int i, j;
		if(!fl_s) {
			mls = (unsigned long)get_time_ms();
		}
		if(prov_krok(x + 1, y)) {
			x++;
			prnt_podg(1);
		}else{
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					mas_rab[x + i - 1][y + j - 1] = fig_tec[i][j] != 0 ? fig_tec[i][j] : mas_rab[x + i - 1][y + j - 1];
			if(prov_lin() && !no_adv) {
				total += vaj_mem_2 * 100;
				setup(0);
//				_getch();
				return 1;
			}
			gen(1);
			if(!prov_krok(x, y)) {
				takt = 100;
				prnt_podg(1);
				while(true){
//					prnt(0);
					if((clock() - mls) >= takt) break;
				}
				return 0;
			}
			prnt_podg(1);
			if(fl_s) {
				fl_s = 0;
				mls = clock();
			}
			
		}
	}
//	prnt(0);
	return 1;
}

int main(int q, char** qq)
{
//	if(q != 2)
//		return 0;
	vaj_mem_2 = 0;
	max_total = 0;
#ifdef MY_CONIO_H
	cls();
#endif
	printf(" \'t\' - adventure   ");
	heh = GetStdHandle(STD_OUTPUT_HANDLE);
#ifdef PRESENT_COLORS
	SetConsoleOutputCP(860);
	int i;
	printf("\n   x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 xA xB xC xD xE xF");
	for(i = 0; i < 256; i++) {
		if(i % 16 == 0)
			printf("\n%Xx ", i / 16);

		SetConsoleTextAttribute(heh, rand_color(i));//Sleep(10);
#ifdef MY_CONIO_H
		my_putchar(-37);
		my_putchar(-37);
		my_putchar(32);
#else
		putchar(-37);
		putchar(-37);
		putchar(32);
#endif
		SetConsoleTextAttribute(heh, 15);
	}
	SetConsoleOutputCP(1251);
#endif

	if(_getch() == 't') {
		printf("%c                  ", 13);
		do{
			vaj_mem_2 = 1;
			setup(0);
			do{}while(loop(0));
		}while(_getche() != 27);
	}else {
		printf("%c                  ", 13);
		do{	
			setup(1);
			do{}while(loop(1));
		}while(_getche() != 27);
	}
	return 0;
}







