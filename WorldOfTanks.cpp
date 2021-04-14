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

class Tank
{
public:
	Tank();
	~Tank();
	void Generate(double x, double y);
	void Move();
	void Control();
	void Draw();

private:
	ALLEGRO_TRANSFORM trans;
	
	int life;
	
	QVector der;
	double speed, speedRotate, angle;
	
	QPoint centre;
	double w, h;

	QPoint p1, p2, p3, p4;
	
	double towerX, towerY, towerR;
	double barrelX, barrelY, barrelW, barrelH;
};
Tank::Tank()
{
	der.init(1,0);
	angle = 0;
	w = 30;
	h = 50;
	speed = 5;
	speedRotate = 2;
	life = false;
}
Tank::~Tank()
{

}
void Tank::Generate(double x, double y)
{
	centre.init(x, y);
	life = 10;
}
void Tank::Move()
{
	x += dx*speed;
	y += dy*speed;
}
void Tank::Control()
{

	al_identity_transform(&trans);
	

	if (kboard.key[ALLEGRO_KEY_A]) {
		al_translate_transform(&trans, -x, -y);
		al_rotate_transform(&trans, -45 / FROMRADTOGRAD);
		al_translate_transform(&trans, x, y);
	}
		
	if (kboard.key[ALLEGRO_KEY_D]) {
		al_translate_transform(&trans, -x, -y);
		al_rotate_transform(&trans, 45 / FROMRADTOGRAD);
		al_translate_transform(&trans, x, y);
	}
	
}
void Tank::Draw()
{
	al_use_transform(&trans);
	al_draw_circle(x, y, 2, al_map_rgb(255, 255, 0), 2);
	al_draw_rectangle(x - w / 2, y - h / 2, x + w / 2, y + h / 2, al_map_rgb(255, 255, 0), 1);
	al_identity_transform(&trans);
	al_use_transform(&trans);
}

Tank player;

int main()
{
	ALLEGRO_EVENT ev;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* fps = NULL;

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

	al_register_event_source(event_queue, al_get_timer_event_source(fps));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	srand(time(0));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	font24 = al_load_font("resources/BRUSH-N.TTF", 24, 0);
	if (!font24) abort_example("Error load fail: resources/BRUSH-N.TTF.");

	////////////////// init


	player.Generate(100, 400);

	bool done = false, draw = false;
	al_start_timer(fps);
	
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

				player.Control();

				mouse_click = 0;
				kboard.last_key_refresher();
			}
		}

		if (draw && al_is_event_queue_empty(event_queue)) {      // ��������� 
			draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//al_draw_filled_rectangle(0,0,width,height,al_map_rgba(0,0,0,10));
			
			player.Draw();

			al_flip_display();
		}
	}

	// ����� ������ 

	al_destroy_font(font24);
	al_destroy_timer(fps);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}
