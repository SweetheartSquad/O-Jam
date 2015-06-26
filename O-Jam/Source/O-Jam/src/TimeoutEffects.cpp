#pragma once

#include <TimeoutEffects.h>
#include <NodeUI.h>
#include <Easing.h>

Fade::Fade(float _targetSeconds, NodeUI * _target) :
	Timeout(_targetSeconds),
	target(_target)
{
}

void Fade::start(){
	Timeout::start();
	target->setVisible(true);
}

Fadeout::Fadeout(float _targetSeconds, NodeUI * _target) :
	Fade(_targetSeconds, _target)
{
}

void Fadeout::update(Step * _step){
	Fade::update(_step);
	if(!complete){
		target->setBackgroundColour(0,0,0, Easing::easeInQuad(elapsedSeconds, 1, 0, targetSeconds));
	}
}

void Fadeout::trigger(){
	target->setBackgroundColour(0,0,0,0);
	target->setVisible(false);
	Fade::trigger();
}

Fadein::Fadein(float _targetSeconds, NodeUI * _target) :
	Fade(_targetSeconds, _target)
{
}

void Fadein::update(Step * _step){
	Fade::update(_step);
	if(!complete){
		target->setBackgroundColour(0,0,0, Easing::easeInQuad(elapsedSeconds, 0, 1, targetSeconds));
	}
}

void Fadein::trigger(){
	target->setBackgroundColour(0,0,0,1);
	Fade::trigger();
}