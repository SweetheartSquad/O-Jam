#pragma once

#include <Vox.h>
#include <node\Node.h>
#include <typeinfo>
#include <Texture.h>
#include <Game.h>
#include <WAG_ResourceManager.h>

#include <WAG_Game.h>

// memory leak debugging
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

int main(void){
#ifdef _DEBUG
	_CrtMemState s1;
	_CrtMemCheckpoint( &s1 );
#endif

	vox::initialize("WAG Challenge - demo");
	WAG_ResourceManager::init();
	WAG_ResourceManager::load();
	WAG_Game * game = new WAG_Game();

	while (game->isRunning){
		game->performGameLoop();
	}
	
	delete game;
	game = nullptr;
	WAG_ResourceManager::destruct();
#ifdef _DEBUG
	std::cout << "Final node count: " << Node::nodes.size() << std::endl;

	for(auto n : Node::nodes){
		std::cout << typeid(*n).name() << std::endl;
	}
#endif

	vox::destruct();
	
#ifdef _DEBUG
	_CrtMemDumpAllObjectsSince(&s1);
#endif
}