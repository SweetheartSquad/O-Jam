#pragma once

#include <node\Node.h>
#include <json\json.h>

#include <functional>
#include <NodeContent.h>

class Trigger abstract : public NodeContent{
public:
	static Trigger * getTrigger(Json::Value _json);
	
	virtual void trigger() = 0;
};

class TriggerSetVar : public Trigger{
public:
	std::string target;
	std::string variable;
	std::string newValue;

	TriggerSetVar(Json::Value _json);
	virtual void trigger() override;
};

class TriggerSetConversation : public Trigger{
public:
	std::string newConversation;

	TriggerSetConversation(Json::Value _json);
	virtual void trigger() override;
};

class Condition abstract: public NodeContent{
public:
	static Condition * getCondition(Json::Value _json);
	
	std::string target;
	std::string variable;

	Condition(Json::Value _json);

	// returns true if the condition's criteria are met
	virtual bool evaluate() = 0;
};

class ConditionEquality : public Condition{
public:
	std::string requiredValue;

	ConditionEquality(Json::Value _json);
	virtual bool evaluate() override;
};

class DialogueSay : public NodeContent{
public:
	std::string speaker;
	std::string portrait;
	std::vector<std::string> text;
	signed long int currentText;

	std::vector<Condition *> conditions;
	
	// this constructor doesn't call init
	DialogueSay();
	// parses the string into a json object and uses it to call init
	DialogueSay(std::string _jsonString);
	DialogueSay(Json::Value _json);
	virtual void init(Json::Value _json);
	
	std::string getCurrentText();
	// increments currentText
	// returns true if there is a corresponding text to say
	// returns false if the end of the dialogue has been reached
	bool sayNextText();
};


// a dialog object which prompts for user selection
class DialogueAsk : public DialogueSay{
public:
	std::vector<std::string> options;
	std::vector<std::vector<Trigger *>> optionsResults;

	DialogueAsk(std::string _jsonString);
	DialogueAsk(Json::Value _json);
	~DialogueAsk();
	virtual void init(Json::Value _json) override;
};