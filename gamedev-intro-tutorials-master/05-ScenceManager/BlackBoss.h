#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "PlayScence.h"
#include "Game.h"
#include "Gimmick.h"

#define BLACKBOSS_GRAVITY	0.001f

#define BLACKBOSS_STATE_DIE			0
#define BLACKBOSS_STATE_IDLE		1
#define BLACKBOSS_STATE_SHOOTING	2
#define BLACKBOSS_STATE_STUN		3
#define BLACKBOSS_STATE_BLINK		4
#define BLACKBOSS_STATE_WALK		5

#define BLACKBOSS_BBOX_WIDTH				32
#define BLACKBOSS_BBOX_HEIGHT				31
#define BLACKBOSS_EARS_WIDTH				20
#define BLACKBOSS_EARS_HEIGHT				13
#define BLACKBOSS_SHOOTING_EARS_HEIGHT		16
#define BLACKBOSS_STUN_EARS_HEIGHT			8

#define BLACKBOSS_ANI_IDLE			0
#define BLACKBOSS_EAR_ANI			1
#define BLACKBOSS_ANI_SHOOTING		2
#define BLACKBOSS_ANI_STUN			3
#define BLACKBOSS_ANI_BLINK			4
#define BLACKBOSS_ANI_WALK			5

#define BLACKBOSS_DEFLECT_SPEED					0.09f
#define BLACKBOSS_BLACKENEMY_ACCELERATION		0.0002f
#define BLACKBOSS_WALK_SPEED					0.02f

#define BLACKBOSS_STOP_POSITION_X				80

#define BLACKBOSS_IDLE_TIME			2000
#define BLACKBOSS_SHOOT_TIME		1500
#define BLACKBOSS_BLINK_TIME		450


class CBlackBoss : public CGameObject
{
public:
	float ax = 0;
	ULONGLONG idle_time = 0;

	ULONGLONG shoot_time = 0;
	int shots_count = 0;

	ULONGLONG blink_time = 0;

	int life_count = 4;

	CBlackBoss();
	~CBlackBoss() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	bool onTopOf(CGameObject* object);
	void RenderEars();
	void CalculateState();
	void Shoot(float init_vx, float init_vy);
	void FirstShoot();
	void CalculateShooting();
	void CalculateSpeed();
	void Injure() { life_count -= 1; };
};

