#pragma once

#include <OJ_Arena.h>
#include <Box2DWorld.h>
#include <Box2D/Box2D.h>

const float PI = 3.1415926;

OJ_Arena::OJ_Arena(Box2DWorld * _world, float _radius) :
	Entity(),
	world(_world)
{
	b2Vec2 vs[100];
	for(unsigned long int i = 0; i < 100; ++i) {
		float ang = PI * 2.0 / 100.0 * i;
		b2Vec2 pos(cos(ang) * _radius, sin(ang) * _radius);
		vs[i] = pos;
	}

	b2ChainShape chain;
	chain.CreateLoop(vs, 100);

	b2FixtureDef fd;
	fd.shape = &chain;

	b2BodyDef bodyDef;
	bodyDef.position.Set(0, 0);
	bodyDef.type = b2_staticBody;
	
	b2Body * chainBody = world->b2world->CreateBody(&bodyDef);
	b2Fixture * f = chainBody->CreateFixture(&fd);
}

OJ_Arena::~OJ_Arena() {
}