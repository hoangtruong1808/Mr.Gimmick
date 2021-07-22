#pragma once
#include "GameObject.h"

#define WATER_BBOX_WIDTH 64
#define WATER_BBOX_HEIGHT 16

#define WATER_ANI_WAVE 0

class CWater : public CGameObject
{
public:
	CWater() { }
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void SetState(int state) {};

};

