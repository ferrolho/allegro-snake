#include "Apple.h"

Apple::Apple(void)
{
	size = 15;
}

void Apple::NewApple(const vector<vector<int> > & SnakeCells)
{
	do
	{
		x = 20 * random_number(1, (CurrentScreenWidth / 20) - 1);
		y = 20 * random_number(1, (CurrentScreenHeight / 20) - 1);
	}
	while (AppleCreatedOnSnake(x, y, SnakeCells));
}

void Apple::DrawApple(ALLEGRO_BITMAP * png)
{
	al_draw_bitmap(png, x - 6, y - 6, NULL);
}

int Apple::GetAppleX()
{
	return x;
}

int Apple::GetAppleY()
{
	return y;
}

bool Apple::AppleCreatedOnSnake(int AppleX, int AppleY, const vector<vector<int> > & SnakeCells)
{
	for (unsigned int i = 0; i < SnakeCells.size(); i++)
	{
		if ((AppleX == SnakeCells[i][0]) && (AppleY == SnakeCells[i][1]))
		{
			return true;
		}
	}
	return false;
}

Apple::~Apple(void)
{
}