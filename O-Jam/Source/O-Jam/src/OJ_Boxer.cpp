#pragma once

#include <OJ_Boxer.h>
#include <Box2DSprite.h>

OJ_Boxer::OJ_Boxer(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits, int16 _groupIndex) :
	OJ_Character(_texPack == nullptr ? new OJ_TexturePack("torso", "hand") : _texPack, _world, _categoryBits, _maskBits, _groupIndex),
	ticksSincePunchL(0),
	ticksSincePunchR(0),
	punchedL(false),
	punchedR(false),
	punchDir(0),
	punchSpeed(2.f),
	punchDelay(5)
{
	handR = new Box2DSprite(world, b2_dynamicBody, false, nullptr, texPack->handTex, 0.5f, 0.5f, 0, 0, componentScale);
	handL = new Box2DSprite(world, b2_dynamicBody, false, nullptr, texPack->handTex, 0.5f, 0.5f, 0, 0, componentScale);
	addComponent(&handR);
	addComponent(&handL);

		b2Fixture * f = handR->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(0.01f);
	f = handL->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(0.01f);
	
	////////////
	// JOINTS //
	////////////
	{
		b2DistanceJointDef j;
		j.bodyA = torso->body;
		j.bodyB = handR->body;
		j.localAnchorA.Set(0.9f * torso->getCorrectedWidth(), 0);
		j.localAnchorB.Set(0, 0);
		j.collideConnected = false;
		j.length = componentScale;
		leftHandJoint = (b2DistanceJoint *)world->b2world->CreateJoint(&j);
	}
	{
		b2DistanceJointDef j;
		j.bodyA = torso->body;
		j.bodyB = handL->body;
		j.localAnchorA.Set(-0.9f * torso->getCorrectedWidth(), 0);
		j.localAnchorB.Set(0, 0);
		j.collideConnected = false;
		j.length = componentScale;
		rightHandJoint = (b2DistanceJoint *)world->b2world->CreateJoint(&j);
	}

}

OJ_Boxer::~OJ_Boxer() {

}

void OJ_Boxer::update(Step * _step) {
	if(punchedR) {
		ticksSincePunchR++;
	}
	if(punchedL) {
		ticksSincePunchL++;
	}
	
	if(ticksSincePunchR > punchDelay){
		rightHandJoint->SetLength(componentScale*0.3f);
		ticksSincePunchR = 0;
		punchedR = false;
	}
	if(ticksSincePunchL > punchDelay){
		leftHandJoint->SetLength(componentScale*0.3f);
		ticksSincePunchL = 0;
		punchedL = false;
	}
	OJ_Character::update(_step);
}

void OJ_Boxer::punchR(){
	if(!punchedR){
		punchedR = true;

		rightHandJoint->SetLength(componentScale*3.f);

		handR->applyLinearImpulseToCenter(punchDir.x * punchSpeed * handR->body->GetMass(), punchDir.y * punchSpeed * handR->body->GetMass());
	}
}

void OJ_Boxer::punchL(){
	if(!punchedL){
		punchedL = true;

		leftHandJoint->SetLength(componentScale*3.f);

		handL->applyLinearImpulseToCenter(punchDir.x * punchSpeed * handL->body->GetMass(), punchDir.y * punchSpeed * handL->body->GetMass());
	}
}