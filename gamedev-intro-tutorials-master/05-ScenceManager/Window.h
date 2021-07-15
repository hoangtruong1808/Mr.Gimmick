#pragma once
#include "GameObject.h"

#define WINDOW_BBOX_WIDTH  48
#define WINDOW_BBOX_HEIGHT 16

#define WINDOW_STATE_CLOSE 0
#define WINDOW_STATE_OPENING 1
#define WINDOW_STATE_OPEN 2

#define WINDOW_ANI_CLOSE 0
#define WINDOW_ANI_OPEN 1

class CWindow : public CGameObject
{
public:
	CWindow();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void SetState(int state);
};

