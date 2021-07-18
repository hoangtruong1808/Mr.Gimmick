#pragma once
#include "GameObject.h"

#define GUN_GRAVITY				-0.001f
#define GUN_ATTACK_TIME	2500


#define GUN_BBOX_WIDTH	16
#define GUN_BBOX_HEIGHT	16

#define GUN_STATE_IDLE	0

#define GUN_ANI_IDLE 0

class CGun : public CGameObject
{
private:
	int time = 0;

public:

	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Attack();
};