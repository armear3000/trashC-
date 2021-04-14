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

class block
{
public:
	block();
	~block();

	double x, y, w, h;
	bool life;
	void die();
	void SetCor(double x, double y);
	void Draw();
};
block::block()
{
	w = 50;
	h = 25;
	life = true;
}
block::~block()
{
}
void block::die()
{
	life = false;
}
void block::SetCor(double x, double y)
{
	this->x = x;
	this->y = y;
}
void block::Draw()
{
	if(life)
		al_draw_filled_rectangle(x, y, x + w, y + h, al_map_rgb(rand() % 255, 0, 0));
}

block blocks[50];
int maxX = 10, maxY = 5;
int Nb = 50;
void GenerateBlocks();
void DrawBlocks();

class player
{
public:
	player();
	~player();
	void draw();
	void mathrun();
	double x, y, w, h;
};
player::player()
{
	y = double(height) - 40;
	x = double(width)/2 - 50;
	w = 200;
	h = 20;

}
player::~player()
{
}
void player::draw()
{
	double c1 = x + w / 5 * 2;
	double c2 = x + w / 5 * 3;
	al_draw_circle(c1, y, 3, al_map_rgb(0, 255, 0), 2);
	al_draw_circle(c2, y, 3, al_map_rgb(0, 255, 0), 2);
	al_draw_rectangle(x, y, x + w, y + h, al_map_rgb(255, 255, 255),2);
}
void player::mathrun()
{
	x = mouse.x - w/2;
	if (x < 0) x = 0;
	else if (x + w > width) x = width - w;
}

player p;

class balls
{
public:
	balls();
	~balls();
	void mathrun();
	void draw();

	double x, y, r;
	double speed,dx, dy, angle;
};
balls::balls()
{
	x = width / 2;
	y = height - 150.0;
	r = 5;
	speed = 8;
	angle = 300;

	dx = 0;
	dy = -1;
}
balls::~balls()
{
}
void balls::mathrun()
{
	double tempX = x;
	double tempY = y;
	x += dx*speed;
	y += dy*speed;
	
	if (x + r > width) {
		dx = -dx;
		x = width - r;
	}
	else if (x - r < 0) {
		dx = -dx;
		x = r;
	}
	if (y + r > height) {
		dy = -dy;
		y = height - r;
	}
	else if (y - r < 0) {
		dy = -dy;
		y = r;
	}
	if (CollideDetect(p.x,p.y, p.x + p.w, p.y + p.h, x, y, r)){
		
		double c1 = p.x + p.w / 5 * 2;
		double c2 = p.x + p.w / 5 * 3;
		double w = p.w / 5 * 2;

		if (x < c1) {
			dx = cos(((45 / w) * (c1 - x) + 90) / FROMRADTOGRAD);
			dy = -sin(((45 / w) * (c1 - x) + 90) / FROMRADTOGRAD);
		}
		if (x >= c1 && x <= c2) {
			dx = 0;
			dy = -1;
		}
		if (x > c2) {
			dx = cos((-(45 / w) * (x - c2) + 90) / FROMRADTOGRAD); 
			dy = -sin((-(45 / w) * (x - c2) + 90) / FROMRADTOGRAD);
		}
	}
	for (int i = 0; i < Nb; ++i) {
		if (blocks[i].life && CollideDetect(blocks[i].x, blocks[i].y, blocks[i].x + blocks[i].w, blocks[i].y + blocks[i].h, x, y, r)) {
			blocks[i].die();
			if (tempX  < blocks[i].x) {
				if (tempY < blocks[i].y) {
					dy = -dy;
				}
				else if (tempY > blocks[i].y + blocks[i].h) {
					dy = -dy;
				}
				else {
					dx = -dx;
				}
			}
			else if (tempX > blocks[i].x + blocks[i].w) {
				if (tempY < blocks[i].y) {
					dy = -dy;
				}
				else if (tempY > blocks[i].y + blocks[i].h) {
					dy = -dy;
				}
				else {
					dx = -dx;
				}
			}
			else if (tempY < blocks[i].y) {
				dy = -dy;
			
			}
			else {
				dy = -dy;
			}
			
		}
	
	}
}
void balls::draw()
{
	
	al_draw_filled_circle(x, y, r, al_map_rgb(255, 255, 255));

	al_draw_textf(font24, al_map_rgb(255, 255, 255), x + r, y - 24, 0, "dx: %.4f", dx);
	al_draw_textf(font24, al_map_rgb(255, 255, 255), x + r, y - 48, 0, "dy: %.4f", dy);


	al_draw_line(x, y, x + dx * (r + 50), y, al_map_rgb(0, 255, 0), 2);
	al_draw_line(x, y, x, y + dy * (r + 50), al_map_rgb(0, 255, 0), 2);

	al_draw_circle(x, y, r + 50, al_map_rgb(150, 150, 150), 1);
}

balls ball;




int main()
{
	srand(time(0));
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

	bool done = false, draw = false;
	al_start_timer(fps);
	GenerateBlocks();

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

				p.mathrun();

				ball.mathrun();

				mouse_click = 0;
				kboard.last_key_refresher();
			}
		}

		if (draw && al_is_event_queue_empty(event_queue)) {      // ��������� 
			draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//al_draw_filled_rectangle(0,0,width,height,al_map_rgba(0,0,0,10));
			DrawBlocks();

			p.draw();
			ball.draw();

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


void GenerateBlocks()
{
	int i = 0;
	double coretX = width / 2 - maxX/2* blocks[0].w;

	for (int y = 0; y < maxY; ++y) {
		for (int x = 0; x < maxX; ++x) {
			blocks[i].SetCor(x * blocks[i].w + coretX, y * blocks[i].h + 50);
			i++;
		}
	}
}
void DrawBlocks()
{
	int i = 0;
	for (int y = 0; y < maxY; ++y) {
		for (int x = 0; x < maxX; ++x) {
			blocks[i].Draw();
			i++;
		}
	}
}