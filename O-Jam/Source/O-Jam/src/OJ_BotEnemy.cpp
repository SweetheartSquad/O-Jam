#pragma once

#include <OJ_BotEnemy.h>
#include <OJ_Game.h>

OJ_BotEnemy::OJ_BotEnemy(Box2DWorld* _world, float _componentmult) :
	OJ_Enemy(4.f * _componentmult, new OJ_TexturePack("BOT_TORSO", "BOT_HAND"), _world, OJ_Game::BOX2D_CATEGORY::kENEMY, OJ_Game::BOX2D_CATEGORY::kPLAYER | OJ_Game::BOX2D_CATEGORY::kBULLET, 1)
{
	health = 500.f;
	damage = 10.f;
	scoreVal = 500.f;
}