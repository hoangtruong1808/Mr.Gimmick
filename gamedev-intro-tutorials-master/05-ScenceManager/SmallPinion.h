#pragma once
#include "GameObject.h"

#define SMALLPINION_BBOX_WIDTH	16
#define SMALLPINION_BBOX_HEIGHT	16

#define SMALLPINION_ANI_TURN 0

class CSmallPinion : public CGameObject
{

public:
	CSmallPinion() {};
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void SetState(int state) {};

};

