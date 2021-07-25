#pragma once
#include "GameObject.h"
#include <math.h> 

#define SLOPE_STATE_UP 100
#define SLOPE_STATE_BACK 200

#define BRICK_SPEED 0.004f
#define BRICK_SPEED_UP 0.004f


class CSlopeBrick : public CGameObject
{
private:
	float slope_width;
	float slope_height;
	float ratio_hw;
	double length;
	float brick_speed;
public:
	CSlopeBrick(float Width, float Height)
	{
		this->slope_width = Width;
		this->slope_height = Height; 
		this->ratio_hw = Height / Width;
		brick_speed = ratio_hw * BRICK_SPEED;
	};
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Collision(LPGAMEOBJECT object, float dy, float dx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void SetState(int state);
	
};

