#include "Snake.h"

Snake::Snake(int size_x, int size_y, int maxfood, int startlength){
	int error = 0;
	error += !Screen.Initialize();
	error += setsize_x(size_x);
	error += setsize_y(size_y);
	error += setmaxfood(maxfood);
	error += setstartlength(startlength);
	if (error)
		exit(error);
};

bool Snake::setsize_x(int size_x) {
	if (size_x <= ChromaSDK::Keyboard::MAX_COLUMN) {
		this->size_x = size_x;
		ScreenBuffer.resize(size_x * size_y);
		return 0;
	}
	return 1;
};
bool Snake::setsize_y(int size_y) {
	if (size_y <= ChromaSDK::Keyboard::MAX_ROW) {
		this->size_y = size_y;
		ScreenBuffer.resize(size_x * size_y);
		return 0;
	}
	return 1;
};
bool Snake::setmaxfood(int maxfood) {
	if (maxfood < size_x * size_y) {
		this->maxfood = maxfood;
		Food.resize(maxfood);
		return 0;
	}
	return 1;
};
bool Snake::setstartlength(int startlength) {
	if (startlength < size_x * size_y) {
		this->startlength = startlength;
		return 0;
	}
	return 1;
};

void Snake::GetInput() {
	if ((GetAsyncKeyState('W') | GetAsyncKeyState(VK_UP)) & 0x8000 && !vel.y) {
		tempvel.y = -1;
		tempvel.x = 0;
	}
	if ((GetAsyncKeyState('S') | GetAsyncKeyState(VK_DOWN)) & 0x8000 && !vel.y) {
		tempvel.y = 1;
		tempvel.x = 0;
	}
	if ((GetAsyncKeyState('D') | GetAsyncKeyState(VK_RIGHT)) & 0x8000 && !vel.x) {
		tempvel.y = 0;
		tempvel.x = 1;
	}
	if ((GetAsyncKeyState('A') | GetAsyncKeyState(VK_LEFT)) & 0x8000 && !vel.x) {
		tempvel.y = 0;
		tempvel.x = -1;
	}
}

void Snake::MoveSnake() {

	for (int i = SnakeObj.size()-1; i > 0; i--) {
		SnakeObj[i] = SnakeObj[i - 1];
	}
	//SnakeObj[1] = SnakeObj.end();

	SnakeObj[0].x += vel.x;
	SnakeObj[0].y += vel.y;
	if (SnakeObj[0].x >= size_x)
		SnakeObj[0].x = 0;
	if (SnakeObj[0].x < 0)
		SnakeObj[0].x = size_x-1;

	if (SnakeObj[0].y< 0)
		SnakeObj[0].y = size_y-1;
	if (SnakeObj[0].y >= size_y)
		SnakeObj[0].y = 0;

	//std::cout << SnakeObj[0].x << " " << SnakeObj[0].y << std::endl;

}

void Snake::CreateFood(int index) {
	int flag=1; 
	while (flag) {
		flag = 0;
		Food[index] = { (std::rand() * (size_x - 1)) / RAND_MAX, (std::rand() * (size_y - 1)) / RAND_MAX };
		for (int i = 0; i < SnakeObj.size(); i++) {
			if (Food[index] == SnakeObj[i]) 
				flag = 1;
		}
		for (int i = 0; i < maxfood; i++) {
			if (Food[i] == Food[index] && i != index)
				flag = 1;
		}
	}

}

void Snake::Death() {
	DrawScreen();
	for (int i=0; i < SnakeObj.size(); i++) {
		//std::cout << SnakeObj.at(i).x + (size_x * SnakeObj.at(i).y) << std::endl;
		ScreenBuffer[SnakeObj.at(i).x + (size_x * SnakeObj.at(i).y)] = RGB(255, 0, 0);
		Screen.setScreen(ScreenBuffer, size_x, size_y, 2, 1);
		Sleep(1000 / speed);
	}
	for (int i = SnakeObj.size() - 1; i >= 0; i--) {
		//std::cout << SnakeObj.at(i).x + (size_x * SnakeObj.at(i).y) << std::endl;
		ScreenBuffer[SnakeObj.at(i).x + (size_x * SnakeObj.at(i).y)] = RGB(0, 0, 0);
		Screen.setScreen(ScreenBuffer, size_x, size_y, 2, 1);
		Sleep(1000 / speed);
	}
}

void Snake::GetSnakeColor(float color) {
	while (color >= 3) {
		color -= 3;
	}
	int index = (int)color;
	float frac = color - index;
	Snake_color = 0;
	((byte*)&Snake_color)[index] = (byte)(255 * frac);
	if (index == 0) {
		((byte*)&Snake_color)[2] = 255 - ((byte*)&Snake_color)[index];
	}
	else {
		((byte*)&Snake_color)[index - 1] = 255 - ((byte*)&Snake_color)[index];
	}
	//printf("%d %d %d\n", ((byte*)&Snake_color)[0], ((byte*)&Snake_color)[1], ((byte*)&Snake_color)[2]);
}

void Snake::DrawScreen() {
	std::fill(ScreenBuffer.begin(), ScreenBuffer.end(), 0);
	for (int i = 0; i < SnakeObj.size(); i++) {
		//std::cout << SnakeObj.at(i).x + (size_x * SnakeObj.at(i).y) << std::endl;
		GetSnakeColor(color_index-i*.05);
		ScreenBuffer[SnakeObj.at(i).x + (size_x * SnakeObj.at(i).y)] = Snake_color;
	}
	for (int i = 0; i < maxfood; i++) {
		ScreenBuffer[Food[i].x + (size_x * Food[i].y)] = RGB(255, 50, 50);
	}
	Screen.setScreen(ScreenBuffer, size_x, size_y, 2, 1);
}

int Snake::start() {
	time_t start = clock();
	int run = 1;
	color_index = 0;
	vec2d origin = { size_x / 2, size_y / 2 };
	SnakeObj.resize(startlength);
	for (int i = 0; i < startlength; i++) {
		SnakeObj[i]=origin;
	}
	vel = {};
	for (int i = 0; i < maxfood; i++)
		CreateFood(i);
	while (run) {
		GetInput();
		if ((float)(clock()-start) / (float)CLOCKS_PER_SEC > 1.0/speed) {
			start = clock();
			vel = tempvel;
			MoveSnake();
			for (int i = 1; i < SnakeObj.size(); i++) {
				if (SnakeObj[i]==SnakeObj[0]) {
					Death();
					run = 0;
					break;
				}
			}
			for (int i = 0; i < maxfood; i++) {
				if (Food[i] == SnakeObj[0]) {
					SnakeObj.push_back(SnakeObj[0]);
					if (SnakeObj.size() >= 44) {
						run = 0;
						break;
					}
					//std::cout << SnakeObj.size() << std::endl;
					CreateFood(i);
				}
			}
			//DebugPrint();
		}
		if(run)
			DrawScreen();
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			return -1;
		}
		color_index +=.001;
		Sleep(10);

	}
	return SnakeObj.size();
};

Snake::~Snake() {

};