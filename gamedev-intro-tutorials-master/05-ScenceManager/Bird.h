#pragma once
#include "GameObject.h"
#include <math.h> 

#define BIRD_STATE_IDLE		1	
#define BIRD_STATE_FLY	2
#define BIRD_STATE_FLYHIGHER		3

#define BIRD_ANI_IDLE_RIGHT		0	
#define BIRD_ANI_IDLE_LEFT		1
#define BIRD_ANI_FLY_RIGHT	2
#define BIRD_ANI_FLY_LEFT	3

#define BIRD_BBOX_WIDTH	48
#define BIRD_BBOX_HEIGHT	16

#define BIRD_VX			0.03f
#define BIRD_VY			0.01f

class CBird : public CGameObject
{
	float end;
	int direction = 1;		// Huong ->> 1,  <-- -1
	bool onBird;
	DWORD start = 0;
	int sinX = 0;
public:
	CBird() {};
	CBird(int direction, float end);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

	void isPlayerOn(bool a);
	bool FlyHigher();
	void Start() { start = GetTickCount(); }
};

