#ifndef BUTTON_H
#define BUTTON_H

#include "stdIncludes.h"

class Button
{
public:
	Button(int button_width_percentage, int button_height_percentage, ALLEGRO_COLOR color, int dx = 0, int dy = 0);

	void DisplayButton();
	void SetButtonColor(ALLEGRO_COLOR color);

	bool MouseOverButton(int mouse_x, int mouse_y);

	int GetButtonX1();
	int GetButtonY1();
	int GetButtonX2();
	int GetButtonY2();

	int GetButtonWidthCenter();
	int GetButtonHeightCenter();

	~Button(void);

private:
	int button_x1, button_y1, button_x2, button_y2;
	ALLEGRO_COLOR button_color;

public:
	bool play_over_button_sound;
	bool play_pressed_button_sound;

	bool StillOverButton;
	bool pressed_button;
	bool StillPressingButton;
};

#endif