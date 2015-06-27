#pragma once

#include <OJ_Player.h>
#include <OJ_ResourceManager.h>
#include <Keyboard.h>
#include <MeshInterface.h>

#include <glfw\glfw3.h>

OJ_Player::OJ_Player(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	OJ_Boxer(_componentScale, _texPack, _world, _categoryBits, _maskBits, _groupIndex),
	stance(kNONE),
	disabled(false),
	disableTimer(1)
{
	rootComponent->body->SetFixedRotation(true);

	disableTimer.onCompleteFunction = [this](Timeout * _this){
		this->disabled = false;
		this->rootComponent->body->SetFixedRotation(true);
	};
}

void OJ_Player::update(Step * _step){
	disableTimer.update(_step);
	
	if(!disabled){
		rootComponent->body->SetTransform(rootComponent->body->GetWorldCenter(), punchAngle);
	}
	OJ_Boxer::update(_step);
}

OJ_Player::~OJ_Player() {
}

void OJ_Player::move(glm::vec2 _v){
	if(!disabled){
		if(_v.x != 0 || _v.y != 0){
			float s = speed;
			//rootComponent->body->SetLinearVelocity(b2Vec2(_v.x * s, _v.y * s));
			rootComponent->applyLinearImpulseToCenter(_v.x * s, _v.y * s);
		}
	}
}

void OJ_Player::punchR(){
	if(!disabled){
		OJ_Boxer::punchR();
	}
}

void OJ_Player::punchL(){
	if(!disabled){
		OJ_Boxer::punchL();
	}
}

void OJ_Player::getReady(Stance _stance){
	if(!disabled){
		stance = _stance;
	}
}

void OJ_Player::disable(float _seconds){
	disabled = true;
	stance = kNONE;
	rootComponent->body->SetFixedRotation(false);
	disableTimer.targetSeconds = _seconds;
	disableTimer.restart();
}