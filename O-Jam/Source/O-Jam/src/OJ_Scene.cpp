#pragma once

#include <OJ_Scene.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentTexture.h>
#include <glfw/glfw3.h>

OJ_Scene::OJ_Scene(Game * _game) :
	Scene(_game),
	uiLayer(this, 0,0,0,0),
	mainShader(new ComponentShaderBase(true))
{
	// Set screen width and height
	updateScreenDimensions();

	// Initialize and compile the shader 
	mainShader->addComponent(new ShaderComponentTexture(mainShader));
	mainShader->compileShader();
}

OJ_Scene::~OJ_Scene() {
	mainShader->decrementAndDelete();
	delete joy;
}

void OJ_Scene::update(Step* _step) {
	Scene::update(_step);
}

void OJ_Scene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	Scene::render(_matrixStack, _renderOptions);
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