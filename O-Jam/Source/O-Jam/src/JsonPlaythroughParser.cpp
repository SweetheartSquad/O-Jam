#pragma once

#include <JsonPlaythroughParser.h>
#include <Log.h>
#include <Conversation.h>
#include <DialogueSay.h>
#include <FileUtils.h>

Character::Character(Json::Value _json) :
	id(_json.get("id", "NO_ID").asString())
{
	const Json::Value vObj = _json["variables"];
	const Json::Value::Members vMembers = vObj.getMemberNames();
	for(auto v : vMembers){
		variables[v] = vObj[v].asString();
	}
}

JsonPlaythroughParser::JsonPlaythroughParser(std::string _jsonSrc) :
	currentConversation(nullptr)
{
	std::string jsonLoaded = FileUtils::voxReadFile(_jsonSrc);
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( jsonLoaded, root );
	if(!parsingSuccessful){
		Log::error("JSON parse failed: " + reader.getFormattedErrorMessages() + "\n" + jsonLoaded);
	}else{
		
		 Json::Value charactersJson = root["characters"];
		 for(auto i = 0; i < charactersJson.size(); ++i) {
			Character * c = new Character(charactersJson[i]);
			characters[c->id] = c;
		 }

		 Json::Value conversationsJson = root["conversations"];
		 for(auto i = 0; i < conversationsJson.size(); ++i) {
			 Conversation * c = new Conversation(conversationsJson[i]);
			 conversations[c->id] = c;
		 }
	}
}