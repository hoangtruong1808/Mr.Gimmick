#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16

#define BRICK_STATE_UP 100
#define BRICK_STATE_BACK 200

#define BRICK_SPEED 0.05f


class CBrick : public CGameObject
{
private: 
	float brick_speed;
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	float GetBrickSpeed() { return brick_speed; }
};