#pragma once
#include "GameObject.h"
#include "Utils.h"

#define CORD_BBOX_WIDTH	16
#define CORD_BBOX_HEIGHT	16

#define CORD_ANI_TURN_TOPLEFT	0
#define CORD_ANI_TURN_TOPRIGHT	1
#define CORD_ANI_TURN_BOTRIGHT	2
#define CORD_ANI_TURN_BOTLEFT	3

class CCord : public CGameObject
{

private:
	int type;
public:
	CCord(int t);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void SetState(int state) {};

};

