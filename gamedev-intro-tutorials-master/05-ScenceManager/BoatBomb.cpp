#include "BoatBomb.h"
#include "PlayScence.h"
#include "Gimmick.h"
#include "Utils.h"
#include "Game.h"
#include "Water.h"


CBoatBomb::CBoatBomb()
{
	SetState(BOATBOMB_STATE_IDLE);
}

void CBoatBomb::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOATBOMB_BBOX_WIDTH;
	bottom = y - BOATBOMB_BBOX_HEIGHT;
}

void CBoatBomb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CWater*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
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

			//Brick
			if (dynamic_cast<CWater*>(e->obj))
			{
				CWater* water = dynamic_cast<CWater*>(e->obj);
				if (e->ny != 0)
				{
					SetState(BOATBOMB_STATE_EXPLODE);
				}
			}

		}
	}
	for (UINT i = 0; i < newCoObjects.size(); i++)
		newCoObjects[i] = nullptr;
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}

void CBoatBomb::Render()
{
	int ani = -1;


	if (state == BOATBOMB_STATE_DIE)
		return;
	else if (state == BOATBOMB_STATE_IDLE)
		ani = BOATBOMB_ANI_FALL;
	else if (state == BOATBOMB_STATE_LIFT)
		ani = BOATBOMB_ANI_FALL;
	else if (state == BOATBOMB_STATE_FALL)
		ani = BOATBOMB_ANI_FALL;
	else if (state == BOATBOMB_STATE_EXPLODE)
		ani = BOATBOMB_ANI_EXPLODE;

	animation_set->at(ani)->Render(x, y);
}

void CBoatBomb::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BOATBOMB_STATE_LIFT:
		vy = BOATBOMB_LIFT_SPEED;
		break;
	case BOATBOMB_STATE_FALL:
		vy = BOATBOMB_GRAVITY;
		break;
	case BOATBOMB_STATE_EXPLODE:
		vy = 0;
		break;
	}
}

void CBoatBomb::Attack()
{
	CGimmick* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (state == BOATBOMB_STATE_IDLE)
	{
		if (player->y < this->y)
		{
			if (player->x + GIMMICK_BBOX_WIDTH >= this->x - 16)
			{
				SetState(BOATBOMB_STATE_LIFT);
			}
		}
	}
	if (state == BOATBOMB_STATE_LIFT && this->y >= 136)
	{
		SetState(BOATBOMB_STATE_FALL);
	}
}

void CBoatBomb::Die()
{
	if (state == BOATBOMB_STATE_EXPLODE)
	{
		if (explode_start == 0)
		{
			StartExplode();
		}
		if (GetTickCount() - explode_start >= BOATBOMB_EXPLODE_TIME)
		{
			SetState(BOATBOMB_STATE_DIE);
		}
	}
}