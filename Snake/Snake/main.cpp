#include "stdIncludes.h"

/* setting default resolution in windowed mode */
int CurrentScreenWidth = DefaultScreenWidth;
int CurrentScreenHeight = DefaultScreenHeight;

int main ()
{
	/* starting random seed */
	srand ((unsigned int) time(NULL));

	/* starting allegro 5 */
	if (!al_init())
	{
		al_show_native_message_box(NULL, NULL, "Could not initialize Allegro 5", NULL, NULL, NULL);
		return -1;
	}

	/* starting addons */
	al_init_image_addon();
	al_init_primitives_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_acodec_addon();
	al_install_keyboard();
	al_install_mouse();
	al_install_audio();

	/* calling start menu (windowed) */
	start_game(0);
	
	return 0;
}