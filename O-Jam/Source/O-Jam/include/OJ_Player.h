#pragma once

#include <Box2DWorld.h>
#include <Box2DSprite.h>
#include <TextureSampler.h>
#include <OJ_Boxer.h>
#include <Timeout.h>

class OJ_Player : public OJ_Boxer {
public:
	explicit OJ_Player(float _componentScale, OJ_TexturePack * _texPack, Box2DWorld * _world, int16 _categoryBits, int16 _maskBits = -1, int16 _groupIndex = 0);
	~OJ_Player();

	void update(Step * _step) override;
	virtual void move(glm::vec2 _v) override;
	virtual void punchL() override;
	virtual void punchR() override;
	
	glm::vec2 aim;

	enum Stance {
		kNONE,
		kPULL,
		kAOE,
		kBEAM,
		kSPIN
	} stance;
	void getReady(Stance _stance);
	
	// disables until enable is called
	void disable();
	// disabled until _seconds has elapsed or enable is called
	void disable(float _seconds);
	void enable();

private:
	bool disabled;
	Timeout disableTimer;
};