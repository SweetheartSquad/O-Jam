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
#include <MeshInterface.h>

#include <OJ_DdosEnemy.h>
#include <OJ_TrojanEnemy.h>
#include <OJ_BotEnemy.h>
#include <OJ_ResourceManager.h>
#include <ParticleSystem.h>
#include <Particle.h>
#include <NumberUtils.h>

const float PI = 3.1415926;

#define ARENA_TILE 10

OJ_Arena::OJ_Arena(OJ_Scene * _scene, Box2DWorld * _world, Shader * _shader, float _radius, int _points) :
	Entity(),
	spawnTimer(0.5f),
	easyEnemiesLeft(0),
	world(_world),
	waveNumber(0),
	scene(_scene),
	shader(_shader),
	radius(_radius),
	hardEnemiesPerRound(0),
	hardEnemiesLeft(0),
	botsLeft(0),
	trojansLeft(0),
	botsPerWave(1),
	trojansPreWave(1),
	particles(new ParticleSystem(new TextureSampler(OJ_ResourceManager::playthrough->getTexture("TORSO")->texture, 1, 1), _world, -1, -1, 0)),
	startIndicatorTimer(1.5f),
	componentMultMutlt(0),
	score(0)
{
	particles->setShader(shader, true);
	b2Vec2 * vs = (b2Vec2 *)malloc(sizeof(b2Vec2) * _points);
	for(unsigned long int i = 0; i < _points; ++i) {
		float ang = PI * 2.0 / _points * i;
		b2Vec2 pos(cos(ang) * _radius * ARENA_TILE, sin(ang) * _radius * ARENA_TILE);
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
	
	// hex tile grid
	for(unsigned int r = 0; r < _radius; ++r){
		// rings

		glm::vec3 pos = glm::vec3(0.f, ARENA_TILE * r, 0.f);
		// align next tile angle
		glm::vec3 unit = glm::vec3(cos(glm::radians(210.f)) * ARENA_TILE, sin(glm::radians(210.f)) * ARENA_TILE, 0.f);

		for(unsigned int t = 0; t < r * 6; ++t){

			// decide if obstacle is here
			if(vox::NumberUtils::randomInt(1, 6) == 1){
				// place obstacle
				Box2DSprite * sprite = getHexTile();
				sprite->setShader(shader, true);
				childTransform->addChild(sprite);
				sprite->setTranslationPhysical(glm::vec3(pos.x, pos.y, 0.f)); 
				
			}
			pos = pos + unit;

			if((t+1) % (r) == 0){
				// change direction vector & pos.x
				glm::vec4 unitRotated = glm::rotate(60.f, glm::vec3(0,0,1)) * glm::vec4(unit.x, unit.y, unit.z, 1);
				unit = glm::vec3(unitRotated.x, unitRotated.y, unitRotated.z);
			}
		}
	}
	
	spawnTimer.onCompleteFunction = [this](Timeout * _this){
		if(easyEnemiesLeft + hardEnemiesLeft > 0) {
			spawnEnemy();
			spawnTimer.restart();
		}
	};

	childTransform->addChild(particles, false);
}

Box2DSprite * OJ_Arena::getHexTile(){
	b2Vec2 * vs = (b2Vec2 *)malloc(sizeof(b2Vec2) * 6);
	for(unsigned long int i = 0; i < 6; ++i) {
		float ang = PI * 2.0 / 6 * i;
		b2Vec2 pos(cos(ang) * ARENA_TILE * 0.5, sin(ang) * ARENA_TILE * 0.5);
		vs[i] = pos;
	}

	b2ChainShape  * chain = new b2ChainShape();
	chain->CreateLoop(vs, 6);

	Box2DSprite * sprite = new Box2DSprite(world, b2_staticBody, false, nullptr, OJ_ResourceManager::playthrough->getTexture("test2")->texture);

	// Fixture
	b2Filter filter;

	filter.categoryBits = OJ_Game::kBOUNDARY;

	b2FixtureDef fd;
	fd.shape = chain;
	fd.restitution = 0.f;
	fd.friction = 0.5f;
	fd.density = 1.f;
	fd.filter = filter;

	// Body
	b2BodyDef bodyDef;
	bodyDef.position.Set(0, 0);
	bodyDef.type = b2_staticBody;
	
	sprite->body = world->b2world->CreateBody(&bodyDef);
	b2Fixture * f = sprite->body->CreateFixture(&fd);

	float height = vox::NumberUtils::randomFloat(5.0f, 10.0f);

	MeshInterface * m = new QuadMesh();
	// Top face - top-half
	m->pushVert(Vertex(vs[0].x, vs[0].y, height));
	m->pushVert(Vertex(vs[1].x, vs[1].y, height));
	m->pushVert(Vertex(vs[2].x, vs[2].y, height));
	m->pushVert(Vertex(vs[3].x, vs[3].y, height));
	m->setNormal(0, 0.0, 0.0, 1.0);
	m->setNormal(1, 0.0, 0.0, 1.0);
	m->setNormal(2, 0.0, 0.0, 1.0);
	m->setNormal(3, 0.0, 0.0, 1.0);
	m->setUV(0, 1.0, 0.5);
	m->setUV(1, 1 - (0.5 / tan(glm::radians(60.f))), 1.0);
	m->setUV(2, 0.5 / tan(glm::radians(60.f)), 1.0);
	m->setUV(3, 0.0, 0.5);

	// Top face - bottom-half
	m->pushVert(Vertex(vs[3].x, vs[3].y, height));
	m->pushVert(Vertex(vs[4].x, vs[4].y, height));
	m->pushVert(Vertex(vs[5].x, vs[5].y, height));
	m->pushVert(Vertex(vs[0].x, vs[0].y, height));
	m->setNormal(4, 0.0, 0.0, 1.0);
	m->setNormal(5, 0.0, 0.0, 1.0);
	m->setNormal(6, 0.0, 0.0, 1.0);
	m->setNormal(7, 0.0, 0.0, 1.0);
	m->setUV(4, 0.0, 0.5);
	m->setUV(5, 0.5 / tan(glm::radians(60.f)), 0.0);
	m->setUV(6, 1 - (0.5 / tan(glm::radians(60.f))), 0.0);
	m->setUV(7, 1.0, 0.5);
	
	// Bottom face - top-half
	m->pushVert(Vertex(vs[0].x, vs[0].y, 0.f));
	m->pushVert(Vertex(vs[1].x, vs[1].y, 0.f));
	m->pushVert(Vertex(vs[2].x, vs[2].y, 0.f));
	m->pushVert(Vertex(vs[3].x, vs[3].y, 0.f));
	m->setNormal(8, 0.0, 0.0, -1.0);
	m->setNormal(9, 0.0, 0.0, -1.0);
	m->setNormal(10, 0.0, 0.0, -1.0);
	m->setNormal(11, 0.0, 0.0, -1.0);
	m->setUV(8, 1.0, 0.5);
	m->setUV(9, 1 - (0.5 / tan(glm::radians(60.f))), 1.0);
	m->setUV(10, 0.5 / tan(glm::radians(60.f)), 1.0);
	m->setUV(11, 0.0, 0.5);

	// Bottom face - bottom-half
	m->pushVert(Vertex(vs[3].x, vs[3].y, 0.f));
	m->pushVert(Vertex(vs[4].x, vs[4].y, 0.f));
	m->pushVert(Vertex(vs[5].x, vs[5].y, 0.f));
	m->pushVert(Vertex(vs[0].x, vs[0].y, 0.f));
	m->setNormal(12, 0.0, 0.0, -1.0);
	m->setNormal(13, 0.0, 0.0, -1.0);
	m->setNormal(14, 0.0, 0.0, -1.0);
	m->setNormal(15, 0.0, 0.0, -1.0);
	m->setUV(12, 0.0, 0.5);
	m->setUV(13, 0.5 / tan(glm::radians(60.f)), 0.0);
	m->setUV(14, 1 - (0.5 / tan(glm::radians(60.f))), 0.0);
	m->setUV(15, 1.0, 0.5);

	// Side 1
	m->pushVert(Vertex(vs[0].x, vs[0].y, height));
	m->pushVert(Vertex(vs[1].x, vs[1].y, height));
	m->pushVert(Vertex(vs[1].x, vs[1].y, 0.f));
	m->pushVert(Vertex(vs[0].x, vs[0].y, 0.f));
	m->setNormal(16, cos(glm::radians(30.f)), sin(glm::radians(30.f)), 0.0);
	m->setNormal(17, cos(glm::radians(30.f)), sin(glm::radians(30.f)), 0.0);
	m->setNormal(18, cos(glm::radians(30.f)), sin(glm::radians(30.f)), 0.0);
	m->setNormal(19, cos(glm::radians(30.f)), sin(glm::radians(30.f)), 0.0);
	m->setUV(16, 0.0, 1.0);
	m->setUV(17, 1.0, 1.0);
	m->setUV(18, 1.0, 0.0);
	m->setUV(19, 0.0, 0.0);

	// Side 2
	m->pushVert(Vertex(vs[1].x, vs[1].y, height));
	m->pushVert(Vertex(vs[2].x, vs[2].y, height));
	m->pushVert(Vertex(vs[2].x, vs[2].y, 0.f));
	m->pushVert(Vertex(vs[1].x, vs[1].y, 0.f));
	m->setNormal(20, 0.0, 1.0, 0.0);
	m->setNormal(21, 0.0, 1.0, 0.0);
	m->setNormal(22, 0.0, 1.0, 0.0);
	m->setNormal(23, 0.0, 1.0, 0.0);
	m->setUV(20, 0.0, 1.0);
	m->setUV(21, 1.0, 1.0);
	m->setUV(22, 1.0, 0.0);
	m->setUV(23, 0.0, 0.0);

	// Side 3
	m->pushVert(Vertex(vs[2].x, vs[2].y, height));
	m->pushVert(Vertex(vs[3].x, vs[3].y, height));
	m->pushVert(Vertex(vs[3].x, vs[3].y, 0.f));
	m->pushVert(Vertex(vs[2].x, vs[2].y, 0.f));
	m->setNormal(24, cos(glm::radians(150.f)), sin(glm::radians(150.f)), 0.0);
	m->setNormal(25, cos(glm::radians(150.f)), sin(glm::radians(150.f)), 0.0);
	m->setNormal(26, cos(glm::radians(150.f)), sin(glm::radians(150.f)), 0.0);
	m->setNormal(27, cos(glm::radians(150.f)), sin(glm::radians(150.f)), 0.0);
	m->setUV(24, 0.0, 1.0);
	m->setUV(25, 1.0, 1.0);
	m->setUV(26, 1.0, 0.0);
	m->setUV(27, 0.0, 0.0);

	// Side 4
	m->pushVert(Vertex(vs[3].x, vs[3].y, height));
	m->pushVert(Vertex(vs[4].x, vs[4].y, height));
	m->pushVert(Vertex(vs[4].x, vs[4].y, 0.f));
	m->pushVert(Vertex(vs[3].x, vs[3].y, 0.f));
	m->setNormal(28, cos(glm::radians(210.f)), sin(glm::radians(210.f)), 0.0);
	m->setNormal(29, cos(glm::radians(210.f)), sin(glm::radians(210.f)), 0.0);
	m->setNormal(30, cos(glm::radians(210.f)), sin(glm::radians(210.f)), 0.0);
	m->setNormal(31, cos(glm::radians(210.f)), sin(glm::radians(210.f)), 0.0);
	m->setUV(28, 0.0, 1.0);
	m->setUV(29, 1.0, 1.0);
	m->setUV(30, 1.0, 0.0);
	m->setUV(31, 0.0, 0.0);

	// Side 5
	m->pushVert(Vertex(vs[4].x, vs[4].y, height));
	m->pushVert(Vertex(vs[5].x, vs[5].y, height));
	m->pushVert(Vertex(vs[5].x, vs[5].y, 0.f));
	m->pushVert(Vertex(vs[4].x, vs[4].y, 0.f));
	m->setNormal(32, 0.0, -1.0, 0.0);
	m->setNormal(33, 0.0, -1.0, 0.0);
	m->setNormal(34, 0.0, -1.0, 0.0);
	m->setNormal(35, 0.0, -1.0, 0.0);
	m->setUV(32, 0.0, 1.0);
	m->setUV(33, 1.0, 1.0);
	m->setUV(34, 1.0, 0.0);
	m->setUV(35, 0.0, 0.0);

	// Side 6
	m->pushVert(Vertex(vs[5].x, vs[5].y, height));
	m->pushVert(Vertex(vs[0].x, vs[0].y, height));
	m->pushVert(Vertex(vs[0].x, vs[0].y, 0.f));
	m->pushVert(Vertex(vs[5].x, vs[5].y, 0.f));
	m->setNormal(36, cos(glm::radians(330.f)), sin(glm::radians(330.f)), 0.0);
	m->setNormal(37, cos(glm::radians(330.f)), sin(glm::radians(330.f)), 0.0);
	m->setNormal(38, cos(glm::radians(330.f)), sin(glm::radians(330.f)), 0.0);
	m->setNormal(39, cos(glm::radians(330.f)), sin(glm::radians(330.f)), 0.0);
	m->setUV(36, 0.0, 1.0);
	m->setUV(37, 1.0, 1.0);
	m->setUV(38, 1.0, 0.0);
	m->setUV(39, 0.0, 0.0);
	
	sprite->childTransform->removeChild(sprite->mesh);
	delete sprite->mesh;
	sprite->mesh = m;
	sprite->childTransform->addChild(sprite->mesh, false);
	sprite->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("test2")->texture);

	free(vs);
	
	return sprite;
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
	startIndicatorTimer.update(_step);

	Entity::update(_step);
}

