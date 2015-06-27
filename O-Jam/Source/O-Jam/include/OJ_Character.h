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

	OJ_Character(OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = -1);
	virtual ~OJ_Character();

};