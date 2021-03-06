#include "OJ_ContactListener.h"
#include "OJ_Scene.h"

#include "Scene.h"
#include <OJ_Game.h>
#include <OJ_Character.h>
#include <OJ_Enemy.h>
#include <OJ_Bullet.h>
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
	}else{
		b2Fixture * bulletFixture = nullptr;
		if((fA.categoryBits & OJ_Game::kBULLET) != 0){
			bulletFixture = _contact->GetFixtureA();
			otherFixture = _contact->GetFixtureB();
		}else if((fB.categoryBits & OJ_Game::kBULLET) != 0){
			bulletFixture = _contact->GetFixtureB();
			otherFixture = _contact->GetFixtureA();
		}

		if(bulletFixture != nullptr){
			if((fA.categoryBits & OJ_Game::kENEMY) != 0 || (fB.categoryBits & OJ_Game::kENEMY) != 0){
				// Bullet-Enemy collision
				bulletEnemyContact(_contact, bulletFixture, otherFixture);
			}
		}
	}
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
	//std::cout << "Player-Enemy Collision" << std::endl;
	OJ_Player * p = static_cast<OJ_Player *>(_playerFixture->GetUserData());
	OJ_Enemy * e = static_cast<OJ_Enemy *>(_enemyFixture->GetUserData());

	// if player or enemy is triggered as dead, don't trigger a proper contact
	if(!p->dead && !e->dead){
		Box2DSprite * hand = nullptr;

		if(p->handL->body->GetFixtureList() == _playerFixture){
			hand = p->handL;
		}else if(p->handR->body->GetFixtureList() == _playerFixture){
			hand = p->handR;
		}

		if(hand != nullptr){
			// hand - enemy punch!
			glm::vec3 hPos = hand->getWorldPos();
			glm::vec3 pPos = p->rootComponent->getWorldPos();

			float d = glm::distance2(hPos, pPos);
			if(d > p->punchReach*p->punchReach){
				e->takeDamage(p->damage);
			}
		}else{
			// enemy - body attack!
			p->takeDamage(e->damage);
		}
	}
}

void OJ_ContactListener::bulletEnemyContact(b2Contact * _contact, b2Fixture * _bulletFixture, b2Fixture * _enemyFixture){
	//std::cout << "Bullet-Enemy Collision" << std::endl;
	OJ_Bullet * b = static_cast<OJ_Bullet *>(_bulletFixture->GetUserData());
	OJ_Enemy * e = static_cast<OJ_Enemy *>(_enemyFixture->GetUserData());

	// if bullet is triggered as destroyed or enemy is triggered as dead, don't trigger a proper contact
	if(!b->destroyed && !e->dead){
		e->takeDamage(b->damage);
		b->health -= b->damage;
	}
}

void OJ_ContactListener::EndContact(b2Contact* _contact){
	b2Filter fA = _contact->GetFixtureA()->GetFilterData();
	b2Filter fB = _contact->GetFixtureB()->GetFilterData();
}