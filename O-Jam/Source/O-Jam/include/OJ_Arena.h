#pragma once

#include <Entity.h>

class Box2DWorld;

class OJ_Arena : public Entity{
public:
	Box2DWorld * world;

	OJ_Arena(Box2DWorld * _world, float _radius);
	~OJ_Arena();
};