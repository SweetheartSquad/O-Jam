#pragma once

#include <OJ_Enemy.h>
#include <Box2DSprite.h>

OJ_Enemy::OJ_Enemy(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	OJ_Character(5.f, _componentScale, _texPack, _world, _categoryBits, _maskBits, _groupIndex),
	target(nullptr),
	scoreVal(50.f)
{

}

OJ_Enemy::~OJ_Enemy(){

}

void OJ_Enemy::update(Step * _step){
	if(target != nullptr){
		moveTowards(target->rootComponent->body->GetPosition() + b2Vec2(std::rand()%25 - std::rand()%25, std::rand()%25 - std::rand()%25));
	}
	OJ_Character::update(_step);
}

void OJ_Enemy::targetCharacter(OJ_Character * _target) {
	target = _target;
}

void OJ_Enemy::nullifyTarget() {
	target = nullptr;
}

void OJ_Enemy::moveTowards(b2Vec2 _pos){
	b2Vec2 bodyPos = rootComponent->body->GetPosition();
	b2Vec2 dir = b2Vec2(_pos.x - bodyPos.x, _pos.y - bodyPos.y);
	rootComponent->applyLinearImpulseToCenter(dir.x * speed, dir.y * speed);
}
