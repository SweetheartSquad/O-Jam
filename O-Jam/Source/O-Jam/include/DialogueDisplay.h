#pragma once

#include <Entity.h>
#include <LinearLayout.h>
#include <TextArea.h>
#include <Timeout.h>
#include <DialogueSay.h>
#include <WAG_Button.h>
#include <Easing.h>
#include <Conversation.h>
#include <DialogueText.h>
#include <TimeoutEffects.h>

class DialogueDisplay : public NodeUI{
public:
	Conversation ** stuffToSay;

	bool autoProgress;
	bool shouldSayNext;

	DialogueDisplay(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width, float _height);
	~DialogueDisplay();
	
	LinearLayout * vlayout;
	LinearLayout * hlayout;
	LinearLayout * optionslayout;
	NodeUI * framePanel;
	NodeUI * framePanelOverlay;
	NodeUI * portraitPanel;
	NodeUI * portraitPanelOverlay;
	TextArea * speaker;
	DialogueTextArea * dialogue;
	Timeout * autoProgressTimer;
	WAG_Button * progressButton;
	bool waitingForInput;
	std::vector<WAG_Button *> options;
	Font * font;
	Shader * textShader;


	virtual void update(Step * _step) override;

	// moves the conversation forward by one step and updates all of the display properties to match the new state
	// if there is nothing left to say, returns false. Returns true otherwise
	bool sayNext();

	
	Fadein * fadeTimeoutFrame;
	Fadein * fadeTimeoutPortrait;
	// load a background frame (uses the portrait attribute of the dialogue object)
	void loadFrame(std::string _portrait);
	// load a character portrait (uses the speaker attribute of the dialogue object)
	void loadPortrait(std::string _speaker);
};