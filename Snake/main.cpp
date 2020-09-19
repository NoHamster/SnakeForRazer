#include "Chromascreen.h"
#include <ctime>
#define Size_X ChromaSDK::Keyboard::MAX_ROW
#define Size_Y ChromaSDK::Keyboard::MAX_COLUMN
#define clamp_x 12
#define clamp_y 4
#define offset_x 2
#define offset_y 1

char counter = 0;
Chromascreen m;
COLORREF screen[Size_X][Size_Y];
char snake[48][2];
char Score;
unsigned char Velocity[2];;
char food[2];
char pos[2];

int death() {
	char Scoretxt[16] = "Your Score: ";
	Scoretxt[12] = (char)(48 + (Score / 10) % 10);
	Scoretxt[13] = (char)(48 + Score % 10);
	Scoretxt[14] = 0;
	for (int i = 0; i < Score; i++) {
		screen[snake[i][0]][snake[i][1]] = RED;
		Sleep(2000/Score);
		m.setKeyboard(screen, Size_X, Size_Y);
	}
	for (int i = Score-1; i >= 0; i--) {
		screen[snake[i][0]][snake[i][1]] = BLACK;
		Sleep(2000 / Score);
		m.setKeyboard(screen, Size_X, Size_Y);
	}
	return MessageBoxA(NULL, Scoretxt, "You Died!", MB_RETRYCANCEL | MB_SYSTEMMODAL);

}

void clearscreen(COLORREF arr[][Size_Y]) {
	for (int x = 0; x < Size_X; x++) {
		for (int y = 0; y < Size_Y; y++)
			arr[x][y] = BLACK;
	}
}

void set(char* A, char* B) {
	A[0] = B[0];
	A[1] = B[1];
}


int CALLBACK WinMain(HINSTANCE Inst, HINSTANCE Prev, LPSTR cmdline, int show) {
	m.Initialize();
	while (true) {
		bool retry = 0;
		counter = 0;
		memset(snake, 0, 96);
		Score = 2;

		clearscreen(screen);

		Velocity[0] = 0;
		Velocity[1] = 1;
		pos[0] = offset_y;
		pos[1] = offset_x;
		food[0] = offset_y + (rand() % (clamp_y - 1));
		food[1] = offset_x + (rand() % (clamp_x - 1));
		snake[1][0] = offset_y - 1;
		snake[1][1] = offset_x;
		while (true) {
			m.setKeyboard(screen, Size_X, Size_Y);
			if (Velocity[0] == 0 && GetAsyncKeyState('W') & 0x8000) {
				Velocity[0] = -1;
				Velocity[1] = 0;
			}
			else if (Velocity[0] == 0 && GetAsyncKeyState('S') & 0x8000) {
				Velocity[0] = 1;
				Velocity[1] = 0;
			}
			else if (Velocity[1] == 0 && GetAsyncKeyState('D') & 0x8000) {
				Velocity[1] = 1;
				Velocity[0] = 0;
			}
			else if (Velocity[1] == 0 && GetAsyncKeyState('A') & 0x8000) {
				Velocity[1] = -1;
				Velocity[0] = 0;
			}
			if (counter >= 4) {
				counter = 0;
				pos[0] += Velocity[0];
				pos[1] += Velocity[1];

				if (pos[1] > clamp_x)
					pos[1] = offset_x;
				if (pos[0] > clamp_y)
					pos[0] = offset_y;
				if (pos[1] < offset_x)
					pos[1] = clamp_x;
				if (pos[0] < offset_y)
					pos[0] = clamp_y;

				if (pos[0] == food[0] && pos[1] == food[1]) {
					snake[Score][0] = snake[Score - 1][0];
					snake[Score][1] = snake[Score - 1][1];
					Score++;
					food[1] = offset_x + (rand() % (clamp_x - 1));
					food[0] = offset_y + (rand() % (clamp_y - 1));
				}

				for (int i = 2; i < Score; i++) {
					if (pos[0] == snake[i][0] && pos[1] == snake[i][1]) {
						if (death() == 4)
							retry = 1;
						else
							ExitProcess(0);
						break;
					}
				}
				if (retry)
					break;
				clearscreen(screen);
				set(snake[0], pos);
				screen[food[0]][food[1]] = RED;
				screen[snake[0][0]][snake[0][1]] = GREEN;
				for (int i = Score - 1; i > 0; i--) {
					screen[snake[i][0]][snake[i][1]] = GREEN;
					set(snake[i], snake[i - 1]);

				}

			}
			Sleep(50);
			counter++;
		}
	}
}
