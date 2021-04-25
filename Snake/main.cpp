#include "Chromascreen.h"
#include "Snake.h"
#include <ctime>


#if 0

int main() {
	char buff[128];
	Snake Snake(11, 4, 2);
	int Score = Snake.start();
	if (Score == -1) {
		return 0;
	}
	if (Score >= 44)
		MessageBoxA(0, "You beat the Game!", "Win!", MB_OK);

	sprintf_s(buff, "You've Scored %d points", Score);
	MessageBoxA(0, buff, "Score", MB_OK);
}
#else
int APIENTRY WinMain(HINSTANCE Inst, HINSTANCE Prev, LPSTR cmdline, int show) {
	char buff[128];
	int Score = 0;
	Snake Snake(11, 4, 2);
	while (1) {
		Score = Snake.start();
		if (Score == -1)
			return 0;
		if (Score >= 44)
			MessageBoxA(0, "You beat the Game!", "Win!", MB_OK);

		sprintf_s(buff, "You've Scored %d points", Score);
		MessageBoxA(0, buff, "Score", MB_OK);
	}
}
#endif