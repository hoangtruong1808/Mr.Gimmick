#include "FallenBomb.h"
#include "PlayScence.h"
#include "Gimmick.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"
#include "Brick.h"
#include "SlopeBrick.h"


CFallenBomb::CFallenBomb()
{
	SetState(FALLENBOMB_STATE_IDLE);
}

void CFallenBomb::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + FALLENBOMB_BBOX_WIDTH;
	bottom = y - FALLENBOMB_BBOX_HEIGHT;
}

void CFallenBomb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == FALLENBOMB_STATE_DIE)
		return;

	CGameObject::Update(dt);
	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		if (dynamic_cast<CSlopeBrick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		if (dynamic_cast<CGimmick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&newCoObjects, coEvents);

	Attack();
	Die();
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;



		float y0, x0;
		x0 = x;
		y0 = y;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];

			if (e->ny != 0)
			{
				SetState(FALLENBOMB_STATE_EXPLODE);
			}
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				if (e->ny != 0)
				{
					SetState(FALLENBOMB_STATE_EXPLODE);
				}
			}
			if (dynamic_cast<CSlopeBrick*>(e->obj))
			{
				CSlopeBrick* slope_brick = dynamic_cast<CSlopeBrick*>(e->obj);
				if (e->ny != 0)
				{
					SetState(FALLENBOMB_STATE_EXPLODE);
				}
			}
			if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				gimmick->StartUntouchable();
			}
		}
	}
	for (UINT i = 0; i < newCoObjects.size(); i++)
		newCoObjects[i] = nullptr;
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CFallenBomb::Render()
{
	int ani = -1;


	if (state == FALLENBOMB_STATE_DIE)
		return;
	else if (state == FALLENBOMB_STATE_IDLE)
		ani = FALLENBOMB_ANI_IDLE;
	else if (state == FALLENBOMB_STATE_FALL)
		ani = FALLENBOMB_ANI_FALL;
	else if (state == FALLENBOMB_STATE_EXPLODE)
		ani = FALLENBOMB_ANI_EXPLODE;


	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CFallenBomb::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FALLENBOMB_STATE_FALL:
		vy = FALLENBOMB_GRAVITY;
		break;
	case FALLENBOMB_STATE_EXPLODE:
		vy = 0;
		break;
	}
}

void CFallenBomb::Attack()
{
	CGimmick* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (state == FALLENBOMB_STATE_IDLE)
	{
		if (player->y < this->y)
		{
			if (player->x + GIMMICK_BBOX_WIDTH >= this->x && player->x <= this->x + FALLENBOMB_BBOX_WIDTH)
			{
				SetState(FALLENBOMB_STATE_FALL);
			}
		}
	}
}

void CFallenBomb::Die()
{
	if (state == FALLENBOMB_STATE_EXPLODE)
	{
		if (explode_start == 0)
		{
			StartExplode();
		}
		if (GetTickCount() - explode_start >= FALLENBOMB_EXPLODE_TIME)
		{
			SetState(FALLENBOMB_STATE_DIE);
		}
	}
}
