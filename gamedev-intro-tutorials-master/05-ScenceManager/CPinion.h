#pragma once
#include "GameObject.h"
#include "Utils.h"

#define PINION_BBOX_WIDTH	16
#define PINION_BBOX_HEIGHT	16

#define PINION_ANI_TURN_TOPLEFT	0
#define PINION_ANI_TURN_TOPRIGHT	1
#define PINION_ANI_TURN_BOTRIGHT	2
#define PINION_ANI_TURN_BOTLEFT	3

class CPinion : public CGameObject
{

private:
	int type;
public:
	CPinion(int t);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void SetState(int state) {};

};

