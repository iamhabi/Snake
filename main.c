#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77


struct Snake {
	int len;
	int x[1000];
	int y[1000];
} Snake ;

struct Apple{
	int x;
	int y;
} Apple ;

int dir;
int BoardSize;

void HideCursor() {
	CONSOLE_CURSOR_INFO ConsoleCursor;
	ConsoleCursor.bVisible = 0;
	ConsoleCursor.dwSize = 1;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &ConsoleCursor);
}

void gotoxy(int x, int y) {
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DrawBoard(int len) {
	for (int i = 0; i < len * 2; i += 2) {
		gotoxy(i, 0);
		printf("бс");
		gotoxy(i, len - 1);
		printf("бс");
	}

	for (int i = 1; i < len; i++) {
		gotoxy(0, i);
		printf("бс");
		gotoxy(len * 2 - 2, i);
		printf("бс");
	}
}

void MoveSnake(int dir) {
	gotoxy(Snake.x[Snake.len - 1], Snake.y[Snake.len - 1]);
	printf("  ");

	for (int i = Snake.len - 1; i > 0; i--) {
		Snake.x[i] = Snake.x[i - 1];
		Snake.y[i] = Snake.y[i - 1];
	}

	switch (dir) {
	case UP:    Snake.y[0]--;    break;
	case DOWN:  Snake.y[0]++;    break;
	case LEFT:  Snake.x[0] -= 2; break;
	case RIGHT: Snake.x[0] += 2; break;
	}

	gotoxy(Snake.x[0], Snake.y[0]);
	printf("бс");
}

int SnakeBodyCheck() {
	if (Snake.x[0] == 0 || Snake.x[0] == BoardSize * 2 - 2 || Snake.y[0] == 0 || Snake.y[0] == BoardSize - 1) {
		return 0;
	}

	for (int i = 1; i < Snake.len; i++) {
		if (Snake.x[0] == Snake.x[i] && Snake.y[0] == Snake.y[i]) {
			return 0;
		}
	}
	return 1;
}

/*void GenerateApple() {
	srand(time(NULL));
	Apple.x = (rand() % (BoardSize - 2) + 1) * 2;
	Apple.y = rand() % (BoardSize - 2) + 1;
	gotoxy(Apple.x, Apple.y);
	printf("б▄");
}*/

int crashCnt = 0;
int GenerateApple() {
	srand((unsigned int)time(NULL));

	Apple.x = (rand() % (BoardSize - 2) + 1) * 2;
	Apple.y = rand() % (BoardSize - 2) + 1;

	int i;
	for (i = 0; i < Snake.len; i++) {
		if (Apple.x == Snake.x[i] && Apple.y == Snake.y[i]) {
			Apple.x = (rand() % (BoardSize - 2) + 1) * 2;
			Apple.y = rand() % (BoardSize - 2) + 1;
			i = 0;
			gotoxy(5, 20);
			printf("crash : %d", ++crashCnt);
		}
	}

	gotoxy(Apple.x, Apple.y);
	printf("б▄");
}

int main() {
	HideCursor();
	system("mode con:cols=40 lines=21");

	BoardSize = 20;
	DrawBoard(BoardSize);

	dir = RIGHT;

	Snake.x[0] = 10;
	Snake.y[0] = 10;
	Snake.len = 5;
	
	gotoxy(Snake.x[0], Snake.y[0]);
	printf("бс");

	GenerateApple();

	while (1) {
		if (SnakeBodyCheck() == 0) break;

		if (Apple.x == Snake.x[0] && Apple.y == Snake.y[0]) {
			Snake.len++;
			Snake.x[Snake.len] = Snake.x[Snake.len - 1];
			Snake.y[Snake.len] = Snake.y[Snake.len - 1];
			GenerateApple();
			gotoxy(0, 20);
			printf("%d", Snake.len - 5);
		}

		if (_kbhit()) {
			int key;

			do { key = _getch(); } while (key == 224);

			if ((dir == RIGHT && key != LEFT) || (dir == LEFT && key != RIGHT) || (dir == UP && key != DOWN) || (dir == DOWN && key != UP)) {
				switch (key) {
				case UP:    dir = UP; break;
				case DOWN:  dir = DOWN; break;
				case LEFT:  dir = LEFT; break;
				case RIGHT: dir = RIGHT; break;
				default: break;
				}
			}
		}

		MoveSnake(dir);
		Sleep(100);
	}

	return 0;
}