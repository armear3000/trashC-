#include "GlobalVar.h"

#ifndef ENGINEBOX2D
#define ENGINEBOX2D

enum _entityCategory {
	SCENERY = 0x0001,           // 0000000000000001 in binary
	FRIENDLY_SHIP = 0x0002,     // 0000000000000010 in binary
	ENEMY_SHIP = 0x0004,        // 0000000000000100 in binary
	PLAYER = 0x0008,            // 0000000000001000 in binary
};
enum _entityMask {
	MASK_SCENERY = -1,     
	MASK_FRIENDLY_SHIP = SCENERY | ENEMY_SHIP | PLAYER,     
	MASK_ENEMY_SHIP = SCENERY | FRIENDLY_SHIP | PLAYER,     
	MASK_PLAYER = SCENERY | ENEMY_SHIP | FRIENDLY_SHIP,    
};

const float sc = 25.0f;

class eb2_object {
public:
	void init(b2World& world, double x, double y, double w, double h, b2BodyType type, double density, short categoryBits, short maskBits);
	void init(b2World& world, double x, double y, double r, b2BodyType type, double density, short categoryBits, short maskBits);
	void draw();

	eb2_object() {
	
	}
	~eb2_object() {
	
	}

	b2Body* Body;

private:
	enum tShape { rectangle, circle };
	int typeShape;
	double w, h;
};
inline void eb2_object::init(b2World& world, double x, double y, double w, double h, b2BodyType type, double density = 1, short categoryBits = 0, short maskBits = 0)
{
	b2BodyDef BodyDef;
	BodyDef.type = type;
	BodyDef.position.Set(x / sc, y / sc);
	Body = world.CreateBody(&BodyDef);

	typeShape = rectangle;
	this->w = w;
	this->h = h;

	b2Vec2 vertices[4];
	vertices[0].Set(-w / sc, -h / sc);
	vertices[1].Set(w / sc, -h / sc);
	vertices[2].Set(w / sc, h / sc);
	vertices[3].Set(-w / sc, h / sc);
	int count = 4;

	b2PolygonShape PolygonShape;
	b2FixtureDef fixtureDef;

	PolygonShape.Set(vertices, count);
	if (type == b2_dynamicBody) {
		fixtureDef.shape = &PolygonShape;
		fixtureDef.density = density;
		fixtureDef.friction = 0.3f;
		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;
		Body->CreateFixture(&fixtureDef);
		Body->SetAngularDamping(10.0f);
		Body->SetLinearDamping(10.0f);
	}
	else {
		Body->CreateFixture(&PolygonShape, 0.0f);
	}
}
inline void eb2_object::init(b2World& world, double x, double y, double r, b2BodyType type, double density = 1, short categoryBits = 0, short maskBits = 0)
{
	b2BodyDef BodyDef;
	BodyDef.type = type;
	BodyDef.position.Set(x / sc, y / sc);
	Body = world.CreateBody(&BodyDef);

	typeShape = circle;
	this->w = r;
	this->h = r;

	b2CircleShape circle;
	circle.m_radius = r / sc;

	b2FixtureDef fixtureDef;

	if (type == b2_dynamicBody) {
		fixtureDef.shape = &circle;
		fixtureDef.density = density;
		fixtureDef.friction = 0.3f;
		fixtureDef.filter.categoryBits = categoryBits;
		fixtureDef.filter.maskBits = maskBits;
		Body->CreateFixture(&fixtureDef);
		Body->SetAngularDamping(10.0f);
		Body->SetLinearDamping(10.0f);
	}
	else {
		Body->CreateFixture(&circle, 0.0f);
	}
}
inline void eb2_object::draw()
{

	b2Vec2 Pos = Body->GetPosition();
	double angle = Body->GetAngle();
	ALLEGRO_TRANSFORM trans;


	al_identity_transform(&trans);
	al_translate_transform(&trans, -Pos.x * sc, -Pos.y * sc);
	al_rotate_transform(&trans, angle);
	al_translate_transform(&trans, Pos.x * sc, Pos.y * sc);
	al_compose_transform(&trans, &camera.transTemp);
	al_use_transform(&trans);

	if (typeShape == rectangle) {
		al_draw_rectangle(Pos.x * sc - w, Pos.y * sc - h, Pos.x * sc + w, Pos.y * sc + h, al_map_rgb(255, 0, 255), 1);
		al_draw_line(Pos.x * sc, Pos.y * sc, Pos.x * sc + w, Pos.y * sc, al_map_rgb(255, 0, 255), 1);
		if (Body->GetType() == b2_staticBody) {
			al_draw_line(Pos.x * sc - w, Pos.y * sc - h, Pos.x * sc + w, Pos.y * sc + h, al_map_rgb(255, 0, 255), 1);
			al_draw_line(Pos.x * sc - w, Pos.y * sc + h, Pos.x * sc + w, Pos.y * sc - h, al_map_rgb(255, 0, 255), 1);
		}
	}
	else if (typeShape == circle) {
		al_draw_circle(Pos.x * sc, Pos.y * sc, w, al_map_rgb(255, 0, 255), 1);
		al_draw_line(Pos.x * sc, Pos.y * sc, Pos.x * sc + w, Pos.y * sc, al_map_rgb(255, 0, 255), 1);
		if (Body->GetType() == b2_staticBody) {
			al_draw_line(Pos.x * sc - w, Pos.y * sc, Pos.x * sc + w, Pos.y * sc, al_map_rgb(255, 0, 255), 1);
			al_draw_line(Pos.x * sc, Pos.y * sc - h, Pos.x * sc, Pos.y * sc + h, al_map_rgb(255, 0, 255), 1);
		}
	}

	al_identity_transform(&trans);
	al_use_transform(&trans);
}

