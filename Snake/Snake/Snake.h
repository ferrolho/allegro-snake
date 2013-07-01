#pragma once

#include "stdIncludes.h"

enum Direction {UP, DOWN, LEFT, RIGHT};

class Snake
{
public:
	Snake(void);
	
	void DrawSnake();
	void MoveSnake();
	void SetColor(ALLEGRO_COLOR NewColor);
	void AddSnakeCell(const vector<int> & cell);
	void IncreaseSnakeLength();
	bool EatedItself();
	void ResetSnakeDetails();
	bool EatedApple(int AppleX, int AppleY);
	bool IsInScreenBoundaries();
	void SetSnakeDirection(Direction new_direction);
	Direction GetSnakeDirection();
	int GetSnakeCellSize();
	const vector<vector<int> > & GetSnakeCells() const;

	~Snake(void);

private:
	int x, y;
	int size;
	vector<vector<int> > cells;
	vector<int> previous_head_coords;
	Direction direction;
	Direction previous_direction;
	ALLEGRO_COLOR color;
	int previous_tail_x, previous_tail_y;
};