#pragma once

#include <OJ_Boxer.h>
#include <Box2DSprite.h>

OJ_Boxer::OJ_Boxer(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	OJ_Character(50.f, _componentScale, _texPack == nullptr ? new OJ_TexturePack("TORSO", "HAND") : _texPack, _world, _categoryBits, _maskBits, _groupIndex),
	punchSpeed(2.f),
	punchReach(3.f)
{
	handR = new Box2DSprite(world, b2_dynamicBody, false, nullptr, texPack->handTex, 0.5f, 0.5f, 0, 0, componentScale);
	handL = new Box2DSprite(world, b2_dynamicBody, false, nullptr, texPack->handTex, 0.5f, 0.5f, 0, 0, componentScale);
	addComponent(&handR);
	addComponent(&handL);

	b2Fixture * f = handR->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(50.f);
	f = handL->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(50.f);
	
	////////////
	// JOINTS //
	////////////
	{
		b2PrismaticJointDef j;
		j.bodyA = torso->body;
		j.bodyB = handR->body;
		j.localAnchorA.Set(0.3f * torso->getCorrectedWidth(), 0.7f * torso->getCorrectedHeight());
		j.localAnchorB.Set(0, 0);
		j.localAxisA.Set(0.0, 1);
		j.collideConnected = false;
		j.enableLimit = true;
		j.enableMotor = true;
		j.maxMotorForce = 100 * componentScale;
		j.motorSpeed = -100 * componentScale;
		j.upperTranslation = componentScale;
		j.lowerTranslation = 0;
		handJointL = (b2PrismaticJoint *)world->b2world->CreateJoint(&j);
	}
	{
		b2PrismaticJointDef j;
		j.bodyA = torso->body;
		j.bodyB = handL->body;
		j.localAnchorA.Set(-0.3f * torso->getCorrectedWidth(), 0.7f * torso->getCorrectedHeight());
		j.localAnchorB.Set(0, 0);
		j.localAxisA.Set(0.0, 1);
		j.enableLimit = true;
		j.enableMotor = true;
		j.maxMotorForce = 100 * componentScale;
		j.motorSpeed = -100 * componentScale;
		j.upperTranslation = componentScale;
		j.lowerTranslation = 0;
		j.collideConnected = false;
		
		handJointR = (b2PrismaticJoint *)world->b2world->CreateJoint(&j);
	}
}

OJ_Boxer::~OJ_Boxer() {

}

void OJ_Boxer::update(Step * _step) {
	OJ_Character::update(_step);
}

void OJ_Boxer::punchR(){
	handJointR->SetLimits(0, punchReach);

	glm::vec2 punchV(glm::cos(punchAngle + glm::half_pi<float>()), glm::sin(punchAngle + glm::half_pi<float>()));
	float s = punchSpeed * handR->body->GetMass();
	handR->applyLinearImpulseToCenter(punchV.x * s, punchV.y * s);
}

void OJ_Boxer::punchL(){
	handJointL->SetLimits(0, punchReach);
		
	glm::vec2 punchV(glm::cos(punchAngle + glm::half_pi<float>()), glm::sin(punchAngle + glm::half_pi<float>()));
	float s = punchSpeed * handL->body->GetMass();
	handL->applyLinearImpulseToCenter(punchV.x * s, punchV.y * s);
}