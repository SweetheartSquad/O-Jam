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

class OJ_Scene : public LayeredScene {

public:

	int sceneHeight;
	int sceneWidth;
	
	Box2DWorld * box2DWorld;
	Box2DDebugDrawer * box2DDebugDrawer;
	BulletWorld * bulletWorld;

	OJ_Player * playerOne;
	OJ_Player * playerTwo;

	OJ_Enemy * testEnemy;

	ComponentShaderBase * mainShader;
	ComponentShaderText * textShader;

	Font * font;

	JoystickManager * joy;

	OJ_Scene(Game * _game);
	~OJ_Scene();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

private:
	void movePlayer(OJ_Player * _player, Joystick * _joystick);
};
