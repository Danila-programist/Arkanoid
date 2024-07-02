#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#define M_PI 3.14159265358979323846

#define width 65
#define height 25

typedef struct {  		             // создание структуры ракетки 
	int x, y;                            // координаты ракетки
	int w;			             // ширина ракетки
} TRacket;

typedef struct {                             // структура шарика
	float x, y;	
	int	ix, iy;     							// координаты шарика
	float alpha;
	float speed;
} TBall;

char mas[height][width + 1];		     //	игровое поле

TRacket racket;
TBall ball;

void moveBall(float x, float y) {
	ball.x = x;
	ball.y = y;
	ball.ix = (int)round(ball.x);
	ball.iy = (int)round(ball.y);
}

void initBall(void){
	moveBall(2.0,2.0);
	srand(time(NULL));
	ball.alpha = rand()%10;
	ball.speed = 1.0;
}

void initRacket(void){
	racket.w = 7;
	racket.x = (width - racket.w) / 2;
	racket.y = height - 1;
}

void putRacket(void) {
	for (int i = racket.x; i < racket.x + racket.w; i++){
		mas[racket.y][i] = '@';
	}
}

void putBall(void){
	mas[ball.iy][ball.ix] = '*';
}


void autoMoveBall(void){                                                          // автоматическое движение шарика
	if (ball.alpha < 0) ball.alpha += M_PI * 2;
	if (ball.alpha > 2 * M_PI) ball.alpha -= M_PI * 2;
	
	TBall bl = ball;   																	// старое положение шарика

	moveBall(ball.x + cos(ball.alpha) * ball.speed, ball.y + sin(ball.alpha) * ball.speed);

	if ((mas[ball.iy][ball.ix] == '#') || (mas[ball.iy][ball.ix] == '@')){
		if ((ball.ix != bl.ix) && (ball.iy != bl.iy)) {
			if(mas[bl.iy][ball.ix] == mas[ball.iy][bl.ix]){
				bl.alpha = bl.alpha + M_PI;
			}
			else {
				if (mas[bl.iy][ball.ix] == '#')	{
					bl.alpha = (2 * M_PI - bl.alpha) + M_PI;
				}
				else {
					bl.alpha = (2 * M_PI - bl.alpha);
				}							
			}
		}
		else if (ball.iy == bl.iy){
			bl.alpha = (2 * M_PI - bl.alpha) + M_PI;	
		}
		else {
			bl.alpha = (2 * M_PI - bl.alpha);
		}

		ball = bl;
		autoMoveBall();
	}
}

void init(void){                            // инициализация игрового поля
	
	for(int i = 0; i < width; i++){
		mas[0][i] = '#';
	}
	mas[0][width] = '\0';
	
	strncpy(mas[1], mas[0], width + 1);
	for(int i = 1; i < width - 1; i++){
		mas[1][i] = ' ';
	}


	for(int i = 2; i < height; i++){
		strncpy(mas[i], mas[1], width + 1);
	}
}

void show(void){			   // показываем в функции main отработку инициализации игрового поля	
	
	for(int i = 0; i < height; i++){
		printf("%s\n", mas[i]);
	}
}

void moveRacket(int x){        	          // параметр задает левый крайний пиксель ракетки
	racket.x = x;
	if (racket.x < 1){
		racket.x = 1;
	}
	if (racket.x + racket.w >= width){
		racket.x = width - racket.w - 1;
	}
}

void setcur(int x, int y){                                            // плавное движение объектов игры 
	COORD coord;
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition( GetStdHandle(STD_OUTPUT_HANDLE), coord);


	HANDLE  handle = GetStdHandle(STD_OUTPUT_HANDLE); 		// код, убирающий курсор с поля зрения
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(handle, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(handle, &structCursorInfo);
}

int main(void) {
	
	BOOL run = FALSE;
	initRacket();
	initBall();
	
	do
	{
		setcur(0, 0);

		if (run) {
			autoMoveBall();
		}

		init();
		putRacket();
		putBall();
		show();
		if (GetKeyState('A') < 0) moveRacket(racket.x - 3);
		if (GetKeyState('D') < 0) moveRacket(racket.x + 3);
		if (GetKeyState('W') < 0) run = TRUE;

		if (!run) {
			moveBall(racket.x + racket.w / 2, racket.y - 1);
		}
	}
	while (GetKeyState(VK_ESCAPE) >= 0);

	return 0;
}
