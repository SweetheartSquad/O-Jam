#pragma once

#include <OJ_SceneSurvival.h>
#include <OJ_Game.h>
#include <OJ_ContactListener.h>
#include <shader/ComponentShaderBase.h>
#include <shader/ShaderComponentTexture.h>
#include <shader/ShaderComponentHsv.h>
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
#include <JoystickVirtual.h>
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

OJ_SceneSurvival::OJ_SceneSurvival(Game * _game) :
	OJ_Scene(_game, 3),
	box2DDebugDrawer(nullptr),
	cl(new OJ_ContactListener(this)),
	stanceDistanceSq(500),
	mainShader(new ComponentShaderBase(false)),
	joy(new JoystickManager()),
	snapped(false),
	snapTime(0),
	maxCharge(3.f),
	minCharge(1.5f),
	specialTimer(0),
	beamActive(false),
	guideActive(false),
	teamworkAngle(0),
	guidedBullet(nullptr),
	screenSurfaceShader(new Shader("../assets/RenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	gameOver(2),
	gameOverMessage(nullptr),
	uiLayer(new UILayer(this, 0,0,0,0))
{

	gameOver.onCompleteFunction = [this](Timeout * _this){
		OJ_ResourceManager::songs["funker"]->play(true);
		this->game->switchScene("MENU", true);
	};
	gameOver.start();

	screenSurfaceShader->unload();
	screenSurfaceShader->load();
	screenSurface->scaleModeMag = GL_NEAREST;
	screenSurface->load();
	screenSurface->configureDefaultVertexAttributes(screenSurfaceShader);
	// Initialize and compile the shader 
	mainShader->addComponent(new ShaderComponentTexture(mainShader));
	mainShader->addComponent(new ShaderComponentHsv(mainShader, 0, 1, 1));
	mainShader->compileShader();

	// Set the text color to white
	textShader->setColor(1.0f, 1.0f, 1.0f);
	
	arena = new OJ_Arena(this, box2DWorld, mainShader, 6, 6);
	addChild(arena, 1);

	MeshEntity * m = new MeshEntity(Resource::loadMeshFromObj("../assets/meshes/hexArena.obj").at(0));
	addChild(m, 0);
	m->parents.at(0)->scale(arena->radius * 10.f);
	m->setShader(mainShader, true);
	m->mesh->pushTexture2D(OJ_ResourceManager::playthrough->getTexture("test")->texture);

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
	gameCam = new FollowCamera(10, glm::vec3(0, 0, 0), 0, 0);
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

	scoreText = new TextArea(bulletWorld, this, font, textShader, 1.f);
	scoreText->horizontalAlignment = kCENTER;
	scoreText->verticalAlignment = kBOTTOM;
	scoreText->setRationalWidth(1.f);
	scoreText->setRationalHeight(1.f);
	scoreText->setMarginBottom(0.05f);
	
	uiLayer->addChild(waveText);
	uiLayer->addChild(scoreText);


	playerOneHealth = new Slider(bulletWorld, this, 200.f, 20.f, playerOne->health);
	playerOneHealth->setValue(playerOne->health);
	playerOneHealth->fill->setBackgroundColour(-1.f, -1.f, 0);
	playerOneHealth->setRationalWidth(0.4f);
	playerOneHealth->setMarginRight(0.1f);

	playerTwoHealth = new Slider(bulletWorld, this, 200.f, 20.f, playerOne->health);
	playerTwoHealth->setValue(playerTwo->health);
	playerTwoHealth->fill->setBackgroundColour(-1.f, -1.f, 0);
	playerTwoHealth->setRationalWidth(0.4f);
	playerTwoHealth->setMarginLeft(0.1f);
	
	HorizontalLinearLayout * hl = new HorizontalLinearLayout(bulletWorld, this);
	hl->setRationalWidth(1.f);
	hl->setRationalHeight(1.f);
	hl->addChild(playerOneHealth);
	hl->addChild(playerTwoHealth);
	hl->horizontalAlignment = kCENTER;
	hl->verticalAlignment = kMIDDLE;
	hl->setMarginTop(0.8f);
	uiLayer->addChild(hl);
	
	playerOneHealth->parents.at(0)->translate(glm::vec3(0, 50.f, 0.f));
	playerTwoHealth->parents.at(0)->translate(glm::vec3(0, 50.f, 0.f));

	playerOne->speed = 75.f;
	playerOne->punchSpeed = 125.f;
	playerTwo->speed = 25.f;
	playerTwo->punchSpeed = 125.f;


	specialTimer.onCompleteFunction = [this](Timeout * _this){
		this->snapped = false;
		if(playerOne != nullptr){
			this->playerOne->getReady(OJ_Player::Stance::kNONE);
		}if(playerTwo != nullptr){
			this->playerTwo->getReady(OJ_Player::Stance::kNONE);
		}this->separatePlayers(std::min(3.f, this->snapTime));
		this->beamActive = false;
		this->guideActive = false;

		OJ_ResourceManager::sounds["charge"]->stop();
		OJ_ResourceManager::sounds["pew"]->stop();

		if(this->guidedBullet != nullptr){
			this->gameCam->removeTarget(this->guidedBullet);
			this->guidedBullet = nullptr;
		}
	};

	OJ_ResourceManager::songs["DDoS"]->play(true);

	addChild(uiLayer, 2, false);
}

OJ_SceneSurvival::~OJ_SceneSurvival() {
	deleteChildTransform();
	delete joy;
	delete bulletWorld;
	delete box2DWorld;

	OJ_ResourceManager::songs["DDoS"]->stop();
	for(auto i : OJ_ResourceManager::sounds){
		i.second->stop();
	}
}

void OJ_SceneSurvival::update(Step* _step) {
	if(game->kc_active){
		playerOne->health = 999999999;
		playerTwo->health = 999999999;
	}

	if(arena->startIndicatorTimer.active){
		waveText->parents.at(0)->scale(Easing::easeOutQuint(arena->startIndicatorTimer.elapsedSeconds, 1.5f, -0.5f, arena->startIndicatorTimer.targetSeconds), false);
		waveText->setMarginTop(vox::NumberUtils::randomFloat(0.0025f, 0.005f) + Easing::easeOutBounce(arena->startIndicatorTimer.elapsedSeconds, 0.75f, -0.75f, arena->startIndicatorTimer.targetSeconds));
	}else{
		waveText->parents.at(0)->scale(vox::NumberUtils::randomFloat(0.98f, 1.02f), false);
		waveText->setMarginTop(vox::NumberUtils::randomFloat(0.0025f, 0.005f));
	}

	std::wstringstream ws;
	ws << "SCORE: " << arena->score;
	scoreText->setText(ws.str());

	specialTimer.update(_step);
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

	if(beamActive){
		OJ_Bullet * beamPart = arena->getBullet(OJ_ResourceManager::playthrough->getTexture("BULLET")->texture);
		beamPart->setTranslationPhysical(snapPos.x + teamworkAngle.x + vox::NumberUtils::randomFloat(-3, 3), snapPos.y + teamworkAngle.y + vox::NumberUtils::randomFloat(-3, 3), 0, false);
		beamPart->applyLinearImpulseToCenter(teamworkAngle.x*25, teamworkAngle.y*25);
	}

	if(guideActive){
		guidedBullet->applyLinearImpulseToCenter(teamworkAngle.x*std::min(maxCharge, snapTime)*10, teamworkAngle.y*std::min(maxCharge, snapTime)*10);
	}

	joy->update(_step);
	
	if(joy->joysticks[0] == nullptr){
		joy->joysticks[0] = new JoystickVirtual(0);
	}
	if(joy->joysticks[1] == nullptr){
		joy->joysticks[1] = new JoystickVirtual(1);
	}

	unsigned int joyCnt = 2;
	switch(joyCnt){
		case 2:
			if(playerTwo != nullptr){
				handlePlayerInput(playerTwo, joy->joysticks[1]);
			}
		case 1:
			if(playerOne != nullptr){
				handlePlayerInput(playerOne, joy->joysticks[0]);
			}
			break;
		default:
			exit;
	}
	
	if(playerOne != nullptr && playerTwo != nullptr){
		glm::vec2 av = (playerOne->aim + playerTwo->aim) * 0.5f;
		float angle = glm::atan(av.y, av.x);
		teamworkAngle = glm::vec2(cos(angle), sin(angle));
	}


	handleStancing();

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

	glm::uvec2 sd = vox::getScreenDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);
	uiLayer->update(_step);


	if(playerOne != nullptr){
		playerOneHealth->setValue(playerOne->health);
		if(playerOne->dead){
			gameCam->removeTarget(playerOne->rootComponent);
			if(playerTwo != nullptr){
				separatePlayers(1);
			}
			removeChild(playerOne->parents.at(0));
			delete playerOne->parents.at(0);
			playerOne = nullptr;
		}
	}
	
	if(playerTwo != nullptr){
		playerTwoHealth->setValue(playerTwo->health);
		if(playerTwo->dead){
			gameCam->removeTarget(playerTwo->rootComponent);
			if(playerOne != nullptr){
				separatePlayers(1);
			}
			removeChild(playerTwo->parents.at(0));
			delete playerTwo->parents.at(0);
			playerTwo = nullptr;
		}
	}

	if(playerOne == nullptr || playerTwo == nullptr){
		if(gameOverMessage == nullptr){
			gameOverMessage = new TextArea(bulletWorld, this, font, textShader, 1.f);
			gameOverMessage->setRationalHeight(1.f);
			gameOverMessage->horizontalAlignment = kCENTER;
			gameOverMessage->verticalAlignment = kMIDDLE;
			gameOverMessage->setText(L"CORRUPTED");
			uiLayer->addChild(gameOverMessage);
		}
		gameOver.update(_step);
		ShaderComponentHsv * s = dynamic_cast<ShaderComponentHsv *>(dynamic_cast<ComponentShaderBase *>(mainShader)->getComponentAt(1));
		s->setValue(Easing::easeInOutCubic(gameOver.elapsedSeconds, 1, -1, gameOver.targetSeconds/2.f));
	}
}

void OJ_SceneSurvival::handlePlayerInput(OJ_Player * _player, Joystick * _joystick){
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
		_player->aim = glm::vec2(0);
		_player->aim.x = _joystick->getAxis(Joystick::xbox_axes::kRX);
		_player->aim.y = -_joystick->getAxis(Joystick::xbox_axes::kRY);
		if(std::abs(_player->aim.x) + std::abs(_player->aim.y) > 0.5f){
			_player->punchAngle = glm::atan(_player->aim.y, _player->aim.x) - glm::half_pi<float>();
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
				_player->getReady(OJ_Player::Stance::kGUIDE);
			}else if(_joystick->buttonDown(Joystick::xbox_buttons::kY)){
				_player->getReady(OJ_Player::Stance::kBEAM);
			}else if(_joystick->buttonDown(Joystick::xbox_buttons::kX)){
				_player->getReady(OJ_Player::Stance::kAOE);
			}
		}
	}
}

