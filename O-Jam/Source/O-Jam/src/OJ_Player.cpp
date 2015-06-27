#pragma once

#include <OJ_Player.h>
#include <OJ_ResourceManager.h>
#include <Keyboard.h>
#include <MeshInterface.h>

#include <glfw\glfw3.h>

OJ_TexturePack::OJ_TexturePack(std::string _torsoSrc, std::string _handSrc) :
	torsoTex(OJ_ResourceManager::playthrough->getTexture(_torsoSrc)->texture),
	handTex(OJ_ResourceManager::playthrough->getTexture(_handSrc)->texture)
{
}

OJ_Player::OJ_Player(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	Box2DSuperSprite(_world, _categoryBits, _maskBits, _groupIndex),
	speed(1),
	punchSpeed(2.f),
	punchDelay(10),
	punchReach(3.f)
{
	if(_texPack == nullptr){
		_texPack = new OJ_TexturePack("torso", "hand");
	}

	componentScale = 1.f;

	torso = new Box2DSprite(world, b2_dynamicBody, false, nullptr, _texPack->torsoTex, 1, 1, 0, 0, componentScale);
	handR = new Box2DSprite(world, b2_dynamicBody, false, nullptr, _texPack->handTex, 0.5f, 0.5f, 0, 0, componentScale);
	handL = new Box2DSprite(world, b2_dynamicBody, false, nullptr, _texPack->handTex, 0.5f, 0.5f, 0, 0, componentScale);

	addComponent(&torso);
	addComponent(&handR);
	addComponent(&handL);

	rootComponent = torso;

	rootComponent->maxVelocity = b2Vec2(componentScale*0.5f, componentScale*0.5f);

	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != static_cast<int16>(-1)){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;

	b2Fixture * f = torso->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(10.f);
	f = handR->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(0.01f);
	f = handL->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(0.01f);

	////////////
	// JOINTS //
	////////////
	{
		b2PrismaticJointDef j;
		j.bodyA = torso->body;
		j.bodyB = handR->body;
		j.localAnchorA.Set(0.9f * torso->getCorrectedWidth(), 0);
		j.localAnchorB.Set(0, 0);
		j.localAxisA.Set(-0.2, 0.8);
		j.collideConnected = false;
		j.enableLimit = true;
		j.enableMotor = true;
		j.maxMotorForce = 100;
		j.motorSpeed = -100;
		j.upperTranslation = componentScale;
		j.lowerTranslation = 0;
		leftHandJoint = (b2PrismaticJoint *)world->b2world->CreateJoint(&j);
	}
	{
		b2PrismaticJointDef j;
		j.bodyA = torso->body;
		j.bodyB = handL->body;
		j.localAnchorA.Set(-0.9f * torso->getCorrectedWidth(), 0);
		j.localAnchorB.Set(0, 0);
		j.localAxisA.Set(0.2, 0.8);
		j.enableLimit = true;
		j.enableMotor = true;
		j.maxMotorForce = 100;
		j.motorSpeed = -100;
		j.upperTranslation = componentScale;
		j.lowerTranslation = 0;
		j.collideConnected = false;
		
		rightHandJoint = (b2PrismaticJoint *)world->b2world->CreateJoint(&j);
	}

	torso->mesh->textures.clear();
	handR->mesh->textures.clear();
	handL->mesh->textures.clear();

}

void OJ_Player::update(Step * _step){
	rootComponent->body->SetFixedRotation(true);
	
	rootComponent->body->SetTransform(rootComponent->body->GetWorldCenter(), punchAngle);

	Box2DSuperSprite::update(_step);
}

OJ_Player::~OJ_Player() {
}




void OJ_Player::punchR(){
	rightHandJoint->SetLimits(0, componentScale*punchReach);

	glm::vec2 punchV(glm::cos(punchAngle + glm::half_pi<float>()), glm::sin(punchAngle + glm::half_pi<float>()));
	float s = punchSpeed * handR->body->GetMass();
	handR->applyLinearImpulseToCenter(punchV.x * s, punchV.y * s);
}

void OJ_Player::punchL(){
	leftHandJoint->SetLimits(0, componentScale*punchReach);
		
	glm::vec2 punchV(glm::cos(punchAngle + glm::half_pi<float>()), glm::sin(punchAngle + glm::half_pi<float>()));
	float s = punchSpeed * handL->body->GetMass();
	handL->applyLinearImpulseToCenter(punchV.x * s, punchV.y * s);
}

void OJ_Player::move(glm::vec2 _v){
	if(_v.x != 0 || _v.y != 0){
		float s = speed * rootComponent->body->GetMass();
		rootComponent->applyLinearImpulseUp(_v.y * s);
		rootComponent->applyLinearImpulseRight(_v.x * s);
	}
}