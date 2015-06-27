#pragma once

#include <OJ_Player.h>
#include <OJ_ResourceManager.h>
#include <Keyboard.h>
#include <MeshInterface.h>

#include <glfw\glfw3.h>

OJ_Player::OJ_Player(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	OJ_Boxer(_texPack, _world, _categoryBits, _maskBits, _groupIndex)
{
	torso->mesh->textures.clear();
	handR->mesh->textures.clear();
	handL->mesh->textures.clear();
}

void OJ_Player::update(Step * _step){
	rootComponent->body->SetFixedRotation(true);
	
	float a = glm::atan(punchDir.y, punchDir.x);
	rootComponent->body->SetTransform(rootComponent->body->GetWorldCenter(), a);

	OJ_Boxer::update(_step);
}

OJ_Player::~OJ_Player() {
}


void OJ_Player::move(glm::vec2 _v){
	if(_v.x != 0 || _v.y != 0){
		float s = speed * rootComponent->body->GetMass();
		rootComponent->applyLinearImpulseUp(_v.y * s);
		rootComponent->applyLinearImpulseRight(_v.x * s);
	}
}