void OJ_Arena::killEnemy(OJ_Enemy * _enemy){
	for(signed long int i = enemies.size()-1; i >= 0; --i){
		if(enemies.at(i) == _enemy){
			enemies.erase(enemies.begin() + i);
			break;
		}
	}
	glm::vec3 pos = _enemy->rootComponent->getWorldPos();
	for(unsigned long int i = 0; i < 10; ++i){
		Particle * p = particles->addParticle(pos);
		p->startSize += vox::NumberUtils::randomFloat(0, 1);
		p->deltaSize = -p->startSize;
	}

	score += _enemy->scoreVal;

	childTransform->removeChild(_enemy->parents.at(0));
	delete _enemy->parents.at(0); // memory leak here
}

void OJ_Arena::spawnNextWave() {
	startIndicatorTimer.restart();
	waveNumber++;
	componentMultMutlt += 0.1f * waveNumber; 
	easyEnemiesLeft = (int)waveNumber * 6;
	if(waveNumber % 3 == 0) {
		botsPerWave++;
	}
	if(waveNumber % 5 == 0) {
		trojansPreWave++;
	}

	trojansLeft = trojansPreWave;
	botsLeft = botsPerWave;

	hardEnemiesLeft = trojansPreWave + botsPerWave;

	spawnTimer.restart();
	scene->showWave(waveNumber);
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

	float ang = PI * 2.0 / 100.f * vox::NumberUtils::randomInt(1, 99);
	b2Vec2 pos(cos(ang) * radius * 1.2f, sin(ang) * radius * 1.2f);

	spawnEnemyAt(e, pos);
}

