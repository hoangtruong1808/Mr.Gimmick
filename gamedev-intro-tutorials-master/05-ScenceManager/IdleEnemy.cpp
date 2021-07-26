#include "IdleEnemy.h"
#include "Brick.h"
#include "Utils.h"
#include "Gimmick.h"
#include "Game.h"

#include "PlayScence.h"


CIdleEnemy::CIdleEnemy() : CGameObject()
{

	SetState(IDLEENEMY_STATE_IDLE);
}


void CIdleEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (state != IDLEENEMY_STATE_IDLE)
		vy -= IDLEENEMY_GRAVITY * dt;
	if (this->state == IDLEENEMY_STATE_DIE)
		return;
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	// turn off collision when die 
	if (state != IDLEENEMY_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		float x0 = x;
		float y0 = y;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBrick*>(e->obj))
			{
				y = y0 + min_ty * dy + ny * 0.1f;
				if (e->ny != 0)
				{
					vy = 0;
				}
			}
			x = x0 + min_tx * dx + nx * 0.1f;
		}
	}


	//// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CIdleEnemy::Render()
{

	int ani = -1;
	if (state == IDLEENEMY_STATE_DIE)
	{
		return;
	}
	else
	{
		ani = IDLEENEMY_ANI_IDLE;
	}
	animation_set->at(ani)->Render(x, y);
}

void CIdleEnemy::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case IDLEENEMY_ANI_IDLE:
		vx = 0;
		vy = 0;
		break;
	case IDLEENEMY_STATE_DIE:
		vx = -1/10;
		vy = 0.08f;
		break;
	}
}

void CIdleEnemy::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + IDLEENEMY_BBOX_WIDTH;
	b = t - IDLEENEMY_BBOX_HEIGHT;
}