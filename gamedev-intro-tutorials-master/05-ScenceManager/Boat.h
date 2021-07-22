#pragma once
#include "GameObject.h"

#define BOAT_BBOX_WIDTH	48
#define BOAT_BBOX_HEIGHT	16

#define BOAT_ANI_DRIFT	0

#define BOAT_STATE_IDLE	0
#define BOAT_STATE_DRIFT	1

#define BOAT_SPEED 0.04f

class CBoat :	public CGameObject
{
public:
	int end;
public:
	CBoat() {};
	void SetEnd(int end) { this->end = end; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

