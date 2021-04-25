#pragma once
#include <vector>
#include "Chromascreen.h"
#include <windows.h>
#include <ctime>
#include <stdio.h>
#include <random>

struct vec2d{
public:
	int x;
	int y;
	bool operator==(const vec2d &A){
		return ((A.x == this->x ) & (A.y == this->y));
	};
};

class Snake
{
private:
	Chromascreen Screen;
	int size_x;
	int size_y;
	int maxfood;
	int startlength;
	int speed=5;
	std::vector<vec2d> SnakeObj;
	std::vector<vec2d> Food;
	vec2d vel;
	vec2d tempvel = {};
	COLORREF Snake_color;
	float color_index;
	std::vector<COLORREF> ScreenBuffer;
public:
	Snake(int size_x=11, int size_y=4, int maxfood=1, int startlenght=1);

	int getsize_x() { return size_x; };
	int getsize_y() { return size_y; };
	int getmaxfood() { return maxfood; };
	int getstartlength() { return startlength; };

	bool setsize_x(int size_x);
	bool setsize_y(int size_y);
	bool setmaxfood(int maxfood);
	bool setstartlength(int startlength);

	void Death();

	void GetSnakeColor(float color);
	void DrawScreen();
	void CreateFood(int index);
	void GetInput();
	void MoveSnake();
	int start();
	~Snake();
};

