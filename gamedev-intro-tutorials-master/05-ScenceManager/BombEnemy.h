#pragma once
#include "GameObject.h"

#define	BOMBENEMY_BBOX_WIDTH			16
#define	BOMBENEMY_BBOX_HEIGHT			32

#define	BOMBENEMY_GRAVITY				0.0005f
#define BOMBENEMY_WALK_SPEED			0.05f
#define BOMBENEMY_DIE_SPEED				0.075f

#define	BOMBENEMY_STATE_WALKING_RIGHT	100
#define	BOMBENEMY_STATE_WALKING_LEFT	200
#define	BOMBENEMY_STATE_DIE_RIGHT		300
#define	BOMBENEMY_STATE_DIE_LEFT		400

#define BOMBENEMY_ANI_WALKING_RIGHT		0
#define BOMBENEMY_ANI_WALKING_LEFT		1
#define BOMBENEMY_ANI_DIE_RIGHT			2
#define BOMBENEMY_ANI_DIE_LEFT			3

class CBombEnemy : public CGameObject
{
	float left;
	float right;
public:
	CBombEnemy(float left, float right);
	void Shot();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

