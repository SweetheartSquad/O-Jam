#pragma once

#include <LayeredScene.h>
#include <OJ_Player.h>
#include <TextArea.h>
#include <shader\ComponentShaderText.h>
#include <BulletWorld.h>
#include <Box2DWorld.h>
#include <Font.h>

class OJ_Scene : public LayeredScene{
public:
	Box2DWorld * box2DWorld;
	BulletWorld * bulletWorld;
	
	ComponentShaderText * textShader;
	Font * font;

	TextArea * waveText;
	TextArea * scoreText;

	OJ_Player * playerOne;
	OJ_Player * playerTwo;

	void showWave(int _wave);

	OJ_Scene(Game * _game, unsigned long int _layers);
};