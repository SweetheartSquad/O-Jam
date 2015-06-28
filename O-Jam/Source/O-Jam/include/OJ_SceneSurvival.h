#pragma once

#include <OJ_Scene.h>
#include <UILayer.h>
#include <Box2DWorld.h>
#include <Box2DDebugDrawer.h>

#include <OJ_Player.h>
#include <OJ_Bullet.h>
#include <TextArea.h>
#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <Slider.h>

class JoystickManager;
class Joystick;
class Game;
class ComponentShaderBase;
class ComponentShaderText;
class Font;
class OJ_Enemy;
class OJ_Arena;
class OJ_ContactListener;
class FollowCamera;

class OJ_SceneSurvival : public OJ_Scene {

public:

	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;

	int sceneHeight;
	int sceneWidth;

	Box2DDebugDrawer * box2DDebugDrawer;

	OJ_ContactListener * cl;

	float stanceDistanceSq;

	ComponentShaderBase * mainShader;

	OJ_Arena * arena;

	UILayer * uiLayer;

	JoystickManager * joy;

	FollowCamera * gameCam;
	
	Slider * playerOneHealth;
	Slider * playerTwoHealth;

	OJ_SceneSurvival(Game * _game);
	~OJ_SceneSurvival();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

private:
	void handlePlayerInput(OJ_Player * _player, Joystick * _joystick);
	void handleStancing();	


	bool snapped;
	float snapTime;
	float maxCharge;
	float minCharge;
	glm::vec3 snapPos;

	void separatePlayers(float _multiplier);
	//bool stanceCharged;
	Timeout specialTimer;

	bool beamActive;
	bool guideActive;

	glm::vec2 teamworkAngle;

	OJ_Bullet * guidedBullet;


	TextArea * gameOverMessage;
	Timeout gameOver;
};
