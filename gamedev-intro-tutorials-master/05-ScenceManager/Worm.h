#pragma once
#include "GameObject.h"

#define WORM_BBOX_WIDTH  24
#define WORM_BBOX_HEIGHT 16

#define WORM_SPEED 0.02f

#define WORM_STATE_WALKING_RIGHT 0
#define WORM_STATE_WALKING_LEFT 1

#define WORM_ANI_WALKING_RIGHT 0
#define WORM_ANI_WALKING_LEFT 1

class CWorm : public CGameObject
{
public:
	int start;
	int end;
public:
	CWorm();
	void SetStart(int start) { this->start = start; }
	void SetEnd(int end) { this->end = end; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};