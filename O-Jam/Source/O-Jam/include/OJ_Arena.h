#pragma once

#include <Entity.h>
#include <Timeout.h>

class Box2DWorld;
class Shader;
class OJ_Enemy;
class OJ_Scene;

class OJ_Arena : public Entity{
public:

	int waveNumber;
	float radius;

	Box2DWorld * world;
	Shader * shader;
	OJ_Scene * scene;

	std::vector<OJ_Enemy *> enemies;

	OJ_Arena(OJ_Scene * _scene, Box2DWorld * _world, Shader * _shader, float _radius, int _points);
	~OJ_Arena();

	void update(Step * _step) override;

	void killEnemy(OJ_Enemy * _enemy);

	void spawnNextWave();

	void spawnEnemy();

private:
	int enemiesLeftInWave;
	Timeout spawnTimer;
};