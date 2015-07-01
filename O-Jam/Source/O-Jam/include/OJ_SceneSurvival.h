#pragma once

#include <OJ_Scene.h>
#include <UILayer.h>
#include <Box2DWorld.h>
#include <Box2DDebugDrawer.h>

#include <OJ_Player.h>
#include <OJ_Bullet.h>
#include <TextArea.h>
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
	Slider * playerOneHealth;
	Slider * playerTwoHealth;

	OJ_SceneSurvival(Game * _game);
	~OJ_SceneSurvival();

	virtual void update(Step * _step) override;

private:
	TextArea * gameOverMessage;
	Timeout gameOver;
};
