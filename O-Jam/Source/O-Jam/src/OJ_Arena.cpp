#pragma once

#include <OJ_Arena.h>
#include <Box2DWorld.h>
#include <Box2D/Box2D.h>

const float PI = 3.1415926;

OJ_Arena::OJ_Arena(Box2DWorld * _world, float _radius, int _points) :
	Entity(),
	world(_world)
{

	b2Vec2 * vs = (b2Vec2 *)malloc(sizeof(b2Vec2) * _points);
	for(unsigned long int i = 0; i < _points; ++i) {
		float ang = PI * 2.0 / _points * i;
		b2Vec2 pos(cos(ang) * _radius, sin(ang) * _radius);
		vs[i] = pos;
	}

	b2ChainShape chain;
	chain.CreateLoop(vs, _points);

	b2FixtureDef fd;
	fd.shape = &chain;

	b2BodyDef bodyDef;
	bodyDef.position.Set(0, 0);
	bodyDef.type = b2_staticBody;
	
	b2Body * chainBody = world->b2world->CreateBody(&bodyDef);
	b2Fixture * f = chainBody->CreateFixture(&fd);

	free(vs);
}

OJ_Arena::~OJ_Arena() {
}