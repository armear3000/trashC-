
#include "GlobalVar.h"

#include "Geometry.h"
#include "SystemFun.h"
#include "DevicesAndButtons.h"
#include "EngineBox2d.h"

using namespace std;


///// box2d /////

b2Vec2 gravity(0.0f, 0.0f);
b2World world(gravity);


float timeStep = 1.0f / 60.0f;
int32 velocityIterations = 6;
int32 positionIterations = 2;


///// box2d /////
class map
{
public:
	map() {}
	~map() {}
	void init();
	void draw();	

	eb2_object wall[4];
	eb2_object trash[100];

private:
};
void map::init()
{
	wall[0].init(world, width / 2, 0, width / 2, 10, b2_staticBody, SCENERY, MASK_SCENERY);
	wall[1].init(world, width, height / 2, 10, height / 2, b2_staticBody, SCENERY, MASK_SCENERY);
	wall[2].init(world, width / 2, height, width / 2, 10, b2_staticBody, SCENERY, MASK_SCENERY);
	wall[3].init(world, 0, height / 2, 10, height / 2, b2_staticBody, SCENERY, MASK_SCENERY);

	for (int y = 0; y < 10; ++y)
		for (int i = 0; i < 10; ++i)
			trash[10 * y + i].init(world, width / 2 + i * 10, height / 2 + y * 10, 10, 10, b2_dynamicBody, 1, SCENERY, MASK_SCENERY);
		

}
void map::draw()
{
	for (int i = 0; i < 4; ++i) {
		wall[i].draw();
	}

	for (int y = 0; y < 10; ++y)
		for (int i = 0; i < 10; ++i)
			trash[10 * y + i].draw();
}

map poligon;

class Tank
{
public:
	Tank();
	~Tank();
	void Generate(double x, double y);
	void Move();
	void Control();
	void Draw();
	b2Vec2 GetPosition();

	eb2_Join towerJoinCore;
	eb2_Join towerBalanceJoinTower;
	eb2_object core;
	eb2_object tower;
	eb2_object towerBalance;
private:
	ALLEGRO_TRANSFORM trans;
	
	int life;
	double speed, speedRotate, speedTowerRotate;
	double w, h;
	double wt, ht;
	double wtb, htb;
};
Tank::Tank()
{
	w = 50;
	h = 30;
	wt = 40;
	ht = 10;
	wtb = 20;
	htb = 20;

	speed = 3.0f;
	speedRotate = 0.5f;
	speedTowerRotate = 4.0f;
	life = false;
}
Tank::~Tank()
{

}
void Tank::Generate(double x, double y)
{
	core.init(world, x, y, w / 2, h / 2, b2_dynamicBody, 2, PLAYER, MASK_PLAYER);
	
	tower.init(world, x, y, wt / 2, ht / 2, b2_dynamicBody, 0.1f, PLAYER, MASK_PLAYER);
	tower.Body->SetAngularDamping(0.0f);
	tower.Body->SetLinearDamping(0.0f);
	towerBalance.init(world, x, y, wtb / 2, htb / 2, b2_dynamicBody, 0.1f, PLAYER, MASK_PLAYER);
	towerBalance.Body->SetAngularDamping(0.0f);
	towerBalance.Body->SetLinearDamping(0.0f);

	towerJoinCore.CreateRevoluteJoin(world, core, tower, 0, 0, -wt/2, 0);
	towerBalanceJoinTower.CreateWeldJoin(world, towerBalance, tower, wtb/2, 0, -wt/2, 0);

}
void Tank::Move()
{
}
void Tank::Control()
{
	if (kboard.key[ALLEGRO_KEY_W]) {
		double angle = core.Body->GetAngle();
		b2Vec2 vec(cos(angle) * speed,sin(angle) * speed);
		core.Body->ApplyLinearImpulseToCenter(vec, 1);
	}
	if (kboard.key[ALLEGRO_KEY_S]) {
		double angle = core.Body->GetAngle();
		b2Vec2 vec(-cos(angle) * speed, -sin(angle) * speed);
		core.Body->ApplyLinearImpulseToCenter(vec, 1);
	}
	if (kboard.key[ALLEGRO_KEY_D]) {
		core.Body->ApplyAngularImpulse(speedRotate, 1);
	}
	if (kboard.key[ALLEGRO_KEY_A]) {
		core.Body->ApplyAngularImpulse(-speedRotate, 1);
	}

	QVector v1(cos(tower.Body->GetAngle()), sin(tower.Body->GetAngle()));
	QVector v2(mouse.x - width / 2, mouse.y - height / 2);
	double Velocity = 0;
	double cbAngleVel = core.Body->GetAngularVelocity();

	if(v1 % v2 < 0) {
		Velocity = -speedTowerRotate;
	}
	else if(v1 % v2 > 0){
		Velocity = speedTowerRotate;
	}
	else if(v1 * v2 < 0){
		Velocity = speedTowerRotate;
	}
	if (Velocity * cbAngleVel <= 0) {
		cbAngleVel /= 5;
	}
	Velocity += cbAngleVel;

	double v1v2 = v1 % v2;
	double v1v2t = v1 * v2;
	double angle = Velocity * timeStep + tower.Body->GetAngle();
	v1.init(cos(angle), sin(angle));
	if(v1v2 * (v1 % v2) > 0)
		tower.Body->SetAngularVelocity(Velocity);
	else {
		double newVelocity = atan2(v1v2, v1v2t) / timeStep;
		if (newVelocity < cbAngleVel - speedTowerRotate)
			tower.Body->SetAngularVelocity(cbAngleVel - speedTowerRotate);
		else if(newVelocity > cbAngleVel + speedTowerRotate)
			tower.Body->SetAngularVelocity(cbAngleVel + speedTowerRotate);
		else
			tower.Body->SetAngularVelocity(newVelocity);
	}	
		
}
void Tank::Draw()
{	
	core.draw();
	tower.draw();
	towerBalance.draw();
}
b2Vec2 Tank::GetPosition()
{
	return core.Body->GetPosition();
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
	
	///// box2d ///// 

	poligon.init();
	player.Generate(100, 400);

	bool done = false, draw = false;
	al_start_timer(fps);

	for (; !done;) {
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {      
			done = true;
		}

		mouse.CoordinatesInit(ev);

		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
			mouse.key[ev.mouse.button - 1] = true;
			mouse_click = ev.mouse.button;


		}
		if (ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			mouse.key[ev.mouse.button - 1] = false;
		}

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			kboard.key[ev.keyboard.keycode] = true;
			kboard.last_key[ev.keyboard.keycode] = true;
		}
		if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			kboard.key[ev.keyboard.keycode] = false;
		}

		if (ev.type == ALLEGRO_EVENT_TIMER) {               

			if (ev.timer.source == fps) {
				draw = true;

				///// box2d /////
				world.Step(timeStep, velocityIterations, positionIterations);
				

				///// box2d /////
				
				player.Control();

				mouse_click = 0;
				kboard.last_key_refresher();
			}
		}

		if (draw && al_is_event_queue_empty(event_queue)) {    
			draw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));
			//al_draw_filled_rectangle(0,0,width,height,al_map_rgba(0,0,0,10));
			
			double posx = player.tower.Body->GetPosition().x * sc;
			double posy = player.tower.Body->GetPosition().y * sc;

			camera.move(player.GetPosition().x * sc - width /2, player.GetPosition().y * sc - height / 2);

			///// box2d /////
			poligon.draw();


			///// box2d /////

			player.Draw();

			camera.refresher();

			al_flip_display();
		}
	}


	al_destroy_font(font24);
	al_destroy_timer(fps);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}
