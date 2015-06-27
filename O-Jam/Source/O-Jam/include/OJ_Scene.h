#pragma once

#include <Scene.h>
#include <UILayer.h>
#include <Box2DWorld.h>

class JoystickManager;
class Game;

class OJ_Scene : public Scene {
public:
	int sceneHeight;
	int sceneWidth;

	JoystickManager * joy;
	ComponentShaderBase * mainShader;
	
	Box2DWorld b2World;
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
};
