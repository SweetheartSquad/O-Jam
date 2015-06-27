#pragma once

#include <OJ_Scene.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentTexture.h>
#include <glfw/glfw3.h>

#include <JoystickManager.h>
#include <Joystick.h>

OJ_Scene::OJ_Scene(Game * _game) :
	Scene(_game),
	joy(new JoystickManager()),
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
	
	joy->update(_step);

	glm::vec3 movement(0);

	unsigned int joyCnt = 0;

	for(unsigned int i = 0; i < GLFW_JOYSTICK_LAST; ++i){
		Joystick * joystick = joy->joysticks[0];
		if(joystick != nullptr){
			++i;
			switch(joyCnt){
				case 1:
					movePlayer(playerOne, joy->joysticks[i]);
					break;
				case 2:
					movePlayer(playerTwo, joy->joysticks[i]);
					break;
				default:
					exit;
			}
		}
		
	}
}

void OJ_Scene::movePlayer(OJ_Player * _player, Joystick * _joystick){
	glm::vec3 movement(0);

	if(_joystick != nullptr){
		// Calculate movement
		/*
		movement += playerSpeed * mass * mouseCam->forwardVectorRotated * -_joystick->getAxis(Joystick::xbox_axes::kLY);
		movement += playerSpeed * mass * mouseCam->rightVectorRotated * _joystick->getAxis(Joystick::xbox_axes::kLX);
		*/
	}

	if(movement.x != 0 || movement.y != 0 || movement.z != 0){
		// Move player
		/*
		ragdoll->upperbody->body->activate(true);
		ragdoll->upperbody->body->applyCentralImpulse(btVector3(movement.x, movement.y, movement.z));
		*/
	}
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