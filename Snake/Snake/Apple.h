#pragma once

#include "stdIncludes.h"

class Apple
{
public:
	Apple(void);
	
	void NewApple(const vector<vector<int> > & SnakeCells);
	void DrawApple(ALLEGRO_BITMAP * png);
	int GetAppleX();
	int GetAppleY();
	bool AppleCreatedOnSnake(int AppleX, int AppleY, const vector<vector<int> > & SnakeCells);

	~Apple(void);

private:
	int x, y;
	int size;
};