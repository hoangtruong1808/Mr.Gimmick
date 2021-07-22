#pragma once
#include "GameObject.h"

#define CANNONBULLET_GRAVITY				-0.005f

#define CANNONBULLET_BOX_WIDTH 16
#define CANNONBULLET_BOX_HEIGHT 16

#define CANNONBULLET_STATE_MOVE	0
#define CANNONBULLET_STATE_IMPACT	1
#define CANNONBULLET_STATE_DIE	2

#define CANNONBULLET_DIE_Y 128

#define CANNONBULLET_ANI_MOVE	0


class CCannonBullet : public CGameObject
{
public:
	CCannonBullet();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Die();
};