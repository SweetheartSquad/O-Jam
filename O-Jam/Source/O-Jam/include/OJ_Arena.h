#pragma once

#include <Entity.h>

class Box2DWorld;
class Shader;

class OJ_Arena : public Entity{
public:
	Box2DWorld * world;

	OJ_Arena(Box2DWorld * _world, Shader * _shader, float _radius, int _points);
	~OJ_Arena();
};