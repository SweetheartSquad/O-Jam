#pragma once

#include <OJ_Scene.h>
#include <OJ_Game.h>
#include <OJ_ContactListener.h>
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
#include <OJ_Arena.h>
#include <Slider.h>
#include <Step.h>

OJ_Scene::OJ_Scene(Game * _game) :
	LayeredScene(_game, 2),
	joy(new JoystickManager()),
	mainShader(new ComponentShaderBase(true)),
	bulletWorld(new BulletWorld()),
	box2DWorld(new Box2DWorld(b2Vec2(0, 0))),
	box2DDebugDrawer(nullptr),
	cl(new OJ_ContactListener(this)),
	textShader(new ComponentShaderText(true)),
	font(new Font("../assets/fonts/Mathlete-Skinny.otf", 48, false)),
	playerOne(new OJ_Player(3.f, new OJ_TexturePack("MOM_TORSO", "MOM_HAND"), box2DWorld, OJ_Game::BOX2D_CATEGORY::kPLAYER, -1, -1)),
	playerTwo(new OJ_Player(1.f, new OJ_TexturePack("SON_TORSO", "SON_HAND"), box2DWorld, OJ_Game::BOX2D_CATEGORY::kPLAYER, -1, -2)),
	stanceDistanceSq(500),
	snapped(false),
	snapTime(0)
{

	// Initialize and compile the shader 
	mainShader->addComponent(new ShaderComponentTexture(mainShader));
	mainShader->compileShader();

	// Set the text color to white
	textShader->setColor(1.0f, 1.0f, 1.0f);
	
	arena = new OJ_Arena(this, box2DWorld, mainShader, 50, 12);
	addChild(arena, 1);

	
	// cheryl box
	MeshEntity * bg = new MeshEntity(MeshFactory::getCubeMesh(100));
	bg->setShader(mainShader,true);
	bg->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("DEFAULT")->texture);
	addChild(bg, 0, false);


	// Add the players to the scene
	addChild(playerOne, 1);
	addChild(playerTwo, 1);

	playerOne->punchReach = 3.0f;

	playerOne->translateComponents(-5.0f, 0.f, 0.f);
	playerTwo->translateComponents(5.0f, 0.f, 0.f);

	playerOne->setShader(mainShader, true);
	playerTwo->setShader(mainShader, true);

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

	Slider * slider = new Slider(bulletWorld, this, 100.f, 30.f, 100.f);
	slider->setValue(50.f);
	slider->fill->setBackgroundColour(0.f, -1.f, -1.f);
	uiLayer.addChild(slider);

	slider->parents.at(0)->translate(glm::vec3(150.f, 50.f, 0.f));

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
	snapTime += _step->deltaTime;
	if(snapped){
		glm::vec3 v = snapPos - playerOne->rootComponent->getWorldPos();
		v = glm::normalize(v);
		float s = playerOne->rootComponent->body->GetMass() * 10 * std::min(snapTime, 10.f);
		playerOne->rootComponent->applyLinearImpulseToCenter(v.x*s, v.y*s);

		v = snapPos - playerTwo->rootComponent->getWorldPos();
		v = glm::normalize(v);
		s = playerTwo->rootComponent->body->GetMass() * 10 * std::min(snapTime, 10.f);
		playerTwo->rootComponent->applyLinearImpulseToCenter(v.x*s, v.y*s);
	}

	joy->update(_step);
	unsigned int joyCnt = 2;
	switch(joyCnt){
		case 2:
			handlePlayerInput(playerTwo, joy->joysticks[1]);
		case 1:
			handlePlayerInput(playerOne, joy->joysticks[0]);
			break;
		default:
			exit;
	}

	handleStancing(playerOne, playerTwo);

	if(keyboard->keyJustUp(GLFW_KEY_2)){
		if(box2DDebugDrawer != nullptr){
			box2DWorld->b2world->SetDebugDraw(nullptr);
			removeChild(box2DDebugDrawer);
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
			addChild(box2DDebugDrawer, 1, false);
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

void OJ_Scene::handlePlayerInput(OJ_Player * _player, Joystick * _joystick){
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
		if(std::abs(punchDir.x) + std::abs(punchDir.y) > 0.5f){
			_player->punchAngle = glm::atan(punchDir.y, punchDir.x) - glm::half_pi<float>();
		}
		if(_joystick->buttonJustDown(Joystick::xbox_buttons::kR1)){
			_player->punchR();
		}else if(_joystick->buttonJustDown(Joystick::xbox_buttons::kL1)){
			_player->punchL();
		}

		// stancing
		if(_player->stance == OJ_Player::Stance::kNONE && !snapped){
			if(_joystick->buttonJustDown(Joystick::xbox_buttons::kA)){
				_player->getReady(OJ_Player::Stance::kPULL);
			}
		}else if(snapped){
			if(_joystick->buttonDown(Joystick::xbox_buttons::kB)){
				_player->getReady(OJ_Player::Stance::kSPIN);
			}else if(_joystick->buttonDown(Joystick::xbox_buttons::kY)){
				_player->getReady(OJ_Player::Stance::kBEAM);
			}else if(_joystick->buttonDown(Joystick::xbox_buttons::kX)){
				_player->getReady(OJ_Player::Stance::kAOE);
			}
		}
	}
}

void OJ_Scene::handleStancing(OJ_Player * _playerOne, OJ_Player * _playerTwo){
	// pull
	if(!snapped){
		if(_playerOne->stance == OJ_Player::Stance::kPULL || _playerTwo->stance == OJ_Player::Stance::kPULL){
			snapped = true;
			_playerOne->disable();
			_playerTwo->disable();
			snapTime = 0;
			snapPos = (_playerOne->rootComponent->getWorldPos() + _playerTwo->rootComponent->getWorldPos()) * 0.5f;
		}
	}

	
	// special
	if(snapped){
		float dist = glm::distance2(_playerOne->rootComponent->getWorldPos(), _playerTwo->rootComponent->getWorldPos());
		if(
			dist < stanceDistanceSq
			&& _playerOne->stance == _playerTwo->stance
			&& _playerOne->stance != OJ_Player::Stance::kNONE
			&& _playerOne->stance != OJ_Player::Stance::kPULL
			&& snapTime > 1.5f
		){
			snapped = false;
			_playerOne->enable();
			_playerTwo->enable();
			if(_playerTwo->stance == OJ_Player::Stance::kAOE){
				float r = 2;
				for(float i = 0; i < 360; i += 30.f / std::min(3.f, snapTime)){
					glm::vec2 dir(cos(i) * r, sin(i) * r);
					
					OJ_Bullet * explosionPart = arena->getBullet(OJ_ResourceManager::playthrough->getTexture("DEFAULT")->texture);

					b2Filter sf;
					sf.categoryBits = OJ_Game::BOX2D_CATEGORY::kBULLET;
					sf.maskBits = OJ_Game::BOX2D_CATEGORY::kENEMY;
					sf.groupIndex = 0;
					explosionPart->createFixture(sf, b2Vec2(0, 0), explosionPart, false);

					explosionPart->setTranslationPhysical(snapPos.x + dir.x, snapPos.y + dir.y, 0, false);
					explosionPart->applyLinearImpulseToCenter(dir.x*10, dir.y*10);
				}
			}else if(_playerTwo->stance == OJ_Player::Stance::kBEAM){
				/*float angle = (_playerOne->punchAngle + _playerTwo->punchAngle) * 0.5f;

				glm::vec2 dir(cos(angle), sin(angle));

				for(float i = 0; i < 360; i += 30.f / std::min(3.f, snapTime)){
					OJ_Bullet * explosionPart = new OJ_Bullet(100, box2DWorld, b2_dynamicBody, false, nullptr, OJ_ResourceManager::playthrough->getTexture("DEFAULT")->texture, 1, 1, 0, 0, 1.f);
					explosionPart->setShader(mainShader, true);
					addChild(explosionPart, 1);
					bullets.push_back(explosionPart);

					b2Filter sf;
					sf.categoryBits = OJ_Game::BOX2D_CATEGORY::kBULLET;
					sf.maskBits = OJ_Game::BOX2D_CATEGORY::kENEMY;
					sf.groupIndex = 0;
					explosionPart->createFixture(sf, b2Vec2(0, 0), explosionPart, false);

					explosionPart->setTranslationPhysical(snapPos.x + dir.x, snapPos.y + dir.y, 0, false);
					explosionPart->applyLinearImpulseToCenter(dir.x*10, dir.y*10);
				}*/
			}else if(_playerTwo->stance == OJ_Player::Stance::kSPIN){

			}
			
			_playerOne->getReady(OJ_Player::Stance::kNONE);
			_playerTwo->getReady(OJ_Player::Stance::kNONE);
			separatePlayers(std::min(3.f, snapTime));
		}
	}

	/*if(_playerOne->stance != OJ_Player::Stance::kNONE && _playerTwo->stance != OJ_Player::Stance::kNONE){
		float dist = glm::distance2(_playerOne->rootComponent->getWorldPos(), _playerTwo->rootComponent->getWorldPos());
		if(dist < stanceDistanceSq && _playerOne->stance == _playerTwo->stance){
			
			}else if(snapTimer.active){
				
			}
		}else{
			// the players were either too far apart or didn't synchronize
			// punish by pushing them apart
			_playerOne->disable(0.25f);
			_playerTwo->disable(0.25f);

			glm::vec3 v = _playerOne->rootComponent->getWorldPos() - _playerTwo->rootComponent->getWorldPos();
			v = glm::normalize(v);
			float s = _playerOne->rootComponent->body->GetMass() * 50;
			_playerOne->rootComponent->applyLinearImpulseToCenter(v.x*s, v.y*s);

			v = _playerTwo->rootComponent->getWorldPos() - _playerOne->rootComponent->getWorldPos();
			v = glm::normalize(v);
			s = _playerTwo->rootComponent->body->GetMass() * 50;
			_playerTwo->rootComponent->applyLinearImpulseToCenter(v.x*s, v.y*s);
		}
	}*/
}

void OJ_Scene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	LayeredScene::render(_matrixStack, _renderOptions);
}