void OJ_SceneSurvival::handleStancing(){
	if(playerOne == nullptr || playerTwo == nullptr){
		return;
	}
	// pull
	if(!snapped){
		if(playerOne->stance == OJ_Player::Stance::kPULL || playerTwo->stance == OJ_Player::Stance::kPULL){
			snapped = true;
			OJ_ResourceManager::sounds["charge"]->play(true);
			playerOne->disable();
			playerTwo->disable();
			snapTime = 0;
			snapPos = (playerOne->rootComponent->getWorldPos() + playerTwo->rootComponent->getWorldPos()) * 0.5f;
		}
	}

	
	// special
	if(snapped){
		float dist = glm::distance2(playerOne->rootComponent->getWorldPos(), playerTwo->rootComponent->getWorldPos());
		if(
			dist < stanceDistanceSq
			&& playerOne->stance == playerTwo->stance
			&& playerOne->stance != OJ_Player::Stance::kNONE
			&& playerOne->stance != OJ_Player::Stance::kPULL
			&& snapTime > minCharge
			&& !beamActive
			&& !guideActive
		){
			OJ_ResourceManager::sounds["charge"]->stop();
			playerOne->enable();
			playerTwo->enable();
			if(playerTwo->stance == OJ_Player::Stance::kAOE){
				OJ_ResourceManager::sounds["blast"]->play();
				float r = 2;
				for(float i = 0; i < 360; i += 10.f / std::min(maxCharge, snapTime)){
					glm::vec2 dir(cos(i) * r, sin(i) * r);
					
					OJ_Bullet * explosionPart = arena->getBullet(OJ_ResourceManager::playthrough->getTexture("BULLET")->texture);

					explosionPart->setTranslationPhysical(snapPos.x + dir.x + vox::NumberUtils::randomFloat(-2.5, 2.5), snapPos.y + dir.y + vox::NumberUtils::randomFloat(-2.5, 2.5), 0, false);
					explosionPart->applyLinearImpulseToCenter(dir.x*10 + vox::NumberUtils::randomFloat(-2.5, 2.5), dir.y*10 + vox::NumberUtils::randomFloat(-2.5, 2.5));

				}
				specialTimer.trigger();
			}else if(playerTwo->stance == OJ_Player::Stance::kBEAM){
				OJ_ResourceManager::sounds["pew"]->play(true);
				beamActive = true;
				specialTimer.targetSeconds = std::min(maxCharge, snapTime)*0.75f;
				specialTimer.restart();
			}else if(playerTwo->stance == OJ_Player::Stance::kGUIDE){
				OJ_ResourceManager::sounds["blast"]->play();
				guideActive = true;
				specialTimer.targetSeconds = std::min(maxCharge, snapTime)*1.25f;
				specialTimer.restart();

				guidedBullet = arena->getBullet(OJ_ResourceManager::playthrough->getTexture("BULLET")->texture, std::min(maxCharge, snapTime)*3);
				guidedBullet->life = specialTimer.targetSeconds;
				guidedBullet->health = 999999999999999999;

				guidedBullet->setTranslationPhysical(snapPos.x + teamworkAngle.x, snapPos.y + teamworkAngle.y, 0, false);
				guidedBullet->applyLinearImpulseToCenter(teamworkAngle.x*std::min(maxCharge, snapTime)*20, teamworkAngle.y*std::min(maxCharge, snapTime)*20);

				gameCam->addTarget(guidedBullet);
			}
			
		}
	}
}

void OJ_SceneSurvival::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
	//clear();
	glUseProgram(screenSurfaceShader->getProgramId());
	GLint test = glGetUniformLocation(screenSurfaceShader->getProgramId(), "time");
	checkForGlError(0,__FILE__,__LINE__);

	GLint test2 = glGetUniformLocation(screenSurfaceShader->getProgramId(), "distortionMode");
	checkForGlError(0,__FILE__,__LINE__);

	GLint test3 = glGetUniformLocation(screenSurfaceShader->getProgramId(), "mult");
	checkForGlError(0,__FILE__,__LINE__);
	GLint timeSnappedLoc = glGetUniformLocation(screenSurfaceShader->getProgramId(), "timeSnapped");
	checkForGlError(0,__FILE__,__LINE__);
	GLint snapPosLoc = glGetUniformLocation(screenSurfaceShader->getProgramId(), "snapPos");
	checkForGlError(0,__FILE__,__LINE__);

	if(test != -1){
		glUniform1f(test, (float)vox::lastTimestamp);
		checkForGlError(0,__FILE__,__LINE__);
	}
	if(test2 != -1){
		glUniform1i(test2, arena->waveNumber % 5+1);
		checkForGlError(0,__FILE__,__LINE__);
	}
	if(test3 != -1){
		glUniform1f(test3, std::abs(OJ_ResourceManager::songs["DDoS"]->getAmplitude()*OJ_ResourceManager::songs["DDoS"]->getAmplitude()*std::min(arena->waveNumber, 8)*0.1f));
		checkForGlError(0,__FILE__,__LINE__);
	}
	if(timeSnappedLoc != -1){
		glUniform1f(timeSnappedLoc, snapped ? snapTime : 0);
		checkForGlError(0,__FILE__,__LINE__);
	}
	if(snapPosLoc != -1){
		glm::vec3 snapPosScreen = gameCam->worldToScreen(snapPos, glm::vec2(game->viewPortWidth, game->viewPortHeight));
		glUniform2f(snapPosLoc, snapPosScreen.x / game->viewPortWidth, snapPosScreen.y / game->viewPortHeight);
		checkForGlError(0,__FILE__,__LINE__);
	}

	float scale = vox::NumberUtils::randomFloat(1.0, std::min(arena->waveNumber, 8)*2);
	game->setViewport(0, 0, game->viewPortWidth * 1 / scale, game->viewPortHeight * 1 / scale);
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);

	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	LayeredScene::render(_matrixStack, _renderOptions);
	game->setViewport(0, 0, game->viewPortWidth*scale, game->viewPortHeight*scale);

	//Render the buffer to the render surface
	screenSurface->render(screenFBO->getTextureId());

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	uiLayer->render(_matrixStack, _renderOptions);
}

void OJ_SceneSurvival::load() {
	Scene::load();
	mainShader->load();
	textShader->load();
	uiLayer->load();
	font->load();

	if(box2DDebugDrawer != nullptr){
		box2DDebugDrawer->load();
	}
}

void OJ_SceneSurvival::unload() {
	mainShader->unload();
	textShader->unload();
	font->unload();
	uiLayer->unload();

	if(box2DDebugDrawer != nullptr){
		box2DDebugDrawer->unload();
	}

	Scene::unload();
}

void OJ_SceneSurvival::separatePlayers(float _multiplier){
	if(playerOne == nullptr || playerTwo == nullptr){
		return;
	}

	snapped = false;
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