void OJ_Arena::spawnEnemyAt(OJ_Enemy * e, b2Vec2 _pos) {
	enemies.push_back(e);
	e->setShader(shader, true);
	childTransform->addChild(e);
	e->rootComponent->maxVelocity = b2Vec2(10.0f, 10.0f);
	e->speed = 10.0f;
	
	e->translateComponents(_pos.x, _pos.y, 0.f);
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
	float compMult = vox::NumberUtils::randomFloat(1.0f, 2.f + componentMultMutlt);
	return new OJ_DdosEnemy(world, compMult);
}

OJ_Enemy* OJ_Arena::getHardEnemy() {
	if(trojansLeft == 0) {
		trojansLeft--;
		float compMult = vox::NumberUtils::randomFloat(1.0f, 2.f + componentMultMutlt);
		return new OJ_BotEnemy(world, compMult);
	}
	if(botsLeft == 0) {	
		botsLeft--;
		float compMult = vox::NumberUtils::randomFloat(1.0f, 1.5f + componentMultMutlt);
		return new OJ_TrojanEnemy(world, this, compMult);
	}
	
	int i = vox::NumberUtils::randomInt(0, 10);

	if(i >= 5) {
		botsLeft--;
		float compMult = vox::NumberUtils::randomFloat(1.0f, 2.f + componentMultMutlt);
		return new OJ_BotEnemy(world, compMult);
	}else {
		trojansLeft--;
		float compMult = vox::NumberUtils::randomFloat(1.0f, 1.5f + componentMultMutlt);
		return new OJ_TrojanEnemy(world, this, compMult);
	}
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