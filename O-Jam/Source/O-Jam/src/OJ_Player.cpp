#pragma once

#include <OJ_Player.h>

OJ_Player::OJ_Player(Box2DWorld * _world) :
	Box2DSuperSprite(_world, 0)
{
}

OJ_Player::~OJ_Player() {
}