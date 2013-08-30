#include "Game.h"

enum _GameState {MainMenu, PlayGame, PauseGame, GameOver};
_GameState gameState;

/* generating random numbers */
int random_number (int min, int max) {
	max++;

	return ((rand() % (max-min)) + min);
}

/* returns pixels corresponding to screen width percentage given */
int get_width (int percentage) {
	if (!(-100 < percentage && percentage < 100))
		return -1;
	else
		return ((CurrentScreenWidth * percentage) / 100);
}

/* returns pixels corresponding to screen height percentage given */
int get_height (int percentage) {
	if (!(-100 < percentage && percentage < 100))
		return -1;
	else
		return ((CurrentScreenHeight * percentage) / 100);
}

/* starting game UI */
void start_game (bool Fullscreen) {
	/* creating display */
	ALLEGRO_DISPLAY * display;
	if (Fullscreen) {
		/* creating disp_data struct to store supported resolutions */
		ALLEGRO_DISPLAY_MODE disp_data;

		/* making it fullscreen */
		al_set_new_display_flags(ALLEGRO_FULLSCREEN);

		/* storing info */
		al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
		CurrentScreenWidth = disp_data.width;
		CurrentScreenHeight = disp_data.height;

		/* creating display with different resolutions for different screens */
		display = al_create_display(CurrentScreenWidth, CurrentScreenHeight);
	}
	else {
		al_set_new_display_flags(ALLEGRO_WINDOWED);

		display = al_create_display(CurrentScreenWidth, CurrentScreenHeight);
	}
	if (!display) {
		al_show_native_message_box(display, "Error", "Display Settings", "Couldn't create a display.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}
	/* setting new window title */
	al_set_window_title(display, "Snake");

	// -----------------------

	/* creating fonts */
	ALLEGRO_FONT * font = al_load_font(MainFont, 36, ALLEGRO_ALIGN_CENTER);
	ALLEGRO_FONT * credits_font = al_load_font(CreditsFont, 20, NULL);
	if (!font) {
		al_show_native_message_box(display, "Error", "Could not load font file.", "Have you included the resources in the same directory of the program?", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}

	/* loading audio samples */
	ALLEGRO_SAMPLE * over_button_sound = al_load_sample(OverButton);
	ALLEGRO_SAMPLE * pressed_button_sound = al_load_sample(PressedButton);
	ALLEGRO_SAMPLE * eating_apple_sound = al_load_sample(EatApple);
	ALLEGRO_SAMPLE * game_over_sound = al_load_sample(GameOverSound);
	if (!over_button_sound || !pressed_button_sound) {
		al_show_native_message_box(display, "Error", "Could not load one or more sound files.", "Your resources folder must be corrupt, please download it again.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}
	al_reserve_samples(2);

	/* creating timer */
	ALLEGRO_TIMER * timer = al_create_timer(1.0 / FPS);
	ALLEGRO_TIMER *frametimer = al_create_timer(1.0 / frameFPS);

	/* creating event queue */
	ALLEGRO_EVENT_QUEUE * event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_timer_event_source(frametimer));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	ALLEGRO_KEYBOARD_STATE keystate;

	/* loading BITMAPS */
	ALLEGRO_BITMAP * SmallWallpaperBitmap = al_load_bitmap(SmallWallpaper);
	ALLEGRO_BITMAP * BigWallpaperBitmap = al_load_bitmap(BigWallpaper);
	ALLEGRO_BITMAP * mouse = al_load_bitmap(MouseCursor);
	ALLEGRO_BITMAP * applepng = al_load_bitmap(Apple_png);
	if (!mouse)
	{
		al_show_native_message_box(display, "Error", "Could not load one or more resource file.", "Your resources folder must be corrupt, please download it again.", NULL, ALLEGRO_MESSAGEBOX_ERROR);
		exit(-1);
	}
	al_hide_mouse_cursor(display);

	/* ---- VARIABLES ---- */
	bool done = false, change_resolution = false, draw = true;
	int mouse_x = CurrentScreenWidth, mouse_y = CurrentScreenHeight;
	/* mouse */
	bool left_mouse_button_down = false;
	bool left_mouse_button_up = false;
	/* MAIN MENU */
	int button_displacement;
	if (!Fullscreen)
	{
		button_displacement = 20;
	}
	else
	{
		button_displacement = 15;
	}
	Button play_button(50, 20, Blue, 0, -button_displacement);
	Button options_button(40, 15, Blue);
	Button exit_button(40, 15, Blue, 0, button_displacement);
	/* PLAY */
	string score;
	Direction new_direction;
	int speed_up, speed_up_anim_frame = 0;
	bool speed_up_anim = false;
	Snake snake;
	Apple apple;

	/* starting timers */
	al_start_timer(timer);
	al_start_timer(frametimer);

	while (!done)
	{
		/* actually defining our events */
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);
		al_get_keyboard_state(&keystate);

		switch (gameState)
		{
		case MainMenu:
			{
				/* WINDOW */
				if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				{
					done = true;
				}
				/* MOUSE */
				if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
				{
					mouse_x = events.mouse.x;
					mouse_y = events.mouse.y;

					draw = true;
				}
				if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
				{
					/* left button */
					if (events.mouse.button & 1)
					{
						left_mouse_button_down = true;
						draw = true;
					}
				}
				if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
				{
					/* left button */
					if (events.mouse.button & 1)
					{
						left_mouse_button_down = false;
						left_mouse_button_up = true;
						draw = true;
					}
				}
				/* button conditions */
				if (play_button.MouseOverButton(mouse_x, mouse_y) && !play_button.StillOverButton)
				{
					play_button.play_over_button_sound = true;
				}
				if (options_button.MouseOverButton(mouse_x, mouse_y) && !options_button.StillOverButton)
				{
					options_button.play_over_button_sound = true;
				}
				if (exit_button.MouseOverButton(mouse_x, mouse_y) && !exit_button.StillOverButton)
				{
					exit_button.play_over_button_sound = true;
				}
				/* KEYBOARD */
				if (events.type == ALLEGRO_EVENT_KEY_UP)
				{
					switch (events.keyboard.keycode)
					{
					case ALLEGRO_KEY_ESCAPE:
						{
							done = true;
							break;
						}
					case ALLEGRO_KEY_SPACE:
					case ALLEGRO_KEY_ENTER:
						{
							/* STARTING GAME NOW */
							new_direction = RIGHT;
							speed_up = 0;
							snake.ResetSnakeDetails();
							apple.NewApple(snake.GetSnakeCells());
							gameState = PlayGame;
						}
					}
					break;
				}

				/* ------------ NOW DRAWING ------------ */
				if (draw)
				{
					/* drawing wallpaper */
					if (Fullscreen)
					{
						al_draw_scaled_bitmap(BigWallpaperBitmap, 0, 0, al_get_bitmap_width(BigWallpaperBitmap), al_get_bitmap_height(BigWallpaperBitmap), 0, 0, CurrentScreenWidth, CurrentScreenHeight, NULL);
					}
					else
					{
						al_draw_bitmap(SmallWallpaperBitmap, 0, 0, 0);
					}

					/* -- play button -- */
					if (play_button.MouseOverButton(mouse_x, mouse_y))
					{
						play_button.SetButtonColor(LightBlue);
						/* button pressed */
						if (left_mouse_button_down)
						{
							play_button.pressed_button = true;
							play_button.SetButtonColor(DarkBlue);
						}
						/* button released */
						else if (left_mouse_button_up)
						{
							play_button.pressed_button = false;
							play_button.StillPressingButton = false;
							play_button.SetButtonColor(Blue);

							/* STARTING GAME NOW */
							new_direction = RIGHT;
							speed_up = 0;
							snake.ResetSnakeDetails();
							apple.NewApple(snake.GetSnakeCells());
							gameState = PlayGame;
						}
						play_button.DisplayButton();
					}
					else
					{
						play_button.pressed_button = false;
						play_button.StillPressingButton = false;
						play_button.StillOverButton = false;
						play_button.SetButtonColor(Blue);
						play_button.DisplayButton();
					}
					/* -- options button -- */
					if (options_button.MouseOverButton(mouse_x, mouse_y))
					{
						options_button.SetButtonColor(LightBlue);
						/* button pressed */
						if (left_mouse_button_down)
						{
							options_button.pressed_button = true;
							options_button.SetButtonColor(DarkBlue);
						}
						/* button released */
						else if (left_mouse_button_up)
						{
							options_button.pressed_button = false;
							options_button.StillPressingButton = false;
							options_button.SetButtonColor(Blue);
							switch (Fullscreen)
							{
							case 0:
								{
									Fullscreen = 1;
									done = true;
									change_resolution = true;
									break;
								}
							case 1:
								{
									Fullscreen = 0;
									done = true;
									change_resolution = true;
									break;
								}
							}
						}
						options_button.DisplayButton();
					}
					else
					{
						options_button.pressed_button = false;
						options_button.StillPressingButton = false;
						options_button.StillOverButton = false;
						options_button.SetButtonColor(Blue);
						options_button.DisplayButton();
					}
					/* -- exit button -- */
					if (exit_button.MouseOverButton(mouse_x, mouse_y))
					{
						exit_button.SetButtonColor(LightBlue);
						/* button pressed */
						if (left_mouse_button_down)
						{
							exit_button.pressed_button = true;
							exit_button.SetButtonColor(DarkBlue);
						}
						/* button released */
						else if (left_mouse_button_up)
						{
							exit_button.pressed_button = false;
							exit_button.StillPressingButton = false;
							exit_button.SetButtonColor(Blue);
							done = true;
						}
						exit_button.DisplayButton();
					}
					else
					{
						exit_button.pressed_button = false;
						exit_button.StillPressingButton = false;
						exit_button.StillOverButton = false;
						exit_button.SetButtonColor(Blue);
						exit_button.DisplayButton();
					}

					/* -- sound -- */
					/* mouse over button */
					if (play_button.MouseOverButton(mouse_x, mouse_y) && !play_button.StillOverButton)
					{
						play_button.StillOverButton = true;
						al_play_sample(over_button_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
					if (options_button.MouseOverButton(mouse_x, mouse_y) && !options_button.StillOverButton)
					{
						options_button.StillOverButton = true;
						al_play_sample(over_button_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
					if (exit_button.MouseOverButton(mouse_x, mouse_y) && !exit_button.StillOverButton)
					{
						exit_button.StillOverButton = true;
						al_play_sample(over_button_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
					/* button pressed */
					if (play_button.pressed_button && !play_button.StillPressingButton)
					{
						play_button.StillPressingButton = true;
						al_play_sample(pressed_button_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
					else if (options_button.pressed_button && !options_button.StillPressingButton)
					{
						options_button.StillPressingButton = true;
						al_play_sample(pressed_button_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
					else if (exit_button.pressed_button && !exit_button.StillPressingButton)
					{
						exit_button.StillPressingButton = true;
						al_play_sample(pressed_button_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}

					/* -- text -- */
					string fullscreenmode_string;
					if (Fullscreen)
					{
						fullscreenmode_string = "Fullscreen: On";
					}
					else
					{
						fullscreenmode_string = "Fullscreen: Off";
					}
					al_draw_text(font, White, CurrentScreenWidth / 2, play_button.GetButtonHeightCenter() - 23, ALLEGRO_ALIGN_CENTER, "New Game");
					al_draw_text(font, White, CurrentScreenWidth / 2, options_button.GetButtonHeightCenter() - 23, ALLEGRO_ALIGN_CENTER, fullscreenmode_string.c_str());
					al_draw_text(font, White, CurrentScreenWidth / 2, exit_button.GetButtonHeightCenter() - 23, ALLEGRO_ALIGN_CENTER, "Exit");
					al_draw_text(credits_font, White, 3, CurrentScreenHeight - 20, NULL, "FEUP 2013 - Henrique Ferrolho");

					/* -- mouse cursor -- */
					al_draw_bitmap(mouse, mouse_x, mouse_y, NULL);

					al_flip_display();
					al_clear_to_color(al_map_rgb(0, 0, 0));
					left_mouse_button_up = false;
					draw = false;
				}
				break;
			}
		case PlayGame:
			{
				if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				{
					done = true;
				}
				/* KEYBOARD */
				if (events.type == ALLEGRO_EVENT_KEY_UP)
				{
					switch (events.keyboard.keycode)
					{
					case ALLEGRO_KEY_ESCAPE:
						{
							/* going back to MAIN MENU */
							draw = true;
							gameState = MainMenu;
							break;
						}
					case ALLEGRO_KEY_ENTER:
					case ALLEGRO_KEY_SPACE:
						{
							/* pausing game */
							draw = true;
							gameState = PauseGame;
							break;
						}

					}
					break;
				}
				if (events.type == ALLEGRO_EVENT_TIMER)
				{
					if (events.timer.source == timer)
					{
						draw = true;

						/* navigation keys */
						if (al_key_down(&keystate, ALLEGRO_KEY_DOWN) || al_key_down(&keystate, ALLEGRO_KEY_S))
						{
							new_direction = DOWN;
						}
						else if (al_key_down(&keystate, ALLEGRO_KEY_UP) || al_key_down(&keystate, ALLEGRO_KEY_W))
						{
							new_direction = UP;
						}
						else if (al_key_down(&keystate, ALLEGRO_KEY_RIGHT) || al_key_down(&keystate, ALLEGRO_KEY_D))
						{
							new_direction = RIGHT;
						}
						else if (al_key_down(&keystate, ALLEGRO_KEY_LEFT) || al_key_down(&keystate, ALLEGRO_KEY_A))
						{
							new_direction = LEFT;
						}

						/* checking boundaries */
						if (!snake.IsInScreenBoundaries() || snake.EatedItself())
						{
							draw = true;
							al_play_sample(game_over_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
							gameState = GameOver;
							break;
						}
					}
					else if (events.timer.source == frametimer)
					{
						/* moving snake */
						snake.SetSnakeDirection(new_direction);
						snake.MoveSnake();
						if (snake.EatedApple(apple.GetAppleX(), apple.GetAppleY()))
						{
							al_play_sample(eating_apple_sound, 0.5, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
							apple.NewApple(snake.GetSnakeCells());
							snake.IncreaseSnakeLength();
							draw = true;
						}
					}					
				}

				/* ------------ NOW DRAWING ------------ */
				if (draw)
				{	
					/* game frame */
					if (Fullscreen)
					{
						al_draw_rectangle(0, 0, CurrentScreenWidth, CurrentScreenHeight, DarkRed, 20);
						//al_draw_rectangle(0, 0, 1360, 760, DarkRed, 20);
					}
					else
					{
						al_draw_rectangle(0, 0, CurrentScreenWidth, CurrentScreenHeight, DarkRed, 20);
					}
					apple.DrawApple(applepng);
					snake.DrawSnake();

					/* increasing speed */
					if (snake.GetSnakeCells().size() < 5)
					{
						al_set_timer_speed(frametimer, 1.0 / frameFPS);
					}
					else if (snake.GetSnakeCells().size() < 10)
					{
						if (speed_up == 0)
						{
							speed_up++;
							speed_up_anim = true;
						}
						al_set_timer_speed(frametimer, 1.0 / 14);
					}
					else if (snake.GetSnakeCells().size() < 20)
					{
						if (speed_up == 1)
						{
							snake.SetColor(LightBlue);
							speed_up++;
							speed_up_anim = true;
						}
						al_set_timer_speed(frametimer, 1.0 / 16);
					}
					else if (snake.GetSnakeCells().size() < 30)
					{
						if (speed_up == 2)
						{
							speed_up++;
							speed_up_anim = true;
						}
						al_set_timer_speed(frametimer, 1.0 / 18);
					}
					else if (snake.GetSnakeCells().size() < 40)
					{
						if (speed_up == 3)
						{
							snake.SetColor(DarkRed);
							speed_up++;
							speed_up_anim = true;
						}
						al_set_timer_speed(frametimer, 1.0 / 20);
					}

					/* speed up animation */
					if (speed_up_anim)
					{
						if (speed_up_anim_frame < 10)
						{
							al_draw_text(font, White, CurrentScreenWidth / 2, (CurrentScreenHeight / 2) - 30, ALLEGRO_ALIGN_CENTER, "SPEED UP!");
							speed_up_anim_frame++;
						}
						else if (speed_up_anim_frame < 20)
						{
							al_draw_text(font, Yellow, CurrentScreenWidth / 2, (CurrentScreenHeight / 2) - 30, ALLEGRO_ALIGN_CENTER, "SPEED UP!");
							speed_up_anim_frame++;
						}
						else if (speed_up_anim_frame < 30)
						{
							al_draw_text(font, White, CurrentScreenWidth / 2, (CurrentScreenHeight / 2) - 30, ALLEGRO_ALIGN_CENTER, "SPEED UP!");
							speed_up_anim_frame++;
						}
						else if (speed_up_anim_frame < 40)
						{
							al_draw_text(font, Yellow, CurrentScreenWidth / 2, (CurrentScreenHeight / 2) - 30, ALLEGRO_ALIGN_CENTER, "SPEED UP!");
							speed_up_anim_frame++;
						}
						else if (speed_up_anim_frame < 50)
						{
							al_draw_text(font, White, CurrentScreenWidth / 2, (CurrentScreenHeight / 2) - 30, ALLEGRO_ALIGN_CENTER, "SPEED UP!");
							speed_up_anim_frame++;
						}
						else if (speed_up_anim_frame < 60)
						{
							speed_up_anim = false;
							speed_up_anim_frame = 0;
						}
					}

					/* printing score */
					stringstream ss;
					ss << "Score: " << snake.GetSnakeCells().size();
					score = ss.str();
					al_draw_text(credits_font, Yellow, 60, 15, ALLEGRO_ALIGN_CENTER, score.c_str());

					al_flip_display();
					al_clear_to_color(al_map_rgb(0, 0, 0));
					draw = false;
				}
				break;
			}
		case PauseGame:
			{
				if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				{
					done = true;
				}
				if (events.type == ALLEGRO_EVENT_KEY_UP)
				{
					switch (events.keyboard.keycode)
					{
					case ALLEGRO_KEY_SPACE:
					case ALLEGRO_KEY_ENTER:
						{
							/* resume game */
							draw = true;
							gameState = PlayGame;
							break;
						}
					}
					break;
				}

				/* ------------ NOW DRAWING ------------ */
				if (draw)
				{
					/* game frame */
					if (Fullscreen)
					{
						al_draw_rectangle(0, 0, CurrentScreenWidth, CurrentScreenHeight, DarkRed, 20);
						//al_draw_rectangle(0, 0, 1360, 760, DarkRed, 20);
					}
					else
					{
						al_draw_rectangle(0, 0, CurrentScreenWidth, CurrentScreenHeight, DarkRed, 20);
					}
					apple.DrawApple(applepng);
					snake.DrawSnake();
					
					/* printing score */
					stringstream ss;
					ss << "Score: " << snake.GetSnakeCells().size();
					score = ss.str();
					al_draw_text(credits_font, Yellow, 60, 15, ALLEGRO_ALIGN_CENTER, score.c_str());

					al_draw_text(font, Yellow, CurrentScreenWidth / 2, (CurrentScreenHeight / 2) - 30, ALLEGRO_ALIGN_CENTER, "GAME PAUSED");

					al_flip_display();
					draw = false;
				}
				break;
			}
		case GameOver:
			{
				if (events.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
				{
					done = true;
				}
				/* MOUSE */
				if (events.type == ALLEGRO_EVENT_MOUSE_AXES)
				{
					mouse_x = events.mouse.x;
					mouse_y = events.mouse.y;

					draw = true;
				}
				if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
				{
					/* left button */
					if (events.mouse.button & 1)
					{
						left_mouse_button_down = true;
						draw = true;
					}
				}
				if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
				{
					/* left button */
					if (events.mouse.button & 1)
					{
						left_mouse_button_down = false;
						left_mouse_button_up = true;
						draw = true;
					}
				}
				
				/* going to MAIN MENU */
				if (events.type == ALLEGRO_EVENT_KEY_UP)
				{
					switch (events.keyboard.keycode)
					{
					case ALLEGRO_KEY_SPACE:
					case ALLEGRO_KEY_ENTER:
						{
							draw = true;
							gameState = MainMenu;
							break;
						}
					}
					break;
				}
				if (left_mouse_button_up)
				{
					left_mouse_button_up = false;
					draw = true;
					gameState = MainMenu;
					break;
				}

				if (draw)
				{
					al_draw_filled_rectangle(0, 0, CurrentScreenWidth, CurrentScreenHeight, DarkRed);

					/* printing score */
					stringstream ss;
					ss << "Score: " << snake.GetSnakeCells().size();
					score = ss.str();
					al_draw_text(font, Yellow, CurrentScreenWidth / 2, (CurrentScreenHeight / 2) - 80, ALLEGRO_ALIGN_CENTER, score.c_str());

					al_draw_text(font, White, CurrentScreenWidth / 2, (CurrentScreenHeight / 2) - 30, ALLEGRO_ALIGN_CENTER, "Click to continue");
					/* -- mouse cursor -- */
					al_draw_bitmap(mouse, mouse_x, mouse_y, NULL);

					al_flip_display();
				}				
				break;
			}
		}
	}

	/* dealocating memory */
	al_destroy_display(display);
	al_destroy_font(font);
	al_destroy_timer(timer);
	al_destroy_bitmap(mouse);
	al_destroy_sample(over_button_sound);
	al_destroy_sample(pressed_button_sound);
	al_destroy_event_queue(event_queue);

	if (change_resolution)
	{
		CurrentScreenWidth = DefaultScreenWidth;
		CurrentScreenHeight = DefaultScreenHeight;
		start_game(Fullscreen);
	}
}