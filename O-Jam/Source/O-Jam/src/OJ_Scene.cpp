#pragma once

#include <OJ_Scene.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentTexture.h>
#include <FpsDisplay.h>
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

OJ_Scene::OJ_Scene(Game * _game) :
	Scene(_game),
	joy(new JoystickManager()),
	uiLayer(this, 0,0,0,0),
	mainShader(new ComponentShaderBase(true)),
	bulletWorld(new BulletWorld()),
	box2DWorld(new Box2DWorld(b2Vec2(0, 0))),
	box2DDebugDrawer(nullptr),
	textShader(new ComponentShaderText(true)),
	font(new Font("../assets/fonts/Mathlete-Skinny.otf", 48, false)),
	playerOne(new OJ_Player(new OJ_TexturePack("MOM_TORSO", "MOM_HAND"), box2DWorld, 0, 0, 0)),
	playerTwo(new OJ_Player(new OJ_TexturePack("SON_TORSO", "SON_HAND"), box2DWorld, 0, 0, 0)),
	testEnemy(new OJ_Enemy(new OJ_TexturePack("TORSO", "HAND"), box2DWorld, 0, 0, 0))
{
	// Set screen width and height
	updateScreenDimensions();

	// Initialize and compile the shader 
	mainShader->addComponent(new ShaderComponentTexture(mainShader));
	mainShader->compileShader();

	// Set the text color to white
	textShader->setColor(1.0f, 1.0f, 1.0f);

	
	// cheryl box
	MeshEntity * bg = new MeshEntity(MeshFactory::getCubeMesh(100));
	bg->setShader(mainShader,true);
	bg->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("DEFAULT")->texture);
	childTransform->addChild(bg, false);


	// Add the players to the scene
	childTransform->addChild(playerOne);
	childTransform->addChild(playerTwo);

	playerOne->translateComponents(-5.0f, 0.f, 0.f);
	playerTwo->translateComponents(5.0f, 0.f, 0.f);

	playerOne->setShader(mainShader, true);
	playerTwo->setShader(mainShader, true);
	
	testEnemy->setShader(mainShader, true);
	childTransform->addChild(testEnemy);

	testEnemy->targetCharacter(playerOne);
	testEnemy->rootComponent->maxVelocity = b2Vec2(10.0f, 10.0f);
	testEnemy->speed = 10.0f;

	//Set up cameras
	{Transform * t = new Transform();
	MousePerspectiveCamera * mouseCamera = new MousePerspectiveCamera();
	t->addChild(mouseCamera, false);
	cameras.push_back(mouseCamera);
	mouseCamera->farClip = 1000.f;
	t->rotate(90, 0, 1, 0, kWORLD);
	t->translate(5.0f, 1.5f, 22.5f);
	mouseCamera->yaw = 90.0f;
	mouseCamera->pitch = -10.0f;
	mouseCamera->speed = 1;}


	{Transform * t = new Transform();
	FollowCamera * gameCam = new FollowCamera(10, glm::vec3(0, 0, 0), 0, 0);
	t->addChild(gameCam, false);
	cameras.push_back(gameCam);
	gameCam->farClip = 1000.f;
	t->rotate(90, 0, 1, 0, kWORLD);
	t->translate(5.0f, 1.5f, 22.5f);
	gameCam->minimumZoom = 22.5f;
	gameCam->yaw = 90.0f;
	gameCam->pitch = -10.0f;
	activeCamera = gameCam;
	
	gameCam->addTarget(playerOne->rootComponent, 1);
	gameCam->addTarget(playerTwo->rootComponent, 1);}


#ifdef _DEBUG
	// Add the fps display
	uiLayer.addChild(new FpsDisplay(bulletWorld, this, font, textShader));
#endif



	playerOne->speed = 25.f;
	playerOne->punchSpeed = 125.f;
	playerTwo->speed = 25.f;
	playerTwo->punchSpeed = 125.f;
}

OJ_Scene::~OJ_Scene() {
	delete mainShader;
	delete textShader;
	delete joy;
	delete bulletWorld;
	delete box2DWorld;
}

