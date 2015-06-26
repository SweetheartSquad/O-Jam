#pragma once

#include <Timeout.h>

class NodeUI;

class Fade : public Timeout{
public:
	NodeUI * target;
	Fade(float _targetSeconds, NodeUI * _target);
	
	virtual void start() override;
};


class Fadeout : public Fade{
public:
	Fadeout(float _targetSeconds, NodeUI * _target);

	virtual void update(Step * _step) override;
	virtual void trigger() override;
};

class Fadein : public Fade{
public:
	Fadein(float _targetSeconds, NodeUI * _target);

	virtual void update(Step * _step) override;
	virtual void trigger() override;
};