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
	ticksSincePunchL(0),
	ticksSincePunchR(0),
	punchedL(false),
	punchedR(false),
	speed(1),
	punchDir(0),
	punchSpeed(2.f),
	punchDelay(5)
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

	/*torso->parents.at(0)->scale(10, 10, 10);
	handR->parents.at(0)->scale(5, 5, 5);
	handL->parents.at(0)->scale(-5, 5, 5);*/

	rootComponent = torso;

	rootComponent->maxVelocity = b2Vec2(componentScale*0.5f, componentScale*0.5f);

	b2Filter sf;
	sf.categoryBits = categoryBits;
	if(maskBits != static_cast<int16>(-1)){
		sf.maskBits = maskBits;
	}
	sf.groupIndex = groupIndex;

	torso->createFixture(sf, b2Vec2(0.f, 0.f), this);
	b2Fixture * f = handR->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(0.01f);
	f = handL->createFixture(sf, b2Vec2(0.f, 0.f), this);
	f->SetDensity(0.01f);
	
	//handR->setTranslationPhysical(3.0f, 0.f, 0.f);
	//handL->setTranslationPhysical(-3.0f, 0.f, 0.f);

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

	torso->mesh->textures.clear();
	handR->mesh->textures.clear();
	handL->mesh->textures.clear();

}

void OJ_Player::update(Step * _step){
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

	rootComponent->body->SetFixedRotation(true);
	
	float a = glm::atan(punchDir.y, punchDir.x);
	rootComponent->body->SetTransform(rootComponent->body->GetWorldCenter(), a);

	Box2DSuperSprite::update(_step);
}

OJ_Player::~OJ_Player() {
}




void OJ_Player::punchR(){
	if(!punchedR){
		punchedR = true;

		rightHandJoint->SetLength(componentScale*3.f);

		handR->applyLinearImpulseToCenter(punchDir.x * punchSpeed * handR->body->GetMass(), punchDir.y * punchSpeed * handR->body->GetMass());
	}
}

void OJ_Player::punchL(){
	if(!punchedL){
		punchedL = true;

		leftHandJoint->SetLength(componentScale*3.f);

		handL->applyLinearImpulseToCenter(punchDir.x * punchSpeed * handL->body->GetMass(), punchDir.y * punchSpeed * handL->body->GetMass());
	}
}

void OJ_Player::move(glm::vec2 _v){
	if(_v.x != 0 || _v.y != 0){
		float s = speed * rootComponent->body->GetMass();
		rootComponent->applyLinearImpulseUp(_v.y * s);
		rootComponent->applyLinearImpulseRight(_v.x * s);
	}
}