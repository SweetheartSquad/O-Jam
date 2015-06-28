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

const float PI = 3.1415926;

#define ARENA_TILE 10

OJ_Arena::OJ_Arena(OJ_Scene * _scene, Box2DWorld * _world, Shader * _shader, float _radius, int _points) :
	Entity(),
	world(_world),
	waveNumber(1),
	scene(_scene),
	shader(_shader),
	radius(_radius),
	spawnTimer(1.0),
	enemiesLeftInWave(0)
{
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
		if(enemiesLeftInWave > 0) {
			spawnEnemy();
			enemiesLeftInWave--;
			spawnTimer.restart();
		}
	};
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
	
	OJ_TexturePack texPack("TORSO", "HAND");

	Box2DSprite * sprite = new Box2DSprite(world, b2_staticBody, false, nullptr, texPack.torsoTex);

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

	MeshInterface * m = new QuadMesh();
	//Top-half
	m->pushVert(Vertex(vs[0].x, vs[0].y, 0.f));
	m->pushVert(Vertex(vs[1].x, vs[1].y, 0.f));
	m->pushVert(Vertex(vs[2].x, vs[2].y, 0.f));
	m->pushVert(Vertex(vs[3].x, vs[3].y, 0.f));
	m->setNormal(0, 0.0, 0.0, 1.0);
	m->setNormal(1, 0.0, 0.0, 1.0);
	m->setNormal(2, 0.0, 0.0, 1.0);
	m->setNormal(3, 0.0, 0.0, 1.0);
	m->setUV(0, 1.0, 0.5);
	m->setUV(1, 0.5 / tan(glm::radians(30.f)), 1.0);
	m->setUV(2, 1 - (0.5 / tan(glm::radians(30.f))), 1.0);
	m->setUV(3, 0.0, 0.5);

	// Bottom-half
	m->pushVert(Vertex(vs[3].x, vs[3].y, 0.f));
	m->pushVert(Vertex(vs[4].x, vs[4].y, 0.f));
	m->pushVert(Vertex(vs[5].x, vs[5].y, 0.f));
	m->pushVert(Vertex(vs[0].x, vs[0].y, 0.f));
	m->setNormal(4, 0.0, 0.0, 1.0);
	m->setNormal(5, 0.0, 0.0, 1.0);
	m->setNormal(6, 0.0, 0.0, 1.0);
	m->setNormal(7, 0.0, 0.0, 1.0);
	m->setUV(4, 0.0, 0.5);
	m->setUV(5, 1 - (0.5 / tan(glm::radians(30.f))), 0.0);
	m->setUV(6, 0.5 / tan(glm::radians(30.f)), 0.0);
	m->setUV(7, 1.0, 0.5);

	sprite->childTransform->removeChild(sprite->mesh);
	delete sprite->mesh;
	sprite->mesh = m;
	sprite->childTransform->addChild(sprite->mesh, false);
	sprite->mesh->pushTexture2D(texPack.torsoTex);

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

	if(enemies.size() == 0 && enemiesLeftInWave == 0) {
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
	enemiesLeftInWave = waveNumber * 10;
	spawnTimer.restart();
}

void OJ_Arena::spawnEnemy() {
	OJ_Enemy * e = new OJ_Enemy(2.f, new OJ_TexturePack("TORSO", "HAND"), world, OJ_Game::BOX2D_CATEGORY::kENEMY, OJ_Game::BOX2D_CATEGORY::kPLAYER | OJ_Game::BOX2D_CATEGORY::kBULLET, 1);
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



OJ_Bullet * OJ_Arena::getBullet(Texture * _tex){
	OJ_Bullet * b = new OJ_Bullet(200, world, b2_dynamicBody, false, nullptr, _tex, 1, 1, 0, 0, 1.f);
	b->setShader(shader, true);
	childTransform->addChild(b);
	bullets.push_back(b);
	return b;
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