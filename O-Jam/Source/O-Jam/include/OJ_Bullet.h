#pragma once

#include <Box2DSprite.h>

class OJ_Bullet : public Box2DSprite{
public:
	float age;
	float life;
	float damage;
	bool destroyed;

	explicit OJ_Bullet(float _damage, Box2DWorld * _world, b2BodyType _bodyType = b2_dynamicBody, bool _defaultFixture = true, Shader * _shader = nullptr, Texture * _texture = nullptr, float _width = 1.f, float _height = 1.f, float _u = 0.f, float _v = 0.f, float _componentScale = 1.f);
	~OJ_Bullet();

	virtual void update(Step * _step) override;
};