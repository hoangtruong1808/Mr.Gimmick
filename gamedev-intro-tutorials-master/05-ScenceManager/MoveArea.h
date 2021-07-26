#pragma once

#include "GameObject.h"

#define MOVEAREA_BBOX_WIDTH  16
#define MOVEAREA_BBOX_HEIGHT 16

/*
	Object that triggers scene switching
*/
class CMoveArea : public CGameObject
{
	int move_x;
	int move_y;
public:
	CMoveArea(float move_x, float move_y);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
};