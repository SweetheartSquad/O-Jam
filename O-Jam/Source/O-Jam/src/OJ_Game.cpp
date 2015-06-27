#pragma once

#include <OJ_Game.h>
#include <OJ_Scene.h>

OJ_Game::OJ_Game() :
	Game(true, std::pair<std::string, Scene *>("test", new OJ_Scene(this)), false)
{
}

OJ_Game::~OJ_Game(){

}