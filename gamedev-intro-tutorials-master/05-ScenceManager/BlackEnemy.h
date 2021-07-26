#pragma once
#include "GameObject.h"


#define BLACKENEMY_GRAVITY				-0.0005f
#define BLACKENEMY_WALKING_SPEED			0.06f 

#define BLACKENEMY_STATE_DIE				0
#define BLACKENEMY_STATE_WALKING_RIGHT		100
#define BLACKENEMY_STATE_WALKING_LEFT		200
#define BLACKENEMY_STATE_FLY_RIGHT			300
#define BLACKENEMY_STATE_FLY_LEFT			400
#define BLACKENEMY_STATE_WALKING_FLY_RIGHT	500
#define BLACKENEMY_STATE_WALKING_FLY_LEFT	600
#define BLACKENEMY_STATE_FLY_SETTING_RIGHT	700
#define BLACKENEMY_STATE_FLY_SETTING_LEFT	800

#define BLACKENEMY_ANI_WALKING_RIGHT		0
#define BLACKENEMY_ANI_WALKING_LEFT			1
#define BLACKENEMY_ANI_FLY_RIGHT			2
#define BLACKENEMY_ANI_FLY_LEFT				3
#define BLACKENEMY_ANI_WALKING_FLY_RIGHT	4
#define BLACKENEMY_ANI_WALKING_FLY_LEFT		5
#define BLACKENEMY_ANI_FLY_SETTING_RIGHT	6
#define BLACKENEMY_ANI_FLY_SETTING_LEFT		7
#define BLACKENEMY_ANI_DIE					8


#define BLACKENEMY_BBOX_WIDTH	16
#define BLACKENEMY_BBOX_HEIGHT	16

#define BLACKENEMY_CHANGE_STATE_TIME 1000
#define BLACKENEMY_CHANGE_FLY_TIME 10000

class CBlackEnemy : public CGameObject
{
	bool bol_fly = false;
	DWORD start_change_state = 0;
	DWORD start_change_fly = 0;
public:
	CBlackEnemy();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update_BLACKENEMY_STATE_WALKING_RIGHT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy);
	void Update_BLACKENEMY_STATE_WALKING_LEFT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy);
	void Update_BLACKENEMY_STATE_FLY_RIGHT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy);
	void Update_BLACKENEMY_STATE_FLY_LEFT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy);
	void Update_BLACKENEMY_STATE_FLY_SETTING_RIGHT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy);
	void Update_BLACKENEMY_STATE_FLY_SETTING_LEFT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy);
	//void Update_BLACKENEMY_STATE_WALKING_FLY_RIGHT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy);
	//void Update_BLACKENEMY_STATE_WALKING_FLY_LEFT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy);

};


