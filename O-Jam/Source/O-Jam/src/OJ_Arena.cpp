#pragma once

#include <OJ_Arena.h>
#include <Box2DWorld.h>
#include <NumberUtils.h>
#include <Box2D/Box2D.h>
#include <Box2DSprite.h>
#include <OJ_Character.h>
#include <OJ_Scene.h>
#include <OJ_Enemy.h>
#include <OJ_Game.h>
#include <OJ_Bullet.h>
#include <Timeout.h>

#include <OJ_DdosEnemy.h>
#include <OJ_BotEnemy.h>

const float PI = 3.1415926;

OJ_Arena::OJ_Arena(OJ_Scene * _scene, Box2DWorld * _world, Shader * _shader, float _radius, int _points) :
	Entity(),
	spawnTimer(1.0),
	easyEnemiesLeft(0),
	world(_world),
	waveNumber(1),
	scene(_scene),
	shader(_shader),
	radius(_radius),
	hardEnemiesPerRound(1),
	hardEnemiesLeft(0)
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

		b2Filter filter;

		filter.categoryBits = OJ_Game::kBOUNDARY;
		sprite->createFixture(filter);
		sprite->setShader(_shader, true);
		childTransform->addChild(sprite);
		sprite->parents.at(0)->scale(randScale, randScale, 1.0f);
		float lim = 0.75f * _radius;
		float x = vox::NumberUtils::randomFloat(-lim, lim);
		float y = vox::NumberUtils::randomFloat(-lim, lim);
		sprite->setTranslationPhysical(x, y, 0.f);
	}

	spawnTimer.onCompleteFunction = [this](Timeout * _this){
		if(easyEnemiesLeft + hardEnemiesLeft > 0) {
			spawnEnemy();
			easyEnemiesLeft--;
			spawnTimer.restart();
		}
	};
}

void OJ_Arena::update(Step* _step) {
	// destroy dead enemies
	for(signed long int i = enemies.size()-1; i >= 0; --i){
		OJ_Enemy * enemy = enemies.at(i);
		if (enemy->dead){
			killEnemy(enemy);
		}
	}
	// destroy dead bullets
	for(signed long int i = bullets.size()-1; i >= 0; --i){
		OJ_Bullet * b = bullets.at(i);
		if (b->destroyed){
			removeBullet(b);
		}
	}

	if(enemies.size() == 0 && easyEnemiesLeft == 0) {
		spawnNextWave();
	}

	spawnTimer.update(_step);

	Entity::update(_step);
}

void OJ_Arena::killEnemy(OJ_Enemy * _enemy){
	for(signed long int i = enemies.size()-1; i >= 0; --i){
		if(enemies.at(i) == _enemy){
			enemies.erase(enemies.begin() + i);
			break;
		}
	}

	childTransform->removeChild(_enemy->parents.at(0));
	delete _enemy->parents.at(0); // memory leak here
}

void OJ_Arena::spawnNextWave() {
	waveNumber++;
	easyEnemiesLeft = (int)waveNumber * 10 * 1.2f;
	if(waveNumber % 4 == 0) {
		hardEnemiesPerRound++;
	}
	hardEnemiesLeft = hardEnemiesPerRound;
	spawnTimer.restart();
}

void OJ_Arena::spawnEnemy() {
	OJ_Enemy * e;
	int isEnemyHard = vox::NumberUtils::randomInt(0, easyEnemiesLeft);
	if(easyEnemiesLeft > 0 && isEnemyHard < easyEnemiesLeft / 2) {
		e = getEasyEnemy();
		easyEnemiesLeft--;
	}else if(hardEnemiesLeft > 0){
		e = getHardEnemy();
		hardEnemiesLeft--;
	}else {
		e = getEasyEnemy();
		easyEnemiesLeft--;
	}
	enemies.push_back(e);
	e->setShader(shader, true);
	childTransform->addChild(e);
	e->rootComponent->maxVelocity = b2Vec2(10.0f, 10.0f);
	e->speed = 10.0f;

	float ang = PI * 2.0 / 100.f * vox::NumberUtils::randomInt(1, 99);
	b2Vec2 pos(cos(ang) * radius * 1.2f, sin(ang) * radius * 1.2f);

	e->translateComponents(pos.x, pos.y, 0.f);
	// This doesn't belong here
	e->targetCharacter(scene->playerOne);
}

OJ_Arena::~OJ_Arena() {
}

OJ_Bullet * OJ_Arena::getBullet(Texture * _tex, float _size){
	OJ_Bullet * b = new OJ_Bullet(200, world, b2_dynamicBody, false, nullptr, _tex, 1, 1, 0, 0, _size);
	b->setShader(shader, true);
	childTransform->addChild(b);
	bullets.push_back(b);

	b2Filter sf;
	sf.categoryBits = OJ_Game::BOX2D_CATEGORY::kBULLET;
	sf.maskBits = OJ_Game::BOX2D_CATEGORY::kENEMY;
	sf.groupIndex = 0;
	b->createFixture(sf, b2Vec2(0, 0), b, false);
	return b;
}

OJ_Enemy * OJ_Arena::getEasyEnemy() {
	return new OJ_DdosEnemy(world);
}

OJ_Enemy* OJ_Arena::getHardEnemy() {
	return new OJ_BotEnemy(world);
}

void OJ_Arena::removeBullet(OJ_Bullet * _bullet){
	for(signed long int i = bullets.size()-1; i >= 0; --i){
		if(bullets.at(i) == _bullet){
			bullets.erase(bullets.begin() + i);
			break;
		}
	}
	childTransform->removeChild(_bullet->parents.at(0));
	delete _bullet->parents.at(0); // memory leak here
}