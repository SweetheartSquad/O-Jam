#pragma once

#include <Box2D\Box2D.h>

class OJ_Scene;

//main collision call back function
class OJ_ContactListener : public b2ContactListener{
public:
	OJ_ContactListener(OJ_Scene * _scene);

	float damageScoreMult;

	//ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB);
	virtual void BeginContact(b2Contact* _contact) override;

	virtual void playerPlayerContact(b2Contact * _contact);
	virtual void playerEnemyContact(b2Contact* b2_contact, b2Fixture * _playerFixture, b2Fixture * _enemyFixture);

	virtual void EndContact(b2Contact* _contact) override;
  
	OJ_Scene * scene;
};
