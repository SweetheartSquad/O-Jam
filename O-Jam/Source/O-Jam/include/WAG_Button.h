#pragma once

#include <NodeUI.h>
#include <TextArea.h>

class WAG_Button : public TextArea{
public:
	WAG_Button(BulletWorld * _world, Scene * _scene, Font * _font, Shader * _textShader, float _width);
	void update(Step * _step) override;
};