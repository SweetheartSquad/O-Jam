#pragma once

#include <WAG_ResourceManager.h>

#include <NumberUtils.h>
#include <Easing.h>

#include <math.h>

Texture * WAG_ResourceManager::cursor = new Texture("../assets/engine basics/cursor.png", 32, 32, true, false);
Texture * WAG_ResourceManager::cheryl = new Texture("../assets/img_cheryl.jpg", 256, 256, true, false);
Texture * WAG_ResourceManager::frameWithAnimals = new Texture("../assets/framewithanimalsaspectratio.PNG", 1920, 1080, true, false);
Texture * WAG_ResourceManager::burrow = new Texture("../assets/burrow.png", 1920, 1080, true, false);
Texture * WAG_ResourceManager::river = new Texture("../assets/river.png", 1920, 1080, true, false);
Texture * WAG_ResourceManager::foxPortrait = new Texture("../assets/fox_avatar.png", 120, 120, true, false);
Texture * WAG_ResourceManager::rabbitPortrait = new Texture("../assets/rabbit_avatar.png", 128, 128, true, false);
Texture * WAG_ResourceManager::scratchings = new Texture("../assets/scratchthings.png", 1920, 1080, true, false);
OpenAL_Sound * WAG_ResourceManager::stream = new OpenAL_SoundStreamGenerative(false, false/*, 100, 10*/);
JsonPlaythroughParser * WAG_ResourceManager::playthrough = nullptr;

std::map<std::string, OpenAL_Sound *> WAG_ResourceManager::voices;


std::string WAG_ResourceManager::speaker = "blip";

void WAG_ResourceManager::init(){
	playthrough = new JsonPlaythroughParser("../assets/json_structure.json");
	resources.push_back(cursor);
	resources.push_back(cheryl);
	resources.push_back(frameWithAnimals);
	resources.push_back(burrow);
	resources.push_back(river);
	resources.push_back(foxPortrait);
	resources.push_back(rabbitPortrait);
	resources.push_back(stream);
	resources.push_back(scratchings);

	OpenAL_Sound * voice = new OpenAL_SoundSimple("../assets/audio/fox.wav", false, false);
	voices["Fox"] = voice;
	resources.push_back(voice);

	voice = new OpenAL_SoundSimple("../assets/audio/rabbit.wav", false, false);
	voices["Rabbit"] = voice;
	resources.push_back(voice);

	voice = new OpenAL_SoundSimple("../assets/audio/blip.ogg", false, false);
	voices["blip"] = voice;
	resources.push_back(voice);
	
	stream->source->buffer->sampleRate = 48000;
	stream->source->buffer->format = AL_FORMAT_STEREO8;
	dynamic_cast<OpenAL_SoundStreamGenerative *>(stream)->generativeFunction = [](unsigned long int t){
		// a oneliner from http://countercomplex.blogspot.ca/2011/10/algorithmic-symphonies-from-one-line-of.html
		// sounds pretty much the same as in the video, so I think the system works
		//return (t>>6|t|t>>(t>>16))*10+((t>>11)&7); 
		
		/////////////////
		// experiments //
		/////////////////
		// kind of sounds like record scratching?
		//return OpenAL_SoundStreamGenerative::compressFloat(Easing::easeInOutElastic(t % 3500, 0, 0.8f, 3500) + Easing::easeInOutElastic(t % 7500, 0, 0.8f, 7500), Easing::easeInOutElastic(t % 2000, 0, 0.8f, 2000));
	
		// loud droning and quiet sliding
		//return t*t&t<<2|t>>(8+t^3);

		// this one's REALLY annoying
		//return t/9+t>>9*t|9^t&13-t<<3;

		// give it a couple seconds to warm up
		//return (t|t*t>>8)*sqrt(t>>10);
		
		//return -t&20|t&40|t*10&t>>6|(t*100&t)/256;
		//return t%63-WAG_ResourceManager::dialogueChar;
		return OpenAL_SoundStreamGenerative::compressFloat(sin(t&4|-t*3&t>>9));
		//return /*t*30>>50|*/(t*2&t)/2;
		//return t*2|t&-3|t/2*4/((unsigned long int)pow(t+1,2))>>t*3/1;
		//return t>>WAG_ResourceManager::dialogueChar;
	};
}