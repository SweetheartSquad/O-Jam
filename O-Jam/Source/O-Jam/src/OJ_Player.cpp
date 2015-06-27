#pragma once

#include <OJ_Player.h>
#include <OJ_ResourceManager.h>
#include <Keyboard.h>
#include <MeshInterface.h>

#include <glfw\glfw3.h>

OJ_Player::OJ_Player(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	OJ_Boxer(_componentScale, _texPack, _world, _categoryBits, _maskBits, _groupIndex)
{
	rootComponent->body->SetType(b2_kinematicBody);
}

void OJ_Player::update(Step * _step){
	rootComponent->body->SetFixedRotation(true);
	
	rootComponent->body->SetTransform(rootComponent->body->GetWorldCenter(), punchAngle);

	OJ_Boxer::update(_step);
}

OJ_Player::~OJ_Player() {
}

void OJ_Player::move(glm::vec2 _v){
	if(_v.x != 0 || _v.y != 0){
		float s = speed;
		rootComponent->body->SetLinearVelocity(b2Vec2(_v.x * s, _v.y * s));
	}
}