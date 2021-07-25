#pragma once
#include "GameObject.h"

#define FIRE_BBOX_WIDTH	16
#define FIRE_BBOX_HEIGHT	16

#define FIRE_ANI_BURN 0

class CFire : public CGameObject
{

public:
	CFire() { }
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state) {};
};

