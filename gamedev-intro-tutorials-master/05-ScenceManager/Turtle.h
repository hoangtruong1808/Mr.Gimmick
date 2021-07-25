#pragma once
#include "GameObject.h"

#define TURTLE_BBOX_WIDTH  22
#define TURTLE_BBOX_HEIGHT 16

#define TURTLE_SPEED 0.01f

#define TURTLE_STATE_WALKING_RIGHT 0
#define TURTLE_STATE_WALKING_LEFT 1
#define TURTLE_STATE_DIE_RIGHT 2
#define TURTLE_STATE_DIE_LEFT 3

#define TURTLE_ANI_WALKING_RIGHT 0
#define TURTLE_ANI_WALKING_LEFT 1
#define TURTLE_ANI_DIE_RIGHT	2
#define TURTLE_ANI_DIE_LEFT	3

class CTurtle : public CGameObject
{
public:
	int start;
	int end;
public:
	CTurtle();
	void SetStart(int start) { this->start = start; }
	void SetEnd(int end) { this->end = end; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};