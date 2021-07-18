#pragma once
#include "GameObject.h"

#define BULLET_GRAVITY				-0.005f

#define BULLET_BOX_WIDTH 12
#define BULLET_BOX_HEIGHT 12

#define BULLET_STATE_MOVE	0
#define BULLET_STATE_EXPLODE	1
#define BULLET_STATE_DIE	2

#define BULLET_ANI_MOVE	0
#define BULLET_ANI_EXPLODE	1

#define BULLET_EXPLODE_TIME	250


class CBullet : public CGameObject
{
	DWORD explode_start = 0;
public:
	CBullet();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void StartExplode() { explode_start = GetTickCount(); }
	void Die();
};