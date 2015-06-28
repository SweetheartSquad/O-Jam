#pragma once

#include <LayeredScene.h>

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

#define TITLE 0
#define STORY 1
#define INSTRUCTIONS 2

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

class OJ_TitleScene : public Scene {
public:

	int frame;

	BulletWorld * bulletWorld;

	ComponentShaderBase * mainShader;

	NodeUI * title;
	NodeUI * story;
	NodeUI * instructions;

	JoystickManager * joy;

	UILayer * uiLayer;

	OJ_TitleScene(Game* _game);
	~OJ_TitleScene();

	virtual void update(Step* _step) override;
	virtual void render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) override;

	virtual void load() override;
	virtual void unload() override;
};