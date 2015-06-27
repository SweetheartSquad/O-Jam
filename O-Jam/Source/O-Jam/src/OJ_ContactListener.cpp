#include "OJ_ContactListener.h"
#include "OJ_Scene.h"

#include "Scene.h"
#include <OJ_Game.h>
#include <OJ_Character.h>
#include <OJ_Enemy.h>
#include <Box2dWorld.h>
#include <Box2DSprite.h>
#include <Box2DSuperSprite.h>
#include <Box2D/Box2D.h>
#include <SoundManager.h>

OJ_ContactListener::OJ_ContactListener(OJ_Scene * _scene) :
	scene(_scene),
	damageScoreMult(0.f)
{
	_scene->box2DWorld->b2world->SetContactListener(this);
}

void OJ_ContactListener::BeginContact(b2Contact * _contact){
	b2Filter fA = _contact->GetFixtureA()->GetFilterData();
	b2Filter fB = _contact->GetFixtureB()->GetFilterData();
	b2Fixture * playerFixture = nullptr;
	b2Fixture * otherFixture = nullptr;
	if((fA.categoryBits & OJ_Game::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureA();
		otherFixture = _contact->GetFixtureB();
	}else if((fB.categoryBits & OJ_Game::kPLAYER) != 0){
		playerFixture = _contact->GetFixtureB();
		otherFixture = _contact->GetFixtureA();
	}
	//std::cout << fA.categoryBits << " | " << fB.categoryBits << std::endl;

	if(playerFixture != nullptr){
		if((fA.categoryBits & OJ_Game::kPLAYER) != 0 && (fB.categoryBits & OJ_Game::kPLAYER) != 0){
			// Player-Player collision
			playerPlayerContact(_contact);
		}else if((fA.categoryBits & OJ_Game::kENEMY) != 0 || (fB.categoryBits & OJ_Game::kENEMY) != 0){
			// Player-Enemy collision
			playerEnemyContact(_contact, playerFixture, otherFixture);
		}
	}

	/*
	// behaviour stuff
	b2Fixture * behaviourFixture = nullptr;

	if((fA.categoryBits & OJ_Game::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureA();
		otherFixture = _contact->GetFixtureB();
	}else if((fB.categoryBits & OJ_Game::kBEHAVIOUR) != 0){
		behaviourFixture = _contact->GetFixtureB();
		otherFixture = _contact->GetFixtureA();
	}

	if(behaviourFixture != nullptr){
		static_cast<Behaviour *>(behaviourFixture->GetUserData())->evaluateBeginContact(otherFixture);
	}
	*/
}

void OJ_ContactListener::playerPlayerContact(b2Contact * _contact){
	b2Fixture * fxA = _contact->GetFixtureA();
	b2Fixture * fxB = _contact->GetFixtureB();

	if(fxA->GetUserData() != nullptr && fxB->GetUserData() != nullptr){
		OJ_Character * playerA = static_cast<OJ_Character *>(fxA->GetUserData());
		OJ_Character * playerB = static_cast<OJ_Character *>(fxB->GetUserData());
		
	}
}

void OJ_ContactListener::playerEnemyContact(b2Contact * _contact, b2Fixture * _playerFixture, b2Fixture * _enemyFixture){
	//std::cout << "Player-Item Collision" << std::endl;
	OJ_Player * p = static_cast<OJ_Player *>(_playerFixture->GetUserData());
	OJ_Enemy * e = static_cast<OJ_Enemy *>(_enemyFixture->GetUserData());

	// if an item is triggered as dead, don't trigger a proper contact
	//if(!item->destroy){

	// IF fixture's linear velocity is greater than userData's root body's linear velocity?
	Box2DSprite * hand = nullptr;

	if(p->handL->body->GetFixtureList() == _playerFixture){
		hand = p->handL;
	}else if(p->handR->body->GetFixtureList() == _playerFixture){
		hand = p->handR;
	}

	if(hand != nullptr){
		glm::vec3 hPos = hand->getWorldPos();
		glm::vec3 pPos = p->rootComponent->getWorldPos();

		glm::vec3 diff = hPos - pPos;
		float d = sqrtf(diff.x * diff.x + diff.y * diff.y + diff.z * diff.z);
		if(d > 4.f){
			// calculate?
			float damage = 50.f;
			e->takeDamage(damage);
		}
	}

	/*
		static_cast<Item *>(item)->hitPlayer();
		if(item->thrown || (item->held && item != p->heldItem)){
			// do some sort of damage thing here
			//PuppetResourceManager::hitSounds->playRandomSound();
			p->takeDamage(item->damage);
			item->owner->score += item->damage * damageScoreMult;
		}else if(p->heldItem == nullptr && !item->held && !item->destroy){
			p->itemToPickup = item;
			// multiple players might be able to pick it up in one update
		}
		*/
	//}
}

void OJ_ContactListener::EndContact(b2Contact* _contact){
	b2Filter fA = _contact->GetFixtureA()->GetFilterData();
	b2Filter fB = _contact->GetFixtureB()->GetFilterData();
}