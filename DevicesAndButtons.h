#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_native_dialog.h>
#include <stdarg.h>
#include <stdio.h>
#include "Geometry.h"
#include "SystemFun.h"


#ifndef DEVICESANDBUTTONS
#define DEVICESANDBUTTONS

int mouse_click;

void Nathing();


class cameras
{
public:
	
	void move(double x, double y);
	void refresher();
	ALLEGRO_TRANSFORM trans;
	ALLEGRO_TRANSFORM transTemp;

private:
	double x, y;

};
inline void cameras::move(double x, double y)
{
	al_identity_transform(&trans);
	al_translate_transform(&trans, -x, -y);
	al_use_transform(&trans);
	transTemp = *al_get_current_transform();
}
inline void cameras::refresher()
{
	al_identity_transform(&trans);
	al_use_transform(&trans);
}

class mouses
{
public:
	double x;
	double y;
	double z;
	bool key[20];
	void CoordinatesInit(ALLEGRO_EVENT ev1);

	mouses();
};
mouses::mouses() {
	x = 0;
	y = 0;
	z = 0;
	for (int i = 0; i < 20; i++) key[i] = false;
	return;
}
void mouses::CoordinatesInit(ALLEGRO_EVENT ev1) {
	if (ev1.type == ALLEGRO_EVENT_MOUSE_AXES) {
		x = ev1.mouse.x;
		y = ev1.mouse.y;
		z = ev1.mouse.z;
	}
}

class keyboards
{
public:
	
	bool key[ALLEGRO_KEY_MAX];
	bool last_key[ALLEGRO_KEY_MAX];
	void last_key_refresher();
	keyboards();
};
keyboards::keyboards()
{
	for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
		key[i] = false;
		last_key[i] = false;
	}
}
void keyboards::last_key_refresher() {
	for (int i = 0; i < ALLEGRO_KEY_MAX; i++) {
		last_key[i] = false;
	}
}

class Button {
public:
	Button *last, *next;
	int type;
	double x1, y1;
	double x2, y2;
	double R;
	bool activ;
	void(*function)();
	void init(double, double, double, double, void(*function1)());
	void init(double, double, double, void(*function1)());
	bool collide(mouses temp1);

};
void Button::init(double x1_1, double y1_1, double x2_2, double y2_2, void(*function1)() = Nathing) {
	x1 = x1_1;
	y1 = y1_1;
	x2 = x2_2;
	y2 = y2_2;
	function = function1;
	type = 0;
	activ = false;
}
void Button::init(double x1_1, double y1_1, double R1, void(*function1)() = Nathing) {
	x1 = x1_1;
	y1 = y1_1;
	R = R1;
	function = function1;
	type = 1;
	activ = false;
}
bool Button::collide(mouses temp1) {
	switch (type) {
	case 0:
		return CollideDetect(temp1.x, temp1.y, temp1.x + 1, temp1.y + 1, x1, y1, x2, y2);
		break;
	case 1:
		return CollideDetect(temp1.x, temp1.y, 1, x1, y1, R);
		break;
	default:
		return 0;
		break;
	}
}

class Buttons {
public:
	Button *first;
	Button *posl;
	Buttons();
	Button* add();
	void del(Button*);
	void destroy();
	void collide(mouses temp);
	void use();
	void draw(ALLEGRO_COLOR FilledRect, ALLEGRO_COLOR Rect, int SizeRect);
};
Buttons::Buttons() {
	first = posl = NULL;

}
Button* Buttons::add() {
	Button* temp = new Button; // динамическая память 
	if (!temp) exit(1);
	if (!first) {
		posl = first = temp;
		temp->last = temp->next = NULL;
	}
	else {
		posl->next = temp;
		temp->last = posl;
		posl = temp;
		temp->next = NULL;
	}
	return temp;
}
void Buttons::del(Button* B) {
	if (B == first) {
		first = first->next;
		if (first) first->last = NULL;
	}
	else if (B == posl) {
		posl = posl->last;
		posl->next = NULL;
	}
	else {
		B->last->next = B->next;
		B->next->last = B->last;
	}
	delete B; // только для динамики
}
void Buttons::destroy() {
	while (first) {
		del(first);
	}
}
void Buttons::collide(mouses temp) {
	Button* t = posl;
	for (; t; t = t->last) {
		if (t->collide(temp)) {
			t->activ = true;
			break;
		}
		else {
			t->activ = false;
		}
	}
	if (t)
		for (t = t->last; t; t = t->last)
			t->activ = false;
}
void Buttons::use() {
	Button* t = posl;
	for (; t; t = t->last) {
		if (t->activ && mouse_click == 1) {
			t->function();
			break;
		}
	}
}
void Buttons::draw(ALLEGRO_COLOR FilledRect, ALLEGRO_COLOR Rect, int SizeRect) {
	Button* t = posl;
	for (; t; t = t->last) {
		al_draw_filled_rectangle(t->x1, t->y1, t->x2, t->y2, FilledRect);
		al_draw_rectangle(t->x1, t->y1, t->x2, t->y2, Rect, SizeRect);
	}

}

void Nathing() {
	return;
}


#endif