#pragma once
#include "GameObject.h"

#define MAGICSTAR_GRAVITY			-0.0005f

#define MAGICSTAR_STATE_SMALL		100
#define MAGICSTAR_STATE_SMALL_1		150
#define MAGICSTAR_STATE_BIG			200
#define MAGICSTAR_STATE_BIG_RUN		300
#define MAGICSTAR_STATE_DIE			400
#define MAGICSTAR_STATE_DIE1		500


#define MAGICSTAR_ANI_SMALL			0
#define	MAGICSTAR_ANI_SMALL_1		1
#define MAGICSTAR_ANI_BIG			2
#define MAGICSTAR_ANI_DIE			3


#define MAGICSTAR_BBOX_SMALL_WIDTH		8
#define MAGICSTAR_BBOX_SMALL_HEIGHT		8
#define MAGICSTAR_BBOX_SMALL1_WIDTH		26
#define MAGICSTAR_BBOX_SMALL1_HEIGHT	27
#define MAGICSTAR_BBOX_BIG_WIDTH		16
#define MAGICSTAR_BBOX_BIG_HEIGHT		16

#define MAGICSTAR_DIE_TIME				3000

class CMagicStar : public CGameObject
{
	DWORD rally_start;
	D3DXVECTOR2 star1, star2, star3, star4, star5;
	float star_len, star_vy;
	DWORD time_die;
public:
	CMagicStar();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};


