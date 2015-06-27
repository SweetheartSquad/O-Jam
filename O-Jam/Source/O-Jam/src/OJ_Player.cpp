#pragma once

#include <OJ_Player.h>
#include <OJ_ResourceManager.h>
#include <Keyboard.h>

#include <glfw\glfw3.h>

OJ_TexturePack::OJ_TexturePack(std::string _torsoSrc, std::string _handSrc) :
	torsoTex(OJ_ResourceManager::playthrough->getTexture(_torsoSrc)->texture),
	handTex(OJ_ResourceManager::playthrough->getTexture(_handSrc)->texture)
{
}

OJ_Player::OJ_Player(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
	keyboard(&Keyboard::getInstance())
{
	if(_texPack == nullptr){
		_texPack = new OJ_TexturePack("torso", "hand");
	}

	torso = new Box2DSprite(world, b2_dynamicBody, false, nullptr, _texPack->torsoTex);
	handR = new Box2DSprite(world, b2_dynamicBody, false, nullptr, _texPack->handTex);
	handL = new Box2DSprite(world, b2_dynamicBody, false, nullptr, _texPack->handTex);
	
	addComponent(&torso);
	addComponent(&handR);
	addComponent(&handL);

	torso->parents.at(0)->scale(10, 10, 10);
	handR->parents.at(0)->scale(5, 5, 5);
	handL->parents.at(0)->scale(-5, 5, 5);

	rootComponent = torso;

	rootComponent->maxVelocity = b2Vec2(5.f, 5.f);

	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != static_cast<int16>(-1)){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;

	torso->createFixture(sf, b2Vec2(0.f, 0.f), this);
	handR->createFixture(sf, b2Vec2(0.f, 0.f), this);
	handL->createFixture(sf, b2Vec2(0.f, 0.f), this);
	

	////////////
	// JOINTS //
	////////////
	{
		b2DistanceJointDef j;
		j.bodyA = torso->body;
		j.bodyB = handR->body;
		j.localAnchorA.Set(0, 0);
		j.localAnchorB.Set(0, 0);
		j.collideConnected = false;
		j.length = 3.0f;
		world->b2world->CreateJoint(&j);
	}
	{
		b2DistanceJointDef j;
		j.bodyA = torso->body;
		j.bodyB = handL->body;
		j.localAnchorA.Set(0, 0);
		j.localAnchorB.Set(0, 0);
		j.collideConnected = false;
		j.length = 3.0f;
		world->b2world->CreateJoint(&j);
	}
}

void OJ_Player::update(Step * _step){

	if(keyboard->keyDown(GLFW_KEY_W)){
		rootComponent->applyLinearImpulseUp(2.0f);
	}
	if(keyboard->keyDown(GLFW_KEY_S)){
		rootComponent->applyLinearImpulseDown(2.0f);
	}
	if(keyboard->keyDown(GLFW_KEY_A)){
		rootComponent->applyLinearImpulseLeft(2.0f);
	}
	if(keyboard->keyDown(GLFW_KEY_D)){
		rootComponent->applyLinearImpulseRight(2.0f);
	}

	Box2DSuperSprite::update(_step);
}

OJ_Player::~OJ_Player() {
}