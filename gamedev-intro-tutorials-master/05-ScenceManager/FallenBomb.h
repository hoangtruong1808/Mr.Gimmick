#pragma once
#include "GameObject.h"

#define FALLENBOMB_GRAVITY	-0.1f

#define FALLENBOMB_BBOX_WIDTH 16
#define FALLENBOMB_BBOX_HEIGHT 16

#define FALLENBOMB_STATE_IDLE 0
#define FALLENBOMB_STATE_FALL 100
#define FALLENBOMB_STATE_EXPLODE 200
#define FALLENBOMB_STATE_DIE 300

#define FALLENBOMB_ANI_IDLE 0
#define FALLENBOMB_ANI_FALL 1
#define FALLENBOMB_ANI_EXPLODE 2

#define	FALLENBOMB_EXPLODE_TIME	250

class CFallenBomb : public CGameObject
{
	DWORD explode_start = 0;

public:
	CFallenBomb();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void StartExplode() { explode_start = GetTickCount64(); }
	void Attack();
	void Die();
	virtual void Render();
};

