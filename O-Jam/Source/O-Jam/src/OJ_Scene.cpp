#pragma once

#include <OJ_Scene.h>
#include <OJ_Game.h>

OJ_Scene::OJ_Scene(Game * _game, unsigned long int _layers) :
	LayeredScene(_game, _layers),
	playerOne(new OJ_Player(3.f, new OJ_TexturePack("MOM_TORSO", "MOM_HAND"), box2DWorld, OJ_Game::BOX2D_CATEGORY::kPLAYER, -1, -1)),
	playerTwo(new OJ_Player(1.f, new OJ_TexturePack("SON_TORSO", "SON_HAND"), box2DWorld, OJ_Game::BOX2D_CATEGORY::kPLAYER, -1, -2)),
	box2DWorld(new Box2DWorld(b2Vec2(0, 0))),
	bulletWorld(new BulletWorld()),
	textShader(new ComponentShaderText(false)),
	font(new Font("../assets/fonts/Asgalt-Regular.ttf", 60, false)),
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
	uiLayer(new UILayer(this, 0,0,0,0))
{
	

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

	//Set up camera
	delete cameras.back()->parents.at(0);
	cameras.pop_back();
	Transform * t = new Transform();
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
	gameCam->addTarget(playerTwo->rootComponent, 1);

	scoreText = new TextArea(bulletWorld, this, font, textShader, 1.f);
	scoreText->horizontalAlignment = kCENTER;
	scoreText->verticalAlignment = kBOTTOM;
	scoreText->setRationalWidth(1.f);
	scoreText->setRationalHeight(1.f);
	scoreText->setMarginBottom(0.05f);

	waveText = new TextArea(bulletWorld, this, font, textShader, 1.f);
	waveText->horizontalAlignment = kCENTER;
	waveText->verticalAlignment = kTOP;
	waveText->setRationalWidth(1.f);
	waveText->setRationalHeight(1.f);
	waveText->setMarginTop(0.05f);
	
	uiLayer->addChild(waveText);
	uiLayer->addChild(scoreText);
	

	playerOne->speed = 75.f;
	playerOne->punchSpeed = 125.f;
	playerTwo->speed = 25.f;
	playerTwo->punchSpeed = 125.f;
	


	specialTimer.onCompleteFunction = [this](Timeout * _this){
		snapped = false;
		if(playerOne != nullptr){
			playerOne->getReady(OJ_Player::Stance::kNONE);
		}if(playerTwo != nullptr){
			playerTwo->getReady(OJ_Player::Stance::kNONE);
		}separatePlayers(std::min(3.f, snapTime));
		beamActive = false;
		guideActive = false;

		OJ_ResourceManager::sounds["charge"]->stop();
		OJ_ResourceManager::sounds["pew"]->stop();

		if(guidedBullet != nullptr){
			gameCam->removeTarget(guidedBullet);
			guidedBullet = nullptr;
		}
	};

	OJ_ResourceManager::songs["DDoS"]->play(true);

	addChild(uiLayer, 2, false);
}
void OJ_Scene::showWave(int _wave){
	waveText->setText(L"WAVE " + std::to_wstring(_wave));
}




void OJ_Scene::handlePlayerInput(OJ_Player * _player, Joystick * _joystick){
	// Calculate movement
	glm::vec2 movement(0);

	if(_joystick != nullptr){
		movement.x += _joystick->getAxis(_joystick->axisLeftX);
		movement.y += -_joystick->getAxis(_joystick->axisLeftY);
	}
	
	_player->move(movement);

	// Calculate punches
	if(_joystick != nullptr){
		_player->aim = glm::vec2(0);
		_player->aim.x = _joystick->getAxis(_joystick->axisRightX);
		_player->aim.y = -_joystick->getAxis(_joystick->axisRightY);
		if(std::abs(_player->aim.x) + std::abs(_player->aim.y) > 0.5f){
			_player->punchAngle = glm::atan(_player->aim.y, _player->aim.x) - glm::half_pi<float>();
		}
		if(_joystick->buttonJustDown(_joystick->bumperRight)){
			_player->punchR();
		}else if(_joystick->buttonJustDown(_joystick->bumperLeft)){
			_player->punchL();
		}

		// stancing
		if(_player->stance == OJ_Player::Stance::kNONE && !snapped){
			if(_joystick->buttonJustDown(_joystick->faceButtonDown)){
				_player->getReady(OJ_Player::Stance::kPULL);
			}
		}else if(snapped){
			if(_joystick->buttonDown(_joystick->faceButtonRight)){
				_player->getReady(OJ_Player::Stance::kGUIDE);
			}else if(_joystick->buttonDown(_joystick->faceButtonUp)){
				_player->getReady(OJ_Player::Stance::kBEAM);
			}else if(_joystick->buttonDown(_joystick->faceButtonLeft)){
				_player->getReady(OJ_Player::Stance::kAOE);
			}
		}
	}
}


void OJ_Scene::handleStancing(){
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
			playerOne->stance == playerTwo->stance
			&& playerOne->stance != OJ_Player::Stance::kNONE
			&& playerOne->stance != OJ_Player::Stance::kPULL
			&& snapTime > minCharge
			&& !beamActive
			&& !guideActive
		){
			OJ_ResourceManager::sounds["charge"]->stop();
			playerOne->enable();
			playerTwo->enable();

			// if the players aren't close enough together, cancel the charge
			if(dist > stanceDistanceSq){
				specialTimer.trigger();
				return;
			}

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

void OJ_Scene::separatePlayers(float _multiplier){
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

void OJ_Scene::render(vox::MatrixStack* _matrixStack, RenderOptions* _renderOptions) {
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
void OJ_Scene::load() {
	Scene::load();
	mainShader->load();
	textShader->load();
	uiLayer->load();
	font->load();

	if(box2DDebugDrawer != nullptr){
		box2DDebugDrawer->load();
	}
}

void OJ_Scene::unload() {
	mainShader->unload();
	textShader->unload();
	font->unload();
	uiLayer->unload();

	if(box2DDebugDrawer != nullptr){
		box2DDebugDrawer->unload();
	}

	Scene::unload();
}

void OJ_Scene::update(Step * _step){
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

	Scene::update(_step);
}