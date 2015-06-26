#pragma once

#include <NodeContent.h>
#include <node/NodeResource.h>
#include <json/json.h>

#include <Conversation.h>

#include <vector>
#include <map>

class Character : public NodeContent{
public:
	Character(Json::Value _json);
	std::string id;
	std::map<std::string, std::string> variables;
};

class JsonPlaythroughParser : public NodeContent{
public:	

	Json::Value root;
	std::map<std::string, Conversation *> conversations;
	std::map<std::string, Character *> characters;

	Conversation * currentConversation;

	JsonPlaythroughParser(std::string _jsonSrc);
};