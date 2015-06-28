#pragma once

#include <Entity.h>
#include <Box2D/Box2D.h>
#include <Timeout.h>

class Box2DWorld;
class Shader;
class OJ_Enemy;
class OJ_Bullet;
class OJ_Scene;
class Texture;
class Box2DSprite;
class ParticleSystem;

class OJ_Arena : public Entity{
public:

	int waveNumber;
	float radius;

	Box2DWorld * world;
	Shader * shader;
	OJ_Scene * scene;
	
	std::vector<OJ_Enemy *> enemies;
	std::vector<OJ_Bullet *> bullets;
	ParticleSystem * particles;

	OJ_Arena(OJ_Scene * _scene, Box2DWorld * _world, Shader * _shader, float _radius, int _points);
	~OJ_Arena();

	void update(Step * _step) override;

	void killEnemy(OJ_Enemy * _enemy);

	OJ_Enemy * getEasyEnemy();
	OJ_Enemy * getHardEnemy();

	void spawnNextWave();

	void spawnEnemy();

	void spawnEnemyAt(OJ_Enemy * e, b2Vec2 _pos);

	OJ_Bullet * getBullet(Texture * _tex, float _size = 1.f);
	void removeBullet(OJ_Bullet * _bullet);

	Box2DSprite * getHexTile();
	Timeout startIndicatorTimer;
	float score;
private:
	int easyEnemiesLeft;
	Timeout spawnTimer;

	int hardEnemiesLeft; 
	int hardEnemiesPerRound;

	int trojansPreWave;
	int botsPerWave;
	int amoebaPerWave;

	int trojansLeft;
	int botsLeft;
	int amoebasLeft;

	float componentMultMutlt;
};