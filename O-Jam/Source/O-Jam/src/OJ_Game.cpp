#pragma once

#include <OJ_Game.h>
#include <OJ_Scene.h>
#include <OJ_TitleScene.h>

OJ_Game::OJ_Game() :
	Game(true, std::pair<std::string, Scene *>("test", new OJ_TitleScene(this)), false)
{
}

OJ_Game::~OJ_Game(){

}