#pragma once

#include <Entity.h>
#include <Timeout.h>

class Box2DWorld;
class Shader;
class OJ_Enemy;
class OJ_Bullet;
class OJ_Scene;
class Texture;

class OJ_Arena : public Entity{
public:

	int waveNumber;
	float radius;

	Box2DWorld * world;
	Shader * shader;
	OJ_Scene * scene;
	
	std::vector<OJ_Enemy *> enemies;
	std::vector<OJ_Bullet *> bullets;

	OJ_Arena(OJ_Scene * _scene, Box2DWorld * _world, Shader * _shader, float _radius, int _points);
	~OJ_Arena();

	void update(Step * _step) override;

	void killEnemy(OJ_Enemy * _enemy);

	void spawnNextWave();

	void spawnEnemy();

	OJ_Bullet * getBullet(Texture * _tex, float _size = 1.f);
	void removeBullet(OJ_Bullet * _bullet);
private:
	int enemiesLeftInWave;
	Timeout spawnTimer;
};