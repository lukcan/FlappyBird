#include <windows.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <random>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <conio.h>
#include <dos.h>
#include <string>

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

const int width = 100, height = 30, pipe_gap_x = 20, pipe_gap_y = 10;

struct Pipe {
	int x, y, gap;
};

void gotoxy(int w, int h);
void instructions();
void null_canvas(char canvas[height][width], int w, int h);
void print_canvas(char canvas[height][width], int w, int h);
int run_game();

int main()
{	
	int menu_x = 2, menu_y = 7;
	int score = -1 , score_best = 0;

	HWND hwnd = GetConsoleWindow();
	RECT rect = { 150, 150, 1100, 700 };
	MoveWindow(hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, TRUE);

	//intro
	do {
		system("cls");
		gotoxy(0,0); cout << "Best: " << score_best  << (score >= 0 ? "  Your score: " + to_string(score) : "");
		gotoxy(width / menu_x - 2, height / menu_y + 5); cout << " -------------------------- ";
		gotoxy(width / menu_x - 2, height / menu_y + 6); cout << " |      Flappy Bird       | ";
		gotoxy(width / menu_x - 2, height / menu_y + 7); cout << " --------------------------";
		gotoxy(width / menu_x, height / menu_y + 9); cout << "1. Start";
		gotoxy(width / menu_x, height / menu_y + 10); cout << "2. Instructions";
		gotoxy(width / menu_x, height / menu_y + 11); cout << "3. Quit";
		gotoxy(width / menu_x, height / menu_y + 13); cout << "Select option: ";
		char op = _getche();

		if (op == '1') {
			score = run_game();
			score_best = max(score, score_best);
		}			

		else if (op == '2') instructions();
		else if (op == '3') break;
		
	} while (1);

	return 0;
}

void gotoxy(int w, int h)
{
	CursorPosition.X = w;
	CursorPosition.Y = h;
	SetConsoleCursorPosition(console, CursorPosition);
}

void instructions() {
	int menu_x = 2, menu_y = 7;
	system("cls");
	gotoxy(width / menu_x, height / menu_y + 5); cout << "   Instructions";
	gotoxy(width / menu_x, height / menu_y + 6); cout << "----------------";
	gotoxy(width / menu_x - 7, height / menu_y + 7); cout << " Press spacebar to make bird fly\n";
	gotoxy(width / menu_x - 7, height / menu_y + 9); cout << "Press any key to go back to menu";
	_getch();
}

void null_canvas(char canvas[height][width], int w, int h) {
	for (int i = 0; i < h; i++)
	{
		for (int j = 0; j < w; j++)
		{
			canvas[i][j] = ' ';
		}
	}
}

void print_canvas(char canvas[height][width], int w, int h) {

	for (int i = 0; i < h; i++)
	{
		cout.write(canvas[i], w);
		cout << '\n';
	}
	cout << endl;
}

int run_game(){

	const int pipe_n = width / pipe_gap_x;
	Pipe pipes[pipe_n];
	char canvas[height][width];
	int bird_y = height / 2, bird_x = (pipe_n / 4) * pipe_gap_x + 1;

	char bird[2][6] = { '<','/','/','/','=','Q',
					'/','/','/',' ',' ',' ' };
	srand(time(0));
	int score = -1;
	
	// cmd "clear" 
	for (int i = 0; i < 1; i++)
	{
		cout << string(10000, '\n');
	}

	//pige gape
	for (int i = 0; i < pipe_n; i++)
	{
		Pipe new_p;
		new_p.x = i * pipe_gap_x;

		new_p.gap = (rand() % 2) + pipe_gap_y;
		int rnd_y = rand() % (height - new_p.gap - 4) + 2;
		new_p.y = rnd_y;


		pipes[i] = new_p;
	}

	pipes[1].y =  height / 2 - pipes[1].gap / 2;

	while (true)
	{
		bird_y++;
		
		// input handling and bird pos update 
		if (_kbhit()) {
			char ch = _getch();
			if (ch == 32) {
				if (bird_y > 3)
					bird_y -= 5;
			}
			if (ch == 27) {
				break;
			}
		}

		// pipes update 
		for (int i = 0; i < pipe_n; i++)
		{
			pipes[i].x--;

			if (pipes[i].x < 0) {
				pipes[i].gap = (rand() % 2) + pipe_gap_y;
				pipes[i].y = rand() % (height - pipes[i].gap -4 ) + 2;
				pipes[i].x = width - 1;
				score++;
			}
		}

		null_canvas(canvas, width, height);

		// pipe printing 
		for (int i = 0; i < pipe_n; i++)
		{
			for (int j = 0; j < height; j++)
			{
				if ((j <= pipes[i].y || j >= pipes[i].y + pipes[i].gap)) {
					canvas[j][pipes[i].x] = '±';
					if (pipes[i].x - 1 > 0 && pipes[i].x + 1 < width) {
						canvas[j][pipes[i].x + 1] = '|';
						canvas[j][pipes[i].x - 1] = '|';

					}

				}
			}
		}

		bool hit = false;
		// bird drawing 
		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 6; j++) {

				if (bird[i][j] != ' ') {

					if (canvas[i + bird_y - 1][j - 2 + bird_x] != ' ') {
						hit = true;
						canvas[i + bird_y - 1][j - 2 + bird_x] = 'X';
						continue;
					}
					canvas[i + bird_y - 1][j - 2 + bird_x] = bird[i][j];
				}
			}
		}

		//canvas printing 
		print_canvas(canvas, width, height);
		cout << "Score: " << score << endl;

		if (hit) {
			break;
		}

		Sleep(80);
		//system("cls");
	}
	return score;
}
