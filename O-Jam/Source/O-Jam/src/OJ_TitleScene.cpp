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
	joy(new JoystickManager()),
	screenSurfaceShader(new Shader("../assets/RenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true))
{
	mainShader->addComponent(new ShaderComponentTexture(mainShader));
	mainShader->compileShader();
	screenSurface->load();

	title = new NodeUI(bulletWorld, this);
	story = new NodeUI(bulletWorld, this);
	instructions = new NodeUI(bulletWorld, this);
	credits = new NodeUI(bulletWorld, this);

	title->setRationalWidth(1.0f);
	title->setRationalHeight(1.0f);

	story->setRationalWidth(1.0f);
	story->setRationalHeight(1.0f);

	instructions->setRationalWidth(1.0f);
	instructions->setRationalHeight(1.0f);

	credits->setRationalWidth(1.0f);
	credits->setRationalHeight(1.0f);

	title->background->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("TITLE")->texture);
	story->background->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("STORY")->texture);
	instructions->background->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("INSTRUCTIONS")->texture);
	credits->background->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("CREDITS")->texture);

	uiLayer->addChild(title);
	uiLayer->addChild(story);
	uiLayer->addChild(instructions);
	uiLayer->addChild(credits);

	childTransform->addChild(uiLayer, false);

	story->setVisible(false);
	instructions->setVisible(false);
	credits->setVisible(false);

	OJ_ResourceManager::songs["funker"]->play(true);
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
	}else if((frame == INSTRUCTIONS || frame == CREDITS || frame == STORY) && joy->joysticks[0]->buttonJustDown(Joystick::kB)) {
		frame = TITLE;
		title->setVisible(true);
		instructions->setVisible(false);
		credits->setVisible(false);
		story->setVisible(false);
	}else if(frame == TITLE && joy->joysticks[0]->buttonJustDown(Joystick::kY)) {
		frame = CREDITS;
		title->setVisible(false);
		credits->setVisible(true);
	}else if(frame == STORY && joy->joysticks[0]->buttonJustDown(Joystick::kA)) {
		frame = TITLE;
		title->setVisible(true);
		story->setVisible(false);
		credits->setVisible(false);
		instructions->setVisible(false);
		game->scenes.insert(std::pair<std::string, Scene *>("GAME", new OJ_Scene(game)));
		game->switchScene("GAME", false);
		OJ_ResourceManager::songs["funker"]->stop();
	}

	glm::uvec2 sd = vox::getScreenDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);
	Scene::update(_step);
}

void OJ_TitleScene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	
	//clear();
	glUseProgram(screenSurfaceShader->getProgramId());
	GLint test = glGetUniformLocation(screenSurfaceShader->getProgramId(), "time");
	checkForGlError(0,__FILE__,__LINE__);

	GLint test2 = glGetUniformLocation(screenSurfaceShader->getProgramId(), "distortionMode");
	checkForGlError(0,__FILE__,__LINE__);

	GLint test3 = glGetUniformLocation(screenSurfaceShader->getProgramId(), "mult");
	checkForGlError(0,__FILE__,__LINE__);

	if(test != -1){
		glUniform1f(test, (float)vox::lastTimestamp);
		checkForGlError(0,__FILE__,__LINE__);
	}
	if(test2 != -1){
		glUniform1i(test2, 1);
		checkForGlError(0,__FILE__,__LINE__);
	}
	if(test3 != -1){
		glUniform1f(test3, std::abs(OJ_ResourceManager::songs["funker"]->getAmplitude()*OJ_ResourceManager::songs["funker"]->getAmplitude()*0.8f));
		checkForGlError(0,__FILE__,__LINE__);
	}

	float scale = vox::NumberUtils::randomFloat(1.0, 8);
	game->setViewport(0, 0, game->viewPortWidth * 1 / scale, game->viewPortHeight * 1 / scale);
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);

	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	Scene::render(_matrixStack, _renderOptions);
	game->setViewport(0, 0, game->viewPortWidth*scale, game->viewPortHeight*scale);

	//Render the buffer to the render surface
	screenSurface->render(screenFBO->getTextureId());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	uiLayer->render(_matrixStack, _renderOptions);
}

void OJ_TitleScene::load() {
	Scene::load();
}

void OJ_TitleScene::unload() {
	Scene::unload();
}