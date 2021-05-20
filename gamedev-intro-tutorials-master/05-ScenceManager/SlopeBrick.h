#pragma once
#include "GameObject.h"
#include <math.h> 

#define BRICK_STATE_UP 100
#define BRICK_STATE_BACK 200

#define BRICK_SPEED 0.05f

typedef struct Slope
{
	float start_x, start_y;
	float end_x, end_y;
	float d_x, d_y;
};

class CSlopeBrick : public CGameObject
{
private:
	float slope_width;
	float slope_height;
	double length;
	Slope slope;
public:
	CSlopeBrick(float Width, float Height, Slope slope)
	{
		this->slope_width = Width;
		this->slope_height = Height; 
		this->slope = slope;
		this->length = sqrt(slope.d_x * slope.d_x + slope.d_y * slope.d_y);
	};
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	//virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetState(int state);
	
};

