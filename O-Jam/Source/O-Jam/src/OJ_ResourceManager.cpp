#pragma once

#include <OJ_ResourceManager.h>

#include <NumberUtils.h>
#include <Easing.h>

#include <math.h>

Texture * OJ_ResourceManager::cursor = new Texture("../assets/engine basics/cursor.png", 32, 32, true, false);
Texture * OJ_ResourceManager::cheryl = new Texture("../assets/engine basics/img_cheryl.jpg", 256, 256, true, false);
JsonPlaythroughParser * OJ_ResourceManager::playthrough = nullptr;

std::map<std::string, OpenAL_Sound *> OJ_ResourceManager::songs;
std::map<std::string, OpenAL_Sound *> OJ_ResourceManager::sounds;


std::string OJ_ResourceManager::speaker = "blip";

void OJ_ResourceManager::init(){
	playthrough = new JsonPlaythroughParser("../assets/playthrough.json");
	resources.push_back(cursor);
	resources.push_back(cheryl);

	OpenAL_Sound * funker = new OpenAL_SoundSimple("../assets/audio/funker.wav", false, false);
	songs["funker"] = funker;
	resources.push_back(funker);

	OpenAL_Sound * thingOne = new OpenAL_SoundSimple("../assets/audio/thingOne.wav", false, false);
	sounds["thingOne"] = thingOne;
	resources.push_back(thingOne);

	OpenAL_Sound * boof = new OpenAL_SoundSimple("../assets/audio/boof.wav", false, false);
	sounds["boof"] = boof;
	resources.push_back(boof);

	OpenAL_Sound * boof2 = new OpenAL_SoundSimple("../assets/audio/boof2.wav", false, false);
	sounds["boof2"] = boof2;
	resources.push_back(boof2);

	OpenAL_Sound * pew = new OpenAL_SoundSimple("../assets/audio/pew.wav", false, false);
	sounds["pew"] = pew;
	resources.push_back(pew);
}