#pragma once

#include <OJ_Scene.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentTexture.h>
#include <FpsDisplay.h>
#include <shader/ComponentShaderText.h>
#include <Font.h>
#include <OJ_Player.h>

#include <glfw/glfw3.h>

OJ_Scene::OJ_Scene(Game * _game) :
	Scene(_game),
	uiLayer(this, 0,0,0,0),
	mainShader(new ComponentShaderBase(true)),
	bulletWorld(new BulletWorld()),
	textShader(new ComponentShaderText(true)),
	font(new Font("../assets/fonts/Mathlete-Skinny.otf", 48, false)),
	playerOne(new OJ_Player(nullptr, b2World, 0, 0, 0)),
	playerTwo(new OJ_Player(nullptr, b2World, 0, 0, 0))
{
	// Set screen width and height
	updateScreenDimensions();

	// Initialize and compile the shader 
	mainShader->addComponent(new ShaderComponentTexture(mainShader));
	mainShader->compileShader();

	// Set the text color to white
	textShader->setColor(1.0f, 1.0f, 1.0f);

	// Add the players to the scene
	childTransform->addChild(playerOne);
	childTransform->addChild(playerTwo);

#ifdef _DEBUG
	// Add the fps display
	uiLayer.addChild(new FpsDisplay(bulletWorld, this, font, textShader));
#endif

}

OJ_Scene::~OJ_Scene() {
	delete mainShader;
	delete textShader;
	delete joy;
	delete bulletWorld;
	delete b2World;
}

void OJ_Scene::update(Step* _step) {
	Scene::update(_step);
	uiLayer.update(_step);
}

void OJ_Scene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	Scene::render(_matrixStack, _renderOptions);
	renderUi(_matrixStack, _renderOptions);
}

void OJ_Scene::load() {
	Scene::load();
	updateScreenDimensions();
}

void OJ_Scene::unload() {
	Scene::unload();
}

void OJ_Scene::renderUi(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	uiLayer.resize(0, sceneWidth, 0, sceneHeight);
	uiLayer.render(_matrixStack, _renderOptions);
}

void OJ_Scene::updateScreenDimensions() {
	glfwGetWindowSize(vox::currentContext, &sceneWidth, &sceneHeight);
}