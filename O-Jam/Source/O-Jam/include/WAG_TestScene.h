#pragma once

#include <Scene.h>
#include <SoundManager.h>
#include <UILayer.h>
#include <Joystick.h>
#include <JoystickManager.h>

#include <Font.h>

#include <BulletDebugDrawer.h>
#include <BulletWorld.h>
#include <TextLabel.h>

class MousePerspectiveCamera;
class FollowCamera;

class Box2DWorld;
class Box2DDebugDrawer;
class Box2DMeshEntity;
class MeshEntity;

class ShaderComponentHsv;

class Shader;
class RenderSurface;
class StandardFrameBuffer;
class Material;
class Sprite;

class PointLight;

class BulletMeshEntity;
class BulletRagdoll;
class ComponentShaderText;


class WAG_TestScene : public Scene{
public:
	ComponentShaderBase * shader;
	ComponentShaderText * textShader;

	Font  * font;

	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;
	Material * phongMat;

	float sceneHeight;
	float sceneWidth;

	JoystickManager * joy;

	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

	UILayer uiLayer;
	Sprite * mouseIndicator;

	WAG_TestScene(Game * _game);
	~WAG_TestScene();
};