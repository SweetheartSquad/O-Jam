#pragma once

#include <OJ_ResourceManager.h>

#include <NumberUtils.h>
#include <Easing.h>

#include <math.h>

Texture * OJ_ResourceManager::cursor = new Texture("assets/engine basics/cursor.png", 32, 32, true, false);

Scenario * OJ_ResourceManager::playthrough = nullptr;

std::map<std::string, OpenAL_Sound *> OJ_ResourceManager::songs;
std::map<std::string, OpenAL_Sound *> OJ_ResourceManager::sounds;

std::string OJ_ResourceManager::speaker = "blip";

void OJ_ResourceManager::init(){
	playthrough = new Scenario("assets/playthrough.json");
	resources.push_back(cursor);
	resources.push_back(playthrough);

	OpenAL_Sound * funker = new OpenAL_SoundSimple("assets/audio/funker.ogg", false, false);
	songs["funker"] = funker;
	resources.push_back(funker);
	alSourcef(funker->source->sourceId, AL_GAIN, 0.3f);

	OpenAL_Sound * DDoS = new OpenAL_SoundSimple("assets/audio/Directed Denial of Service.ogg", false, false);
	songs["DDoS"] = DDoS;
	resources.push_back(DDoS);
	alSourcef(DDoS->source->sourceId, AL_GAIN, 0.4f);

	OpenAL_Sound * boof = new OpenAL_SoundSimple("assets/audio/boof.wav", false, false);
	sounds["boof"] = boof;
	resources.push_back(boof);
	alSourcef(boof->source->sourceId, AL_GAIN, 0.3f);

	OpenAL_Sound * boof2 = new OpenAL_SoundSimple("assets/audio/boof2.wav", false, false);
	sounds["boof2"] = boof2;
	resources.push_back(boof2);
	alSourcef(boof2->source->sourceId, AL_GAIN, 0.3f);

	OpenAL_Sound * pew = new OpenAL_SoundSimple("assets/audio/pew.wav", false, false);
	sounds["pew"] = pew;
	resources.push_back(pew);
	alSourcef(pew->source->sourceId, AL_GAIN, 0.3f);

	OpenAL_Sound * charge = new OpenAL_SoundSimple("assets/audio/charge.wav", false, false);
	sounds["charge"] = charge;
	resources.push_back(charge);
	alSourcef(charge->source->sourceId, AL_GAIN, 0.3f);

	OpenAL_Sound * blast = new OpenAL_SoundSimple("assets/audio/blast.wav", false, false);
	sounds["blast"] = blast;
	resources.push_back(blast);
	alSourcef(blast->source->sourceId, AL_GAIN, 0.3f);
}