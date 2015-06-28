#pragma once

#include <OJ_BotEnemy.h>
#include <OJ_Game.h>

OJ_BotEnemy::OJ_BotEnemy(Box2DWorld* _world) :
	OJ_Enemy(4.f, new OJ_TexturePack("BOT_TORSO", "BOT_HAND"), _world, OJ_Game::BOX2D_CATEGORY::kENEMY, OJ_Game::BOX2D_CATEGORY::kPLAYER | OJ_Game::BOX2D_CATEGORY::kBULLET, 1)
{
}