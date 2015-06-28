#pragma once

#include <Box2DSuperSprite.h>
#include <Box2DWorld.h>
#include <Timeout.h>

class OJ_TexturePack : public Node{
public:
	Texture * torsoTex;
	Texture * handTex;

	OJ_TexturePack(std::string _torsoSrc, std::string _handSrc);
};

class OJ_Character : public Box2DSuperSprite {
public:
	bool dead;
	Timeout hitTimer;

	float health;
	float damage;
	float speed;
	
	b2Filter sf;

	Box2DSprite * torso;
	OJ_TexturePack * texPack;

	OJ_Character(float _damage, float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	
	virtual void update(Step * _step) override;
	virtual void render(vox::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;

	virtual ~OJ_Character();

	// move the player
	// _v should be normalized in most cases, and the player speed, mass, etc will be taken into account here
	virtual void move(glm::vec2 _v);
	virtual void takeDamage(float _damage);
	virtual void die();
};