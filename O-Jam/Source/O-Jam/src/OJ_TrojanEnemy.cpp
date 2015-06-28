#pragma once
 
#include <OJ_TrojanEnemy.h>
#include <OJ_Game.h>
#include <Box2DSprite.h>

const float PI = 3.1415926;

OJ_TrojanEnemy::OJ_TrojanEnemy(Box2DWorld* _world, OJ_Arena * _arena, float _componentmult) :
	OJ_Enemy(8.f + _componentmult, new OJ_TexturePack("TROJAN_TORSO", "BOT_HAND"), _world, OJ_Game::BOX2D_CATEGORY::kENEMY, OJ_Game::BOX2D_CATEGORY::kPLAYER | OJ_Game::BOX2D_CATEGORY::kBULLET, 1),
	arena(_arena)
{
	spawnTimer = new Timeout(0.7);
	health = 1500.0f;
	spawnTimer->onCompleteFunction = [this](Timeout * _this){
		if(arena->enemies.size() < 30){
			if(parents.size() > 0){
				arena->spawnEnemyAt(arena->getEasyEnemy(), rootComponent->body->GetPosition());
			}
		};
		spawnTimer->restart();
	};
	
	rootComponent->body->SetTransform(rootComponent->body->GetPosition(), PI);

	rootComponent->body->SetFixedRotation(true);

	spawnTimer->start();
}

OJ_TrojanEnemy::~OJ_TrojanEnemy() {
}

void OJ_TrojanEnemy::update(Step* _step) {
	spawnTimer->update(_step);
	OJ_Enemy::update(_step);
}
