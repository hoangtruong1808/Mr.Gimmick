#pragma once
#include "GameObject.h"
#include "Brick.h"
#include "PlayScence.h"
#include "Game.h"
#include "Gimmick.h"

#define ORANGEBOSS_GRAVITY			0.0005f

#define ORANGEBOSS_STATE_DIE				0
#define ORANGEBOSS_STATE_WALK			1
#define ORANGEBOSS_STATE_ATTACK			2
#define ORANGEBOSS_STATE_STUN			3

#define ORANGEBOSS_ANI_IDLE_LEFT					0
#define ORANGEBOSS_ANI_IDLE_RIGHT				1
#define ORANGEBOSS_ANI_WALK_LEFT					2
#define ORANGEBOSS_ANI_WALK_RIGHT				3
#define ORANGEBOSS_ANI_ATTACK_LEFT					4
#define ORANGEBOSS_ANI_ATTACK_RIGHT				5
#define ORANGEBOSS_ANI_STUN_LEFT					6
#define ORANGEBOSS_ANI_STUN_RIGHT				7
#define ORANGEBOSS_ANI_DIE_LEFT					8
#define ORANGEBOSS_ANI_DIE_RIGHT					9

#define ORANGEBOSS_BBOX_WIDTH				32
#define	ORANGEBOSS_BBOX_HEIGHT				32
#define ORANGEBOSS_SWORD_BBOX_WIDTH			10

#define ORANGEBOSS_PLAYER_DISTANCE					80 //khoang cach giua walk va player
#define ORANGEBOSS_PLAYER_CANJUMP_DISTANCE			48 // khoang cach walk nhay
#define ORANGEBOSS_PLAYER_CRAZY_CANJUMP_DISTANCE			80 // khoang cach walk nhay khi crazy
#define ORANGEBOSS_BLOCK_RANGE						30

#define ORANGEBOSS_PLAYER_ATTACK_DISTANCE				20 

#define ORANGEBOSS_ACCELERATION				0.00035f
#define ORANGEBOSS_WALK_SPEED				0.1f
#define	ORANGEBOSS_JUMP_SPEED				0.26f
#define ORANGEBOSS_JUMP_SPEED_X				0.15f

#define ORANGEBOSS_DEFLECT_SPEED_X			0.05f
#define ORANGEBOSS_DEFLECT_SPEED_Y			0.07f

#define ORANGEBOSS_ATTACK_TIME						700
#define ORANGEBOSS_TIME_BETWEEN_AUTOATTACKS		3000 

#define ORANGEBOSS_MAP_END_RIGHT				2016
#define ORANGEBOSS_MAP_END_LEFT				1816


class CSwordBoss : public CGameObject
{
public:
	float ax = 0;
	bool onGround = false;
	bool render_onground = false;

	int life_counts = 5;

	bool is_stunning = false;
	ULONGLONG stun_start = 0;

	bool can_attack = true;
	ULONGLONG canattack_start = 0;

	bool is_attacking = false;
	ULONGLONG attack_start = 0;
	ULONGLONG autoattack_timing = 0;

	bool crazy = false;

	CSwordBoss();
	~CSwordBoss() {}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();
	virtual void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Jump();
	void CalculateSpeed();
	bool isEqual(float a, float b, float equal = 1.0f);
	void CalculateState();
	void CreateSword();
};

