#pragma once

#include <OJ_Character.h>
#include <OJ_ResourceManager.h>
#include <Box2DSprite.h>
#include <OJ_Game.h>

OJ_TexturePack::OJ_TexturePack(std::string _torsoSrc, std::string _handSrc) :
	torsoTex(OJ_ResourceManager::playthrough->getTexture(_torsoSrc)->texture),
	handTex(OJ_ResourceManager::playthrough->getTexture(_handSrc)->texture)
{
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


OJ_Character::OJ_Character(float _damage, float _componentScale, OJ_TexturePack* _texPack, Box2DWorld* _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
	speed(1.0f),
	health(100.f),
	damage(_damage),
	dead(false),
	justTookDamage(false),
	texPack(_texPack)
{
	componentScale = _componentScale;
	torso = new Box2DSprite(world, b2_dynamicBody, false, nullptr, texPack->torsoTex, 1, 1, 0, 0, componentScale);

	addComponent(&torso);

	sf.categoryBits = categoryBits;
	if(maskBits != static_cast<int16>(-1)){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;

	rootComponent = torso;

	rootComponent->maxVelocity = b2Vec2(componentScale*10.f, componentScale*10.f);
	rootComponent->body->SetLinearDamping(1);
	
	b2Fixture * f = torso->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(10.f);
}

OJ_Character::~OJ_Character() {
}

void OJ_Character::update(Step * _step){
	Box2DSuperSprite::update(_step);

	if(health <= 0.0f) {
		die();
	}
}

void OJ_Character::move(glm::vec2 _v){
	if(_v.x != 0 || _v.y != 0){
		float s = speed * rootComponent->body->GetMass();
		rootComponent->applyLinearImpulseUp(_v.y * s);
		rootComponent->applyLinearImpulseRight(_v.x * s);
	}
}

void OJ_Character::takeDamage(float _damage){
	health -= _damage;

	//justTookDamage = true;
}

void OJ_Character::die(){
	dead = true;
}