void OJ_Scene::load() {
	Scene::load();
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

/*
OJ_Enemy * OJ_Scene::findClosestEnemy(OJ_Player * _toPlayer){
	OJ_Enemy * res = nullptr;
	float dmin = 100000000000;
	glm::vec3 dv(0);
	float d;
	for(unsigned long int i = 0; i < enemies.size(); ++i){
		dv = enemies.at(i)->rootComponent->worldPos - _toPlayer->rootComponent->getWorldPos();
		d = glm::length2(dv);
		if(d < dmin){
			dmin = d;
			res = enemies.at(i);
		}
	}
	return res;
}
*/


void OJ_Scene::separatePlayers(float _multiplier){
	playerOne->disable(0.25f*_multiplier);
	playerTwo->disable(0.25f*_multiplier);

	glm::vec3 v = playerOne->rootComponent->getWorldPos() - playerTwo->rootComponent->getWorldPos();
	v = glm::normalize(v);
	float s = playerOne->rootComponent->body->GetMass() * 50 * _multiplier;
	playerOne->rootComponent->applyLinearImpulseToCenter(v.x*s, v.y*s);

	v = playerTwo->rootComponent->getWorldPos() - playerOne->rootComponent->getWorldPos();
	v = glm::normalize(v);
	s = playerTwo->rootComponent->body->GetMass() * 50 * _multiplier;
	playerTwo->rootComponent->applyLinearImpulseToCenter(v.x*s, v.y*s);
}