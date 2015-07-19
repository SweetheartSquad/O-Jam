#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>
#include <Texture.h>
#include <map>

#include <OpenALSound.h>
#include <scenario/Scenario.h>

class OJ_ResourceManager : public ResourceManager{
public:
	static Texture * cursor;

	static Scenario * playthrough;

	static std::string speaker;
	static std::map<std::string, OpenAL_Sound *> songs;
	static std::map<std::string, OpenAL_Sound *> sounds;

	static void init();
};