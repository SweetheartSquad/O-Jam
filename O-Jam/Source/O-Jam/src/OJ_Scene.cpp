#pragma once

#include <OJ_Scene.h>
#include <OJ_Game.h>

OJ_Scene::OJ_Scene(Game * _game, unsigned long int _layers) :
	LayeredScene(_game, _layers),
	playerOne(new OJ_Player(3.f, new OJ_TexturePack("MOM_TORSO", "MOM_HAND"), box2DWorld, OJ_Game::BOX2D_CATEGORY::kPLAYER, -1, -1)),
	playerTwo(new OJ_Player(1.f, new OJ_TexturePack("SON_TORSO", "SON_HAND"), box2DWorld, OJ_Game::BOX2D_CATEGORY::kPLAYER, -1, -2)),
	box2DWorld(new Box2DWorld(b2Vec2(0, 0))),
	bulletWorld(new BulletWorld()),
	textShader(new ComponentShaderText(false)),
	font(new Font("../assets/fonts/Asgalt-Regular.ttf", 60, false))
{

	waveText = new TextArea(bulletWorld, this, font, textShader, 1.f);
	waveText->horizontalAlignment = kCENTER;
	waveText->verticalAlignment = kTOP;
	waveText->setRationalWidth(1.f);
	waveText->setRationalHeight(1.f);
	waveText->setMarginTop(0.05f);
}
void OJ_Scene::showWave(int _wave){
	waveText->setText(L"WAVE " + std::to_wstring(_wave));
}