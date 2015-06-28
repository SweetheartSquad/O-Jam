#pragma once

#include <OJ_Bullet.h>

OJ_Bullet::OJ_Bullet(float _damage, Box2DWorld * _world, b2BodyType _bodyType, bool _defaultFixture, Shader * _shader, Texture * _texture, float _width, float _height, float _u, float _v, float _componentScale) :
	Box2DSprite(_world, _bodyType, _defaultFixture, _shader, _texture, _width, _height, _u, _v, _componentScale),
	destroyed(false),
	damage(_damage),
	age(0),
	life(3)
{

}	

OJ_Bullet::~OJ_Bullet(){

}

void OJ_Bullet::update(Step * _step){
	age += _step->deltaTime;
	if(age > life){
		destroyed = true;
	}
	Box2DSprite::update(_step);
}