#pragma once
#include "GameObject.h"
#include "Utils.h"

#define TUBE_BBOX_WIDTH	16
#define TUBE_BBOX_HEIGHT	16

#define TUBE_ANI_TURN_RIGHT	0
#define TUBE_ANI_TURN_TOP	1
#define TUBE_ANI_TURN_LEFT_BOT	2
#define TUBE_ANI_TURN_RIGHT_BOT	3
#define TUBE_ANI_TURN_RIGHT_TOP	4
#define TUBE_ANI_TURN_LEFT_TOP	5

class CTube : public CGameObject
{
public:
	int type;
	CTube(int t);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void SetState(int state) {};
	float GetX() { return this->x; };
	float GetY() { return this->y; };
};

