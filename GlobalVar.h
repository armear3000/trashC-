
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdlib>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_native_dialog.h>
#include <math.h>
#include <stdio.h> 
#include <stdarg.h>
#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <Windows.h>
#include <box2d.h>


#include "Geometry.h"
#include "SystemFun.h"
#include "DevicesAndButtons.h"

#ifndef GLOBALVAR
#define GLOBALVAR



ALLEGRO_DISPLAY* display;
int width = 1000;
int height = 800;

mouses mouse;
keyboards kboard;
cameras camera;

ALLEGRO_FONT* font24 = NULL;


#endif