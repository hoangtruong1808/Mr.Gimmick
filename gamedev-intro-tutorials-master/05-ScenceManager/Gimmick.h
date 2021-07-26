#pragma once
#include "GameObject.h"
#include "MagicStar.h"


#define GIMMICK_WALKING_SPEED		0.1f 
#define GIMMICK_JUMP_SPEED			0.3f
#define GIMMICK_GRAVITY				-0.0006f
#define GIMMICK_INERTIA				0.004f
#define GIMMICK_DIE_DEFLECT_SPEED	 0.5f

#define GIMMICK_STATE_IDLE			0
#define GIMMICK_STATE_WALKING_RIGHT	100
#define GIMMICK_STATE_WALKING_LEFT	200
#define GIMMICK_STATE_JUMP			300
#define GIMMICK_STATE_DIE			400
#define GIMMICK_STATE_JUMP_RIGHT	500
#define GIMMICK_STATE_JUMP_LEFT		600
#define GIMMICK_STATE_STUNNED		700


#define GIMMICK_ANI_IDLE_RIGHT		0
#define GIMMICK_ANI_IDLE_LEFT		1
#define GIMMICK_ANI_WALKING_RIGHT	2
#define GIMMICK_ANI_WALKING_LEFT	3
#define GIMMICK_ANI_JUMP_RIGHT		4
#define GIMMICK_ANI_JUMP_LEFT		5

#define GIMMICK_ANI_STUNNED_RIGHT	6
#define GIMMICK_ANI_STUNNED_LEFT	7
#define GIMMICK_ANI_DIE				8

#define GIMMICK_BBOX_WIDTH  15
#define GIMMICK_BBOX_HEIGHT 15

#define GIMMICK_BBOX_JUMP_WIDTH  15
#define GIMMICK_BBOX_JUMP_HEIGHT 18

#define GIMMICK_UNTOUCHABLE_TIME 5000
#define GIMMICK_REVIVAL_TIME		2000


class CGimmick : public CGameObject
{
	int lever;
	int untouchable;
	DWORD untouchable_start;
	DWORD die_start;
	bool bol_star = false;
public:
	CMagicStar* magic_star;

	bool bol_jump = false;
	bool inSewer = false;// check in sewer
	bool equalinSewer = false; // check in sewer3
	float XSewer, YSewer, nSewer; // check out sewer 
	CGimmick();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	void StartUntouchable();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);


	void UpdateSpeed_IDLE(float& vx, float& vy, int nx);
	void UpdateSpeed_WALKING_RIGHT(float& vx, float& vy);
	void UpdateSpeed_WALKING_LEFT(float& vx, float& vy);
	void UpdateSpeed_JUMP(float& vx, float& vy, int nx);
	void UpdateSpeed_JUMP_RIGHT(float& vx, float& vy);
	void UpdateSpeed_JUMP_LEFT(float& vx, float& vy);

	void JUMP();
	void STOP_JUMP();

	void MAGICSTAR();
	void STOP_MAGICSTAR();
	void Revival();


};