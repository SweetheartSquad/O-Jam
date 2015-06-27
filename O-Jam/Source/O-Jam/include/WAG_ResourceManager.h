#pragma once

#include <ResourceManager.h>
#include <SoundManager.h>
#include <Texture.h>
#include <map>

#include <OpenALSound.h>
#include <JsonPlaythroughParser.h>

class WAG_ResourceManager : public ResourceManager{
public:
	static Texture * cursor;
	static Texture * cheryl;
	static OpenAL_Sound * stream;
	static JsonPlaythroughParser * playthrough;

	static std::string speaker;
	static std::map<std::string, OpenAL_Sound *> voices;

	static void init();
};