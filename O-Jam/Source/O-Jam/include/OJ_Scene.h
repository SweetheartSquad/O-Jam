#pragma once

#include <LayeredScene.h>

#include <OJ_Game.h>
#include <OJ_ResourceManager.h>
#include <OJ_Player.h>
#include <OJ_Bullet.h>
#include <OJ_Arena.h>
#include <OJ_ContactListener.h>
#include <OJ_Enemy.h>

#include <TextArea.h>
#include <shader\ComponentShaderBase.h>
#include <shader\ComponentShaderText.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentHsv.h>
#include <BulletWorld.h>
#include <Box2DWorld.h>
#include <Box2DDebugDrawer.h>
#include <Font.h>
#include <JoystickManager.h>
#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <FollowCamera.h>
#include <NumberUtils.h>
#include <Resource.h>
#include <Keyboard.h>

class OJ_Scene : public LayeredScene{
public:
	FollowCamera * gameCam;

	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;
	
	ComponentShaderText * textShader;
	ComponentShaderBase * mainShader;

	Box2DDebugDrawer * box2DDebugDrawer;
	Box2DWorld * box2DWorld;
	BulletWorld * bulletWorld;
	OJ_ContactListener * cl;

	UILayer * uiLayer;
	
	Font * font;
	TextArea * waveText;
	TextArea * scoreText;
	
	OJ_Arena * arena;
	OJ_Player * playerOne;
	OJ_Player * playerTwo;

	JoystickManager * joy;

	void showWave(int _wave);

	OJ_Scene(Game * _game, unsigned long int _layers);

	void handlePlayerInput(OJ_Player * _player, Joystick * _joystick);
	void handleStancing();

	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	virtual void update(Step * _step) override;
	
	virtual void load() override;
	virtual void unload() override;

protected:
	void separatePlayers(float _multiplier);
	//bool stanceCharged;
	Timeout specialTimer;

	bool beamActive;
	bool guideActive;

	glm::vec2 teamworkAngle;

	OJ_Bullet * guidedBullet;

	bool snapped;
	float snapTime;
	float maxCharge;
	float minCharge;
	float stanceDistanceSq;
	glm::vec3 snapPos;
};