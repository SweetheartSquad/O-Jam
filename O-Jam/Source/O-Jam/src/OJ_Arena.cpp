#pragma once

#include <OJ_Arena.h>
#include <Box2DWorld.h>
#include <NumberUtils.h>
#include <Box2D/Box2D.h>
#include <Box2DSprite.h>
#include <OJ_Character.h>

const float PI = 3.1415926;

OJ_Arena::OJ_Arena(Box2DWorld * _world, Shader * _shader, float _radius, int _points) :
	Entity(),
	world(_world)
{

	b2Vec2 * vs = (b2Vec2 *)malloc(sizeof(b2Vec2) * _points);
	for(unsigned long int i = 0; i < _points; ++i) {
		float ang = PI * 2.0 / _points * i;
		b2Vec2 pos(cos(ang) * _radius, sin(ang) * _radius);
		vs[i] = pos;
	}

	b2ChainShape chain;
	chain.CreateLoop(vs, _points);

	b2FixtureDef fd;
	fd.shape = &chain;

	b2BodyDef bodyDef;
	bodyDef.position.Set(0, 0);
	bodyDef.type = b2_staticBody;
	
	b2Body * chainBody = world->b2world->CreateBody(&bodyDef);
	b2Fixture * f = chainBody->CreateFixture(&fd);

	free(vs);

	int numObs = vox::NumberUtils::randomInt(10, 30);

	OJ_TexturePack texPack("TORSO", "HAND");

	for(unsigned long int i = 0; i < numObs; ++i) {
		
		float randScale = vox::NumberUtils::randomFloat(2.0f, 5.0f);

		Box2DSprite * sprite = new Box2DSprite(world, b2_staticBody, false, nullptr, texPack.torsoTex);

		sprite->createFixture(b2Filter());

		sprite->setShader(_shader, true);
		
		childTransform->addChild(sprite);

		sprite->parents.at(0)->scale(randScale, randScale, 1.0f);
		
		float lim = 0.75f * _radius;

		float x = vox::NumberUtils::randomFloat(-lim, lim);
		
		float y = vox::NumberUtils::randomFloat(-lim, lim);

		sprite->setTranslationPhysical(x, y, 0.f);
	}
}

OJ_Arena::~OJ_Arena() {
}