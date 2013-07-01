#include "Snake.h"

Snake::Snake(void)
{
	/* coords */
	x = 20 * 3;
	y = 20 * 3;

	/* creating first cell (head) */
	vector<int> cell (2);
	cell[0] = x;
	cell[1] = y;
	cells.push_back(cell);
	
	/* saving head coords */
	previous_head_coords.clear();
	previous_head_coords.push_back(cells[0][0]);
	previous_head_coords.push_back(cells[0][1]);

	/* block size */
	size = 20;

	/* direction */
	previous_direction = direction = RIGHT;

	/* color */
	color = Green;
}

void Snake::DrawSnake()
{
	for (unsigned int i = 0; i < cells.size(); i++)
	{
		al_draw_rounded_rectangle(cells[i][0] - (size / 2), cells[i][1] - (size / 2), cells[i][0] + (size / 2), cells[i][1] + (size / 2), 4, 4, color, 2.0);
	}
}

void Snake::MoveSnake()
{
	if (direction == UP)
	{
		y -= size;
		if (y < 0)
		{
			y += size;
		}
	}
	if (direction == DOWN)
	{
		y += size;
		if (y > CurrentScreenHeight)
		{
			y -= size;
		}
	}
	if (direction == LEFT)
	{
		x -= size;
		if (x < 0)
		{
			x += size;
		}
	}
	if (direction == RIGHT)
	{
		x += size;
		if (x > CurrentScreenWidth)
		{
			x -= size;
		}
	}
	
	/* saving head position */
	previous_head_coords.clear();
	previous_head_coords.push_back(cells[0][0]);
	previous_head_coords.push_back(cells[0][1]);
	
	/* saving tail position */
	previous_tail_x = cells[cells.size() - 1][0];
	previous_tail_y = cells[cells.size() - 1][1];
	
	/* updating cells vector */
	vector<int> head (2);
	head[0] = x;
	head[1] = y;
	cells.pop_back();
	cells.insert(cells.begin(), head);
}

void Snake::SetColor(ALLEGRO_COLOR NewColor)
{
	color = NewColor;
}

void Snake::AddSnakeCell(const vector<int> & cell)
{
	cells.push_back(cell);
}

void Snake::IncreaseSnakeLength()
{
	vector<int> temp (2);
	temp[0] = previous_tail_x;
	temp[1] = previous_tail_y;
	AddSnakeCell(temp);
}

bool Snake::EatedItself()
{
	vector<int> pivot;
	for (unsigned int i = 0; i < cells.size(); i++)
	{
		pivot = cells[i];
		for (unsigned int j = i + 1; j < cells.size(); j++)
		{
			if (pivot == cells[j])
			{
				return true;
			}
		}
	}
	return false;
}

void Snake::ResetSnakeDetails()
{
	/* coords */
	x = 20 * 3;
	y = 20 * 3;

	/* creating first cell (head) */
	vector<int> cell (2);
	cell[0] = x;
	cell[1] = y;
	cells.clear();
	cells.push_back(cell);
	
	/* saving head coords */
	previous_head_coords.clear();
	previous_head_coords.push_back(cells[0][0]);
	previous_head_coords.push_back(cells[0][1]);

	/* direction */
	previous_direction = direction = RIGHT;

	/* color */
	color = Green;
}

bool Snake::EatedApple(int AppleX, int AppleY)
{
	if ((x == AppleX) && (y == AppleY))
	{
		return true;
	}
	else
	{
		return false;
	}	
}

bool Snake::IsInScreenBoundaries()
{
	if ((0 < x && (x + (size / 2)) <= (CurrentScreenWidth - (size / 2))) && (0 < y && (y + (size / 2)) <= (CurrentScreenHeight - (size / 2))))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Snake::SetSnakeDirection(Direction new_direction)
{
	if (cells.size() == 1)
	{		
		previous_direction = direction;
		direction = new_direction;
	}
	else if (!((new_direction == UP && direction == DOWN) || (new_direction == DOWN && direction == UP) ||
		       (new_direction == LEFT && direction == RIGHT) || (new_direction == RIGHT && direction == LEFT)))
	{
		previous_direction = direction;
		direction = new_direction;
	}
}

Direction Snake::GetSnakeDirection()
{
	return direction;
}

int Snake::GetSnakeCellSize()
{
	return size;
}

const vector<vector<int> > & Snake::GetSnakeCells() const
{
	return cells;
}

Snake::~Snake(void)
{
}