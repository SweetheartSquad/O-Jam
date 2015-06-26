#pragma once

#include <WAG_Button.h>
#include <Font.h>
#include <shader\ComponentShaderBase.h>
#include <shader\ShaderComponentText.h>
#include <shader/ShaderComponentTexture.h>
#include <MeshFactory.h>

WAG_Button::WAG_Button(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width) :
	TextArea(_world, _scene, _font, _textShader, _width),
	NodeBulletBody(_world)
{
	mouseEnabled = true;
	horizontalAlignment = kCENTER;
	verticalAlignment = kMIDDLE;
}

void WAG_Button::update(Step * _step){
	TextArea::update(_step);
	if(isHovered){
		if(isDown){
			setBackgroundColour(0, -1, -1);
			//setText(downLabel);
		}else{
			setBackgroundColour(-1, 0, -1);
			//setText(overLabel);
		}
	}else{
		setBackgroundColour(-1, -1, 0);
		//setText(normalLabel);
	}
}