void OJ_Scene::update(Step* _step) {
	joy->update(_step);
	unsigned int joyCnt = 2;
	switch(joyCnt){
		case 2:
			movePlayer(playerTwo, joy->joysticks[1]);
		case 1:
			movePlayer(playerOne, joy->joysticks[0]);
			break;
		default:
			exit;
	}

	if(keyboard->keyJustUp(GLFW_KEY_2)){
		if(box2DDebugDrawer != nullptr){
			box2DWorld->b2world->SetDebugDraw(nullptr);
			childTransform->removeChild(box2DDebugDrawer);
			delete box2DDebugDrawer;
			box2DDebugDrawer = nullptr;
		}else{
			box2DDebugDrawer = new Box2DDebugDrawer(box2DWorld);
			box2DWorld->b2world->SetDebugDraw(box2DDebugDrawer);
			box2DDebugDrawer->drawing = true;
			//box2DDebugDrawer->AppendFlags(b2Draw::e_aabbBit);
			box2DDebugDrawer->AppendFlags(b2Draw::e_shapeBit);
			box2DDebugDrawer->AppendFlags(b2Draw::e_centerOfMassBit);
			box2DDebugDrawer->AppendFlags(b2Draw::e_jointBit);
			//drawer->AppendFlags(b2Draw::e_pairBit);
			childTransform->addChild(box2DDebugDrawer, false);
		}
	}

	
	// camera controls
	if (keyboard->keyJustUp(GLFW_KEY_1)){
		if(activeCamera == cameras.back()){
			activeCamera = cameras.at(0);
		}else{
			for(unsigned long int i = 0; i < cameras.size()-1; ++i){
				if(activeCamera == cameras.at(i)){
					activeCamera = cameras.at(i+1);
					break;
				}
			}
		}
	}
	if (keyboard->keyDown(GLFW_KEY_UP)){
		activeCamera->parents.at(0)->translate((activeCamera->forwardVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		activeCamera->parents.at(0)->translate((activeCamera->forwardVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		activeCamera->parents.at(0)->translate((activeCamera->rightVectorRotated) * -static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		activeCamera->parents.at(0)->translate((activeCamera->rightVectorRotated) * static_cast<MousePerspectiveCamera *>(activeCamera)->speed);
	}

	box2DWorld->update(_step);
	Scene::update(_step);
	uiLayer.update(_step);
}

void OJ_Scene::movePlayer(OJ_Player * _player, Joystick * _joystick){
	// Calculate movement
	glm::vec2 movement(0);

	if(keyboard->keyDown(GLFW_KEY_W)){
		movement.y += 1.f;
	}if(keyboard->keyDown(GLFW_KEY_S)){
		movement.y -= 1.f;
	}if(keyboard->keyDown(GLFW_KEY_A)){
		movement.x -= 1.f;
	}if(keyboard->keyDown(GLFW_KEY_D)){
		movement.x += 1.f;
	}

	if(_joystick != nullptr){
		movement.x += _joystick->getAxis(Joystick::xbox_axes::kLX);
		movement.y += -_joystick->getAxis(Joystick::xbox_axes::kLY);
	}
	
	_player->move(movement);

	// Calculate punches
	if(_joystick != nullptr){
		glm::vec2 punchDir = glm::vec2(0);
		punchDir.x = _joystick->getAxis(Joystick::xbox_axes::kRX);
		punchDir.y = -_joystick->getAxis(Joystick::xbox_axes::kRY);
		_player->punchAngle = glm::atan(punchDir.y, punchDir.x) - glm::half_pi<float>();

		if(_joystick->buttonJustDown(Joystick::xbox_buttons::kR1)){
			_player->punchR();
		}else if(_joystick->buttonJustDown(Joystick::xbox_buttons::kL1)){
			_player->punchL();
		}
	}
}

void OJ_Scene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	Scene::render(_matrixStack, _renderOptions);
	renderUi(_matrixStack, _renderOptions);
}

void OJ_Scene::load() {
	Scene::load();
	updateScreenDimensions();
	mainShader->load();
	textShader->load();
	uiLayer.load();
	font->load();

	if(box2DDebugDrawer != nullptr){
		box2DDebugDrawer->load();
	}
}

void OJ_Scene::unload() {
	mainShader->unload();
	textShader->unload();
	font->unload();
	uiLayer.unload();

	if(box2DDebugDrawer != nullptr){
		box2DDebugDrawer->unload();
	}

	Scene::unload();
}

void OJ_Scene::renderUi(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	uiLayer.resize(0, sceneWidth, 0, sceneHeight);
	uiLayer.render(_matrixStack, _renderOptions);
}

void OJ_Scene::updateScreenDimensions() {
	glfwGetWindowSize(vox::currentContext, &sceneWidth, &sceneHeight);
}