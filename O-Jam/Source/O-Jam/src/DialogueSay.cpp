#pragma once

#include <DialogueSay.h>
#include <iostream>
#include <WAG_ResourceManager.h>

DialogueSay::DialogueSay() :
	currentText(-1)
{
}
DialogueSay::DialogueSay(std::string _jsonString) :
	currentText(-1)
{
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( _jsonString, root );
	if(!parsingSuccessful){
		std::cout << "JSON parse failed: " << reader.getFormattedErrorMessages() << std::endl << _jsonString;
	}
	init(root);
}

DialogueSay::DialogueSay(Json::Value _json) :
	currentText(-1)
{
	init(_json);
	
	const Json::Value conditionsArray = _json["conditions"];
	for(int i = 0; i < conditionsArray.size(); ++i){
		conditions.push_back(Condition::getCondition(conditionsArray[i]));
	}
}

void DialogueSay::init(Json::Value _json){
	speaker = _json.get("speaker", "NO_SPEAKER_DEFINED").asString();
	portrait = _json.get("portrait", "NO_PORTRAIT_DEFINED").asString();
	const Json::Value textArray = _json["text"];
	for(int i = 0; i < textArray.size(); ++i){
		text.push_back(textArray[i].asString());
	}
}

std::string DialogueSay::getCurrentText(){
	return text.at(currentText);
}

bool DialogueSay::sayNextText(){
	++currentText;
	return currentText < text.size();
}





DialogueAsk::DialogueAsk(Json::Value _json){
	init(_json);
}
DialogueAsk::DialogueAsk(std::string _jsonString){
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( _jsonString, root );
	if(!parsingSuccessful){
		std::cout << "JSON parse failed: " << reader.getFormattedErrorMessages();
	}
	init(root);
}

void DialogueAsk::init(Json::Value _json){
	DialogueSay::init(_json);
	
	const Json::Value optionsArray = _json["options"];
	for(int i = 0; i < optionsArray.size(); ++i){
		options.push_back(optionsArray[i]["text"].asString());
		optionsResults.push_back(std::vector<Trigger *>());

		const Json::Value triggersArray = optionsArray[i]["triggers"];
		for(int j = 0; j < triggersArray.size(); ++j){
			optionsResults.back().push_back(Trigger::getTrigger(triggersArray[j]));
		}
	}
}

DialogueAsk::~DialogueAsk(){
	while(optionsResults.size() > 0){
		while(optionsResults.back().size() > 0){
			delete optionsResults.back().back();
			optionsResults.back().pop_back();
		}
		optionsResults.pop_back();
	}
}

Trigger * Trigger::getTrigger(Json::Value _json){
	Trigger * res = nullptr;
	std::string type = _json.get("type", "NO_TYPE").asString();
	// create a different type of Trigger depending on the value of type
	if(type == "setVar"){
		res = new TriggerSetVar(_json);
	}else if(type == "setConversation"){
		res = new TriggerSetConversation(_json);
	}else{
		throw "invalid trigger type";
	}
	return res;
}

TriggerSetVar::TriggerSetVar(Json::Value _json) :
	target(_json.get("target", "NO_TARGET").asString()),
	variable(_json.get("variable", "NO_VARIABLE").asString()),
	newValue(_json.get("value", "NO_NEW_VALUE").asString())
{
}

void TriggerSetVar::trigger(){
	WAG_ResourceManager::playthrough->characters.at(target)->variables.at(variable) = newValue;
}

TriggerSetConversation::TriggerSetConversation(Json::Value _json) :
	newConversation(_json.get("conversation", "NO_NEW_CONVERSATION").asString())
{
}

void TriggerSetConversation::trigger(){
	WAG_ResourceManager::playthrough->currentConversation = WAG_ResourceManager::playthrough->conversations[newConversation];
}

Condition * Condition::getCondition(Json::Value _json){
	Condition * res = nullptr;
	std::string type = _json.get("type", "NO_TYPE").asString();
	// create a different type of Trigger depending on the value of type
	if(type == "equality"){
		res = new ConditionEquality(_json);
	}else{
		throw "invalid trigger type";
	}
	return res;
}

Condition::Condition(Json::Value _json) :
	target(_json.get("target", "NO_TARGET").asString()),
	variable(_json.get("variable", "NO_VARIABLE").asString())
{
}

ConditionEquality::ConditionEquality(Json::Value _json) :
	Condition(_json),
	requiredValue(_json.get("value", "NO_REQUIRED_VALUE").asString())
{
}

bool ConditionEquality::evaluate(){
	return (WAG_ResourceManager::playthrough->characters.at(target)->variables.at(variable) == requiredValue);
}