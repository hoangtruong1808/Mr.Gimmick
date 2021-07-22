#pragma once
#include "GameObject.h"

#define BOATBOMB_GRAVITY	-0.16f
#define BOATBOMB_LIFT_SPEED	0.09f

#define BOATBOMB_BBOX_WIDTH 16
#define BOATBOMB_BBOX_HEIGHT 16

#define BOATBOMB_STATE_IDLE 0
#define BOATBOMB_STATE_LIFT 100
#define BOATBOMB_STATE_FALL 200
#define BOATBOMB_STATE_EXPLODE 300
#define BOATBOMB_STATE_DIE 400

#define BOATBOMB_ANI_FALL 0
#define BOATBOMB_ANI_EXPLODE 1

#define	BOATBOMB_EXPLODE_TIME	250

class CBoatBomb : public CGameObject
{
	DWORD explode_start = 0;
	int ay = 0;
public:
	CBoatBomb();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void StartExplode() { explode_start = GetTickCount(); }
	void Attack();
	void Die();
	virtual void Render();
};