class eb2_Join
{
public:
	void CreateRevoluteJoin(b2World& world, eb2_object &A, eb2_object &B, double x1, double y1, double x2, double y2);
	void CreateDistanceJoin(b2World& world, eb2_object &A, eb2_object &B, double x1, double y1, double x2, double y2, double length);
	void CreateWeldJoin(b2World& world, eb2_object& A, eb2_object& B, double x1, double y1, double x2, double y2);

	eb2_Join() {
	
	}
	~eb2_Join() {
	
	}

private:

};
inline void eb2_Join::CreateRevoluteJoin(b2World& world, eb2_object &A, eb2_object &B, double x1, double y1, double x2, double y2)
{
	b2RevoluteJointDef JoinDef;
	JoinDef.bodyA = A.Body;
	JoinDef.bodyB = B.Body;
	JoinDef.collideConnected = false;
	JoinDef.localAnchorA.Set(x1 / sc, y1 / sc);
	JoinDef.localAnchorB.Set(x2 / sc, y2 / sc);
	world.CreateJoint(&JoinDef);
}
inline void eb2_Join::CreateDistanceJoin(b2World& world, eb2_object& A, eb2_object& B, double x1, double y1, double x2, double y2, double length)
{
	b2DistanceJointDef JoinDef;

	JoinDef.length = length;
	JoinDef.bodyA = A.Body;
	JoinDef.bodyB = B.Body;
	JoinDef.collideConnected = false;
	JoinDef.localAnchorA.Set(x1 / sc, y1 / sc);
	JoinDef.localAnchorB.Set(x2 / sc, y2 / sc);
	world.CreateJoint(&JoinDef);
}
inline void eb2_Join::CreateWeldJoin(b2World& world, eb2_object& A, eb2_object& B, double x1, double y1, double x2, double y2)
{
	b2WeldJointDef JoinDef;

	JoinDef.bodyA = A.Body;
	JoinDef.bodyB = B.Body;
	JoinDef.collideConnected = false;
	JoinDef.localAnchorA.Set(x1 / sc, y1 / sc);
	JoinDef.localAnchorB.Set(x2 / sc, y2 / sc);
	world.CreateJoint(&JoinDef);

}

#endif