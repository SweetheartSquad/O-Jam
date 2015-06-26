#pragma once

#include <TextArea.h>
#include <TextLabel.h>
#include <Timeout.h>

class DialogueTextLabel : public TextLabel{
public:
	DialogueTextLabel(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader);
	~DialogueTextLabel();

	std::vector<Timeout *> timers;
	void tickerIn(float _delay);

	virtual void update(Step * _step) override;
};

class DialogueTextArea : public TextArea{
public:
	std::vector<Timeout *> timers;
	void tickerIn(float _delay);
	virtual TextLabel * getNewLine();
	virtual void update(Step * _step) override;

	DialogueTextArea(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader);
	~DialogueTextArea();
};