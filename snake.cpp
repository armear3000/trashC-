#define _CRT_SECURE_NO_WARNINGS

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
#include <conio.h>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <Windows.h>

#include <iostream>

#include <vector>
#include <deque>
#include <tuple>
#include <algorithm>
#include <iterator>

#include "Geometry.h"
#include "SystemFun.h"
#include "DevicesAndButtons.h"

using namespace std;


ALLEGRO_DISPLAY* display;
int width = 800;
int height = 600;

mouses mouse;
keyboards kboard;

ALLEGRO_FONT* font24 = NULL;


////////////////////

// ����� 40 �� 30
const int max_x = 40;
const int max_y = 30;
const int title_size = 20;
int map[max_y][max_x];

void draw_map();
//

class player
{
public:
	player();
	~player();
	
	bool life;
	int len;
	int x, y;
	int dx, dy;
	int lx, ly;
	int ldx, ldy;
	int speed;
	deque<tuple<int,int,int,int>> kuveve;

	void control();
	void go();
	void draw();


private:

};
player::player()
{
	life = true;
	len = 6;
	speed = 1;
	x = 10;
	y = 10;
	dx = 1;
	dy = 0;
	lx = x - len;
	ly = y;
	ldx = dx;
	ldy = dy;
}
player::~player()
{
}
void player::control()
{
	if (kboard.last_key[ALLEGRO_KEY_A] && dx != 1) {
		dx = -1;
		dy = 0;
		kuveve.push_back({ x, y, dx, dy });
		kboard.last_key[ALLEGRO_KEY_A] = false;
	}
	else if (kboard.last_key[ALLEGRO_KEY_D] && dx != -1) {
		dx = 1;
		dy = 0;
		kuveve.push_back({ x, y, dx, dy });
		kboard.last_key[ALLEGRO_KEY_D] = false;
	}
	else if (kboard.last_key[ALLEGRO_KEY_W] && dy != 1) {
		dx = 0;
		dy = -1;
		kuveve.push_back({ x, y, dx, dy });
		kboard.last_key[ALLEGRO_KEY_W] = false;
	}
	else if (kboard.last_key[ALLEGRO_KEY_S] && dy != -1) {
		dx = 0;
		dy = 1;
		kuveve.push_back({ x, y, dx, dy });
		kboard.last_key[ALLEGRO_KEY_S] = false;
	}
		
}
void player::go()
{
	
	x += dx * speed;
	y += dy * speed;

	if (x >= max_x || x <= -1 || y >= max_y || y <= -1) {
		life = false;
	} else if (map[y][x] == 2)
		len++;
	else {
		lx += ldx * speed;
		ly += ldy * speed;
	}
	if (map[y][x] == 1) {
		life = false;
	}

	map[y][x] = 1;
	map[ly][lx] = 0;

	int tx, ty, tdx, tdy;
	if (!kuveve.empty()) {
		tie(tx, ty, tdx, tdy) = kuveve.front();
		if (tie(tx, ty) == tie(lx, ly)) {
			tie(ldx, ldy) = tie(tdx, tdy);
			kuveve.pop_front();
		}
	}

}
void player::draw()
{
	al_draw_filled_rectangle(x * title_size, y * title_size, x * title_size + title_size, y * title_size + title_size, al_map_rgb(255, 255, 255));

}

player snake;

void gen_apple();

int main()
{
	srand(time(0));
	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* fps = NULL;
	ALLEGRO_TIMER* cps = NULL;

	if (!al_init()) exit(1);
	display = al_create_display(width, height);
	al_set_display_flag(display, 0, true);
	//al_set_display_flag(display, ALLEGRO_FULLSCREEN_WINDOW, true);
	if (!display) exit(1);

	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	al_install_mouse();
	al_install_keyboard();

	event_queue = al_create_event_queue();
	fps = al_create_timer(1 / 60.0);
	cps = al_create_timer(1 / 5.0);

	al_register_event_source(event_queue, al_get_timer_event_source(fps));
	al_register_event_source(event_queue, al_get_timer_event_source(cps));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	srand(time(0));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	font24 = al_load_font("resources/BRUSH-N.TTF", 24, 0);
	if (!font24) abort_example("Error load fail: resources/BRUSH-N.TTF.");

	////////////////// init

	bool done = false, draw = false;
	al_start_timer(fps);
	al_start_timer(cps);

	for (; !done;) {
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {            // �������� ����
			done = true;
		}

		mouse.CoordinatesInit(ev);

		// ��������� ����
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			mouse.key[ev.mouse.button - 1] = true;
			mouse_click = ev.mouse.button;


		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			mouse.key[ev.mouse.button - 1] = false;
		}

		// ��������� ���������� 
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			kboard.key[ev.keyboard.keycode] = true;
			kboard.last_key[ev.keyboard.keycode] = true;
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			kboard.key[ev.keyboard.keycode] = false;
		}


		if (ev.type == ALLEGRO_EVENT_TIMER) {                    // ������� �������
			
			if (ev.timer.source == fps) {
				draw = true;


				
				
				
				mouse_click = 0;
			}
			if (ev.timer.source == cps) {
				if (snake.life) {
					snake.control();
					snake.go();
					gen_apple();
				}
			}
		}

		if (draw && al_is_event_queue_empty(event_queue)) {      // ��������� 
			draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//al_draw_filled_rectangle(0,0,width,height,al_map_rgba(0,0,0,10));
			if (!snake.life) {
				map[2][4] = 2; map[2][5] = 2; map[2][6] = 2; map[2][9] = 2; map[2][12] = 2;  map[2][14] = 2; map[2][16] = 2; map[2][17] = 2; map[2][18] = 2;  map[2][22] = 2; map[2][25] = 2; map[2][27] = 2; map[2][29] = 2;  map[2][30] = 2; map[2][31] = 2; map[2][33] = 2; map[2][34] = 2; map[2][35] = 2;
				map[3][4] = 2; map[3][8] = 2; map[3][10] = 2; map[3][12] = 2; map[3][13] = 2; map[3][14] = 2; map[3][16] = 2; map[3][21] = 2; map[3][23] = 2; map[3][25] = 2; map[3][27] = 2; map[3][29] = 2; map[3][33] = 2; map[3][35] = 2;
				map[4][4] = 2; map[4][8] = 2; map[4][9] = 2; map[4][10] = 2; map[4][12] = 2; map[4][14] = 2; map[4][16] = 2; map[4][17] = 2; map[4][18] = 2; map[4][21] = 2; map[4][23] = 2; map[4][25] = 2; map[4][27] = 2; map[4][29] = 2; map[4][30] = 2; map[4][31] = 2; map[4][33] = 2; map[4][34] = 2; map[4][35] = 2;
				map[5][4] = 2; map[5][6] = 2; map[5][8] = 2; map[5][10] = 2; map[5][12] = 2; map[5][14] = 2; map[5][16] = 2; map[5][21] = 2; map[5][23] = 2; map[5][25] = 2; map[5][27] = 2; map[5][29] = 2; map[5][33] = 2; map[5][34] = 2;
				map[6][4] = 2; map[6][5] = 2; map[6][6] = 2; map[6][8] = 2; map[6][10] = 2; map[6][12] = 2; map[6][14] = 2; map[6][16] = 2; map[6][17] = 2; map[6][18] = 2; map[6][22] = 2; map[6][26] = 2; map[6][29] = 2; map[6][30] = 2; map[6][31] = 2; map[6][33] = 2; map[6][35] = 2;
			}
			draw_map();

			snake.draw();


			al_flip_display();
		}
	}

	// ����� ������ 

	al_destroy_font(font24);
	al_destroy_timer(fps);
	al_destroy_timer(cps);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}

void draw_map()
{
	for (int y = 0; y < max_y; ++y) {
		for (int x = 0; x < max_x; ++x) {
				al_draw_rectangle(x * title_size, y * title_size, x * title_size + title_size, y * title_size + title_size, al_map_rgb(20, 20, 20), 1);
		}
	}
	for (int y = 0; y < max_y; ++y) {
		for (int x = 0; x < max_x; ++x) {
			if (map[y][x] == 1)
				al_draw_filled_rectangle(x * title_size, y * title_size, x * title_size + title_size, y * title_size + title_size, al_map_rgb(200, 200, 200));
		}
	}
	for (int y = 0; y < max_y; ++y) {
		for (int x = 0; x < max_x; ++x) {
			if (map[y][x] == 2)
				al_draw_filled_rectangle(x * title_size, y * title_size, x * title_size + title_size, y * title_size + title_size, al_map_rgb(255, 0, 0));
		}
	}
}

void gen_apple()
{
	int rx, ry;
	do
	{
		rx = rand() % max_x;
		ry = rand() % max_y;

	} while (map[ry][rx] != 0);

	map[ry][rx] = 2;

}