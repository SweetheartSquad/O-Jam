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

class OJ_SceneScoreAttack : public OJ_Scene {
public:
	OJ_SceneScoreAttack(Game * _game);
	~OJ_SceneScoreAttack();

	virtual void update(Step * _step) override;

private:
	TextArea * gameOverMessage;
	Timeout gameOver;
};
