#pragma once

#include <OJ_Player.h>
#include <OJ_ResourceManager.h>

OJ_TexturePack::OJ_TexturePack(std::string _torsoSrc, std::string _handSrc) :
	torsoTex(OJ_ResourceManager::playthrough->getTexture(_torsoSrc)->texture),
	handTex(OJ_ResourceManager::playthrough->getTexture(_handSrc)->texture)
{
}

OJ_Player::OJ_Player(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex)
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

	rootComponent = torso;



	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != static_cast<int16>(-1)){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;

	torso->createFixture(sf, b2Vec2(0.f, 0.f), this);
	handR->createFixture(sf, b2Vec2(0.f, 0.f), this);
	handL->createFixture(sf, b2Vec2(0.f, 0.f), this);
	
	handL->childTransform->scale(-1, 1, 1);

	////////////
	// JOINTS //
	////////////
	{
	b2RevoluteJointDef j;
	j.bodyA = torso->body;
	j.bodyB = handR->body;
	j.localAnchorA.Set(0, 0);
	j.localAnchorB.Set(0, 0);
	j.collideConnected = false;
	j.enableLimit = true;
	j.referenceAngle = 0;
	world->b2world->CreateJoint(&j);
	}
	{
	b2RevoluteJointDef j;
	j.bodyA = torso->body;
	j.bodyB = handL->body;
	j.localAnchorA.Set(0, 0);
	j.localAnchorB.Set(0, 0);
	j.collideConnected = false;
	j.enableLimit = true;
	j.referenceAngle = 0;
	world->b2world->CreateJoint(&j);
	}
}

OJ_Player::~OJ_Player() {
}