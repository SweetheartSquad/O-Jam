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
	

	Json::Reader reader;
	Json::Value defTexJson;
	bool parsingSuccessful = reader.parse(
		"{"
			"\"id\":\"DEFAULT\",\""
			"\"type\": \"texture\","
			"\"width\": 256,"
			"\"height\": 256"
		"}", defTexJson);
	textures["DEFAULT"] = new AssetTexture(defTexJson);


	std::string jsonLoaded = FileUtils::voxReadFile(_jsonSrc);
	parsingSuccessful = reader.parse( jsonLoaded, root );
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

		 Json::Value texturesJson = root["assets"];
		 for(auto i = 0; i < texturesJson.size(); ++i) {
			 Asset * a = Asset::getAsset(texturesJson[i]);
			 AssetTexture * at = dynamic_cast<AssetTexture *>(a);
			 if(at != nullptr){
				textures[at->id] = at;
			 }
		 }
	}
}

AssetTexture * JsonPlaythroughParser::getTexture(std::string _id){
	AssetTexture * res = nullptr;
	auto it = textures.find(_id);
	if(it != textures.end()){
		res = it->second;
	}else{
		res = textures["DEFAULT"];
	}
	return res;
}