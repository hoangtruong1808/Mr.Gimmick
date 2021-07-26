#pragma once
#include "GameObject.h"

#define	BLACKBOMB_GRAVITY				0.0005f
#define BLACKBOMB_WALKING_SPEED			0.1f

#define	BLACKBOMB_BBOX_WIDTH				16
#define	BLACKBOMB_BBOX_HEIGHT			16

#define	BLACKBOMB_STATE_WALKING_RIGHT	100
#define	BLACKBOMB_STATE_WALKING_LEFT		200
#define	BLACKBOMB_STATE_EXPLOSIVE		300
#define	BLACKBOMB_STATE_DIE				400

#define BLACKBOMB_ANI_WALKING			0
#define BLACKBOMB_ANI_EXPLOSIVE			1

#define BLACKBOMB_ALIVE_TIME				1500
#define BLACKBOMB_EXPLOSIVE_TIME			250


class CBlackBomb : public CGameObject
{
	int alive = 0;
	int explosive = 0;

	DWORD alive_start;
	DWORD explosive_start;
public:
	CBlackBomb();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

