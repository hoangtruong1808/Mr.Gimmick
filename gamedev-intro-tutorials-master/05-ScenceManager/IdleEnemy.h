#pragma once
#include "GameObject.h"

#define IDLEENEMY_BBOX_WIDTH	16
#define IDLEENEMY_BBOX_HEIGHT	16

#define IDLEENEMY_GRAVITY	0.0004f

#define IDLEENEMY_STATE_IDLE	0
#define IDLEENEMY_STATE_DIE	100

#define IDLEENEMY_ANI_IDLE	0
#define IDLEENEMY_ANI_DIE	1


class CIdleEnemy : public CGameObject
{
public:
	CIdleEnemy();
	~CIdleEnemy() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

