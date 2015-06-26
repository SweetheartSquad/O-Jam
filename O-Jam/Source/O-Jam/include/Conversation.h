#pragma once

#include <NodeContent.h>
#include <DialogueSay.h>

class Conversation : public NodeContent{
private:
	// keeps track of the progress within this conversation
	unsigned long int currentDialogue;

public:
	std::string id;
	std::vector<DialogueSay *> dialogueObjects;
	Conversation(Json::Value _json);

	// returns dialogueObjects.at(currentDialogue)
	DialogueSay * getCurrentDialogue();
	// increments currentDialogue
	// returns true if there is a corresponding dialogue object
	// returns false if the end of the conversation has been reached
	bool sayNextDialogue();
};