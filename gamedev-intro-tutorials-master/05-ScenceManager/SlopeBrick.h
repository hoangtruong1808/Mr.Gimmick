#pragma once
#include "GameObject.h"
#include <math.h> 

#define SLOPE_STATE_UP 100
#define SLOPE_STATE_BACK 200

#define BRICK_SPEED 0.05f


class CSlopeBrick : public CGameObject
{
private:
	float slope_width;
	float slope_height;
	float ratio_hw;
	double length;
public:
	CSlopeBrick(float Width, float Height)
	{
		this->slope_width = Width;
		this->slope_height = Height; 
		this->ratio_hw = Height / Width;
	};
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Collision(LPGAMEOBJECT object, float dy, float dx);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	void SetState(int state);
	
};

