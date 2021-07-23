 #pragma once
#include "GameObject.h"
#include "MagicStar.h"


#define GIMMICK_WALKING_SPEED		0.1f 
#define GIMMICK_JUMP_SPEED			0.25f
#define GIMMICK_GRAVITY				-0.0005f
#define GIMMICK_INERTIA				0.004f
#define GIMMICK_DIE_DEFLECT_SPEED	 0.5f

#define GIMMICK_STATE_IDLE			0
#define GIMMICK_STATE_WALKING_RIGHT	100
#define GIMMICK_STATE_WALKING_LEFT	200
#define GIMMICK_STATE_JUMP			300
#define GIMMICK_STATE_DIE			400
#define GIMMICK_STATE_JUMP_RIGHT	500
#define GIMMICK_STATE_JUMP_LEFT		600

#define GIMMICK_ANI_IDLE_RIGHT		0
#define GIMMICK_ANI_IDLE_LEFT		1
#define GIMMICK_ANI_WALKING_RIGHT	2
#define GIMMICK_ANI_WALKING_LEFT	3
#define GIMMICK_ANI_JUMP_RIGHT		4
#define GIMMICK_ANI_JUMP_LEFT		5

#define GIMMICK_ANI_DIE				6

#define GIMMICK_BBOX_WIDTH  15
#define GIMMICK_BBOX_HEIGHT 15

#define GIMMICK_BBOX_JUMP_WIDTH  15
#define GIMMICK_BBOX_JUMP_HEIGHT 20

#define GIMMICK_UNTOUCHABLE_TIME 5000


class CGimmick : public CGameObject
{
	

	CMagicStar* magic_star;
	int untouchable;
	DWORD untouchable_start;
	bool bol_jump = false;
	bool bol_star = false;
public:
	CGimmick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	void UpdateSpeed_IDLE(float &vx, float &vy, int nx);
	void UpdateSpeed_WALKING_RIGHT(float& vx, float& vy);
	void UpdateSpeed_WALKING_LEFT(float& vx, float& vy);
	void UpdateSpeed_JUMP(float& vx, float& vy, int nx);
	void UpdateSpeed_JUMP_RIGHT(float& vx, float& vy);
	void UpdateSpeed_JUMP_LEFT(float& vx, float& vy);

	void JUMP();
	void STOP_JUMP();

	void MAGICSTAR();
	void STOP_MAGICSTAR();

	

};