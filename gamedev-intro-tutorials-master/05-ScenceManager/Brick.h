#pragma once
#include "GameObject.h"

#define BRICK_STATE_UP 100
#define BRICK_STATE_BACK 200

#define BRICK_SPEED 0.05f


class CBrick : public CGameObject
{
private: 
	float brick_width;
	float brick_height;
	float brick_speed;
public:
	CBrick(float BrickWidth, float BrickHeight) { this->brick_width = BrickWidth; this->brick_height = BrickHeight; }
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	float GetBrickSpeed() { return brick_speed; }
	void Collision(LPGAMEOBJECT object, float dy, float dx);
};