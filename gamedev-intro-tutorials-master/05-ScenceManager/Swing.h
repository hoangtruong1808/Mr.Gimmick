#pragma once
#include "GameObject.h"

#define SWING_BBOX_WIDTH	36
#define SWING_BBOX_HEIGHT	68

#define SWING_ANI_IDLE 0
#define SWING_ANI_MOVE 1
#define SWING_ANI_OPEN 2
#define SWING_ANI_STOP 3

#define SWING_STATE_IDLE 0
#define SWING_STATE_MOVE 1
#define SWING_STATE_OPEN 2
#define SWING_STATE_STOP 3

#define SWING_STOP_POSITION 1362
#define	SWING_SPEED		0.02
class CSwing : public CGameObject
{
public:
	CSwing();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
};

