#include "Button.h"

Button::Button(int button_width_percentage, int button_height_percentage, ALLEGRO_COLOR color, int dx, int dy)
{
	int button_width = get_width(button_width_percentage);
	int button_height = get_height(button_height_percentage);
	int button_x_displacement = get_width(dx);
	int button_y_displacement = get_width(dy);

	button_x1 = ((CurrentScreenWidth - button_width) / 2) + button_x_displacement;
	button_x2 = (((CurrentScreenWidth - button_width) / 2) + button_width) + button_x_displacement;
	button_y1 = ((CurrentScreenHeight - button_height) / 2) + button_y_displacement;
	button_y2 = (((CurrentScreenHeight - button_height) / 2) + button_height) + button_y_displacement;

	button_color = color;

	play_over_button_sound = false;
	play_pressed_button_sound = false;

	StillOverButton = false;
	pressed_button = false;
	StillPressingButton = false;
}

void Button::DisplayButton()
{
	al_draw_filled_rounded_rectangle(button_x1, button_y1, button_x2, button_y2, 7, 7, button_color);
}

void Button::SetButtonColor(ALLEGRO_COLOR color)
{
	button_color = color;
}

bool Button::MouseOverButton(int mouse_x, int mouse_y)
{
	if ((button_x1 < mouse_x && mouse_x < button_x2) &&
		(button_y1 < mouse_y && mouse_y < button_y2))
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Button::GetButtonX1()
{
	return button_x1;
}

int Button::GetButtonY1()
{
	return button_y1;
}

int Button::GetButtonX2()
{
	return button_x2;
}

int Button::GetButtonY2()
{
	return button_y2;
}

int Button::GetButtonWidthCenter()
{
	return (GetButtonX1() + ((GetButtonX2() - GetButtonX1()) / 2));
}

int Button::GetButtonHeightCenter()
{
	return (GetButtonY1() + ((GetButtonY2() - GetButtonY1()) / 2));
}

Button::~Button(void)
{
}