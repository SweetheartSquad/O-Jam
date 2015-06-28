#pragma once

#include <OJ_AmoebaEnemy.h>
#include <OJ_Game.h>
#include <Box2DSprite.h>

OJ_AmoebaEnemy::OJ_AmoebaEnemy(Box2DWorld* _world, OJ_Arena * _arena, float _componentmult) :
	OJ_Enemy(8.f + _componentmult, new OJ_TexturePack("BULLET", "BOT_HAND"), _world, OJ_Game::BOX2D_CATEGORY::kENEMY, OJ_Game::BOX2D_CATEGORY::kPLAYER | OJ_Game::BOX2D_CATEGORY::kBULLET | OJ_Game::BOX2D_CATEGORY::kBOUNDARY, 1),
	arena(_arena)
{
	spawnTimer = new Timeout(8.0f);
	health = 500.0f;
	spawnTimer->onCompleteFunction = [this](Timeout * _this){
		if(arena->enemies.size() < 30){
			if(parents.size() > 0){
				arena->spawnEnemyAt(new OJ_AmoebaEnemy(world, arena, 1.f), rootComponent->body->GetPosition());
			}
		};

		float newTime = 8.0f - (arena->waveNumber * 0.3);

		if(newTime > 1.0f) {
			spawnTimer->targetSeconds = newTime;
		}else {
			spawnTimer->targetSeconds = 1.0;
		}
		spawnTimer->restart();
	};
	spawnTimer->start();
}

OJ_AmoebaEnemy::~OJ_AmoebaEnemy() {
	delete spawnTimer;
}

void OJ_AmoebaEnemy::update(Step* _step) {
	spawnTimer->update(_step);
	OJ_Enemy::update(_step);
}
