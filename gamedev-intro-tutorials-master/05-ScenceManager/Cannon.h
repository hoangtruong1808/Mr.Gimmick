#pragma once
#include "GameObject.h"

#define CANNON_GRAVITY				-0.001f
#define CANNON_ATTACK_TIME	5000


#define CANNON_BBOX_WIDTH	24
#define CANNON_BBOX_HEIGHT	29

#define CANNON_STATE_IDLE	0
#define CANNON_STATE_MOVE	1

#define CANNON_ANI_IDLE 0
#define CANNON_ANI_MOVE 1

class CCannon : public CGameObject
{
private:
	int time = 0;
public:
	CCannon();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void Attack();
};