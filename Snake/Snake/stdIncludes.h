#ifndef STDINCLUDES_H
#define STDINCLUDES_H

/* allegro includes */
#include "allegro5\allegro.h"
#include "allegro5\allegro_native_dialog.h"
#include "allegro5\allegro_primitives.h"
#include "allegro5\allegro_image.h"
#include "allegro5\allegro_ttf.h"
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_audio.h"
#include "allegro5\allegro_acodec.h"

/* necessary includes */
#include <iostream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

/* global variables */
#define DefaultScreenWidth 800
#define DefaultScreenHeight 600
extern int CurrentScreenWidth;
extern int CurrentScreenHeight;
const float FPS = 60.0;
const float frameFPS = 12.0;

/* defining user-friendly color names */
#define Black al_map_rgb(0, 0, 0)
#define Green al_map_rgb(0, 255, 0)
#define DarkRed al_map_rgb(200, 0, 0)
#define Red al_map_rgb(255, 0, 0)
#define Yellow al_map_rgb(255, 255, 0)
#define DarkBlue al_map_rgb(0, 0, 150)
#define Blue al_map_rgb(0, 0, 255)
#define LightBlue al_map_rgb(30, 80, 255)
#define White al_map_rgb(255, 255, 255)

/* defining user-frinedly things */
#define BigWallpaper "res//images//StartMenuWallpaperHD.png"
#define SmallWallpaper "res//images//StartMenuWallpaper800x600.png"
#define MouseCursor "res//images//mouse_cursor.png"
#define Apple_png "res//images//apple.png"
#define MainFont "res//fonts//COOPBL.TTF"
#define CreditsFont "res//fonts//consola.ttf"
#define OverButton "res//sounds//over_button.wav"
#define PressedButton "res//sounds//pressed_button.wav"
#define EatApple "res//sounds//eat_apple.wav"
#define GameOverSound "res//sounds//game_over.wav"

/* including classes */
#include "Apple.h"
#include "Button.h"
#include "Snake.h"

/* including header files */
#include "Game.h"

#endif