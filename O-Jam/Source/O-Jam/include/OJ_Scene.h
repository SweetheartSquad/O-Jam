#pragma once

#include <LayeredScene.h>
#include <UILayer.h>
#include <Box2DWorld.h>
#include <Box2DDebugDrawer.h>

#include <OJ_Player.h>

class JoystickManager;
class Joystick;
class Game;
class ComponentShaderBase;
class ComponentShaderText;
class Font;
class OJ_Enemy;
class OJ_Arena;
class OJ_ContactListener;

class OJ_Scene : public LayeredScene {

public:

	int sceneHeight;
	int sceneWidth;

	Box2DWorld * box2DWorld;
	Box2DDebugDrawer * box2DDebugDrawer;
	BulletWorld * bulletWorld;

	OJ_ContactListener * cl;

	OJ_Player * playerOne;
	OJ_Player * playerTwo;
	float stanceDistanceSq;

	std::vector<OJ_Enemy *> enemies;

	ComponentShaderBase * mainShader;
	ComponentShaderText * textShader;

	OJ_Arena * arena;

	Font * font;

	JoystickManager * joy;

	OJ_Scene(Game * _game);
	~OJ_Scene();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

private:
	OJ_Enemy * fastBallTarget;
	OJ_Enemy * findClosestEnemy(OJ_Player * _toPlayer);
	void handlePlayerInput(OJ_Player * _player, Joystick * _joystick);
	void handleStancing(OJ_Player * _playerOne, OJ_Player * _playerTwo);

	void killEnemy(OJ_Enemy * _enemy);
};
