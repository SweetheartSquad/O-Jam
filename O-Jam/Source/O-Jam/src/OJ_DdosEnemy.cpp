#pragma once

#include <OJ_DdosEnemy.h>
#include <OJ_Game.h>

OJ_DdosEnemy::OJ_DdosEnemy(Box2DWorld * _world, float _componentmult) : 
	OJ_Enemy(2.f + _componentmult, new OJ_TexturePack("DDOS_TORSO", "DDOS_HAND"), _world, OJ_Game::BOX2D_CATEGORY::kENEMY, OJ_Game::BOX2D_CATEGORY::kPLAYER | OJ_Game::BOX2D_CATEGORY::kBULLET | OJ_Game::BOX2D_CATEGORY::kBOUNDARY, 1)
{
}