#pragma once

#include <Scene.h>
#include <UILayer.h>
#include <Box2DWorld.h>

#include <OJ_Player.h>

class JoystickManager;
class Joystick;
class Game;
class ComponentShaderBase;
class ComponentShaderText;
class Font;

class OJ_Scene : public Scene {

public:

	int sceneHeight;
	int sceneWidth;
	
	Box2DWorld * box2DWorld;
	BulletWorld * bulletWorld;

	OJ_Player * playerOne;
	OJ_Player * playerTwo;

	ComponentShaderBase * mainShader;
	ComponentShaderText * textShader;

	Font * font;

	JoystickManager * joy;

	UILayer uiLayer;
	
	OJ_Scene(Game * _game);
	~OJ_Scene();

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

private:
	void renderUi(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions);
	void updateScreenDimensions();
	void movePlayer(OJ_Player * _player, Joystick * _joystick);
};
