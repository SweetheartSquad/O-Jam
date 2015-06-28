#pragma once

#include <LayeredScene.h>
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

class OJ_Scene : public LayeredScene {

public:

	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;

	int sceneHeight;
	int sceneWidth;

	Box2DWorld * box2DWorld;
	Box2DDebugDrawer * box2DDebugDrawer;
	BulletWorld * bulletWorld;

	OJ_ContactListener * cl;

	OJ_Player * playerOne;
	OJ_Player * playerTwo;
	float stanceDistanceSq;

	ComponentShaderBase * mainShader;
	ComponentShaderText * textShader;

	OJ_Arena * arena;

	Font * font;

	JoystickManager * joy;

	FollowCamera * gameCam;
	
	TextArea * waveText;
	TextArea * scoreText;
	
	Slider * playerOneHealth;
	Slider * playerTwoHealth;

	Timeout * waveTextTimerScaleUp;
	Timeout * waveTextTimerScaleDown;

	OJ_Scene(Game * _game);
	~OJ_Scene();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

	void showWave(int _wave);

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

};
