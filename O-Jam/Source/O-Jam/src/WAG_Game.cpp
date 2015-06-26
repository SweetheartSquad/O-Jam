#pragma once

#include <WAG_Game.h>
#include <WAG_TestScene.h>

WAG_Game::WAG_Game() :
	Game(true, std::pair<std::string, Scene *>("test", new WAG_TestScene(this)), false)
{
}

WAG_Game::~WAG_Game(){

}