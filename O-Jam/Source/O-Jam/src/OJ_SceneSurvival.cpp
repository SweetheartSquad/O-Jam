#pragma once

#include <OJ_SceneSurvival.h>

#include <MeshFactory.h>

#include <glfw/glfw3.h>

#include <Step.h>
#include <ParticleSystem.h>
#include <System.h>
#include <Easing.h>
#include <JoystickVirtual.h>

OJ_SceneSurvival::OJ_SceneSurvival(Game * _game) :
	OJ_Scene(_game, 3),
	gameOver(2),
	gameOverMessage(nullptr)
{

	gameOver.onCompleteFunction = [this](Timeout * _this){
		OJ_ResourceManager::songs["funker"]->play(true);
		this->game->switchScene("MENU", true);
	};
	gameOver.start();


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
	if(game->kc_active && gameOverMessage == nullptr){
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

	box2DWorld->update(_step);

	OJ_Scene::update(_step);

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