#pragma once

#include <OJ_Scene.h>
#include <OJ_Game.h>
#include <OJ_ContactListener.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentTexture.h>
#include <shader/ComponentShaderText.h>
#include <Font.h>
#include <OJ_Player.h>
#include <Keyboard.h>
#include <FollowCamera.h>
#include <MousePerspectiveCamera.h>
#include <MeshFactory.h>
#include <OJ_ResourceManager.h>

#include <glfw/glfw3.h>

#include <JoystickManager.h>
#include <Joystick.h>
#include <OJ_Enemy.h>
#include <OJ_Arena.h>
#include <Slider.h>
#include <Step.h>
#include <NumberUtils.h>
#include <Timeout.h>
#include <Resource.h>
#include <ParticleSystem.h>
#include <System.h>
#include <Easing.h>
#include <OJ_TitleScene.h>
#include <NodeUI.h>
#include <OJ_ResourceManager.h>

OJ_TitleScene::OJ_TitleScene(Game* _game) :
	Scene(_game),
	bulletWorld(new BulletWorld()),
	mainShader(new ComponentShaderBase(true)),
	uiLayer(new UILayer(this, 0, 0, 0, 0)),
	frame(0),
	joy(new JoystickManager())
{
	mainShader->addComponent(new ShaderComponentTexture(mainShader));
	mainShader->compileShader();

	title = new NodeUI(bulletWorld, this);
	story = new NodeUI(bulletWorld, this);
	instructions = new NodeUI(bulletWorld, this);

	title->setRationalWidth(1.0f);
	title->setRationalHeight(1.0f);

	story->setRationalWidth(1.0f);
	story->setRationalHeight(1.0f);

	instructions->setRationalWidth(1.0f);
	instructions->setRationalHeight(1.0f);

	title->background->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("TITLE")->texture);
	story->background->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("STORY")->texture);
	instructions->background->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("DEFAULT")->texture);

	uiLayer->addChild(title);
	uiLayer->addChild(story);
	uiLayer->addChild(instructions);

	childTransform->addChild(uiLayer, false);

	story->setVisible(false);
	instructions->setVisible(false);
}

OJ_TitleScene::~OJ_TitleScene() {
}

void OJ_TitleScene::update(Step* _step) {
	joy->update(_step);
	
	if(frame == TITLE && joy->joysticks[0]->buttonJustDown(Joystick::kA)) {
		frame = STORY;
		story->setVisible(true);
		title->setVisible(false);
	}else if(frame == TITLE && joy->joysticks[0]->buttonJustDown(Joystick::kX)){
		frame = INSTRUCTIONS;
		instructions->setVisible(true);
		title->setVisible(false);
	}else if(frame == INSTRUCTIONS && joy->joysticks[0]->buttonJustDown(Joystick::kB)) {
		frame = TITLE;
		title->setVisible(true);
		instructions->setVisible(false);
	}else if(frame == STORY && joy->joysticks[0]->buttonJustDown(Joystick::kA)) {
		// Start game;
	}

	glm::uvec2 sd = vox::getScreenDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);
	Scene::update(_step);
}

void OJ_TitleScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	Scene::render(_matrixStack, _renderOptions);
}

void OJ_TitleScene::load() {
	Scene::load();
}

void OJ_TitleScene::unload() {
	Scene::unload();
}