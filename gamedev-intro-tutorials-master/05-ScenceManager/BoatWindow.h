#pragma once
#include "GameObject.h"

#define BOATWINDOW_ANI	0
#define BOATWINDOW_BBOX_WIDTH	32
#define BOATWINDOW_BBOX_HEIGHT	48

class CBoatWindow : public CGameObject
{
public:
	CBoatWindow() { }
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update() {};
	virtual void Render();
	virtual void SetState() {};
};
