#pragma once

#include <Box2DSuperSprite.h>
#include <Box2DWorld.h>

class OJ_TexturePack : public Node{
public:
	Texture * torsoTex;
	Texture * handTex;

	OJ_TexturePack(std::string _torsoSrc, std::string _handSrc);
};

class OJ_Character : public Box2DSuperSprite {
public:

	float speed;
	
	b2Filter sf;

	Box2DSprite * torso;
	OJ_TexturePack * texPack;

	OJ_Character(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = -1);
	
	virtual ~OJ_Character();

	// move the player
	// _v should be normalized in most cases, and the player speed, mass, etc will be taken into account here
	virtual void move(glm::vec2 _v);
};