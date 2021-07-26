#include "Worm.h"
#include "Utils.h"
#include "Gimmick.h"
#include "Brick.h"

CWorm::CWorm() : CGameObject()
{
	SetState(WORM_STATE_WALKING_LEFT);
}

void CWorm::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;
	y += dy;
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;


	coEvents.clear();
	CalcPotentialCollisions(coObjects, coEvents);
	if (coEvents.size() == 0)
	{
		if (x < start)
		{
			x = start;
			vx = WORM_SPEED;
		}
		else if (x > end)
		{
			x = end;
			vx = -WORM_SPEED;
		}
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
			if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				gimmick->StartUntouchable();
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CWorm::Render()
{
	int ani = -1;
	if (vx > 0)
	{
		ani = WORM_ANI_WALKING_RIGHT;
	}
	else
	{
		ani = WORM_ANI_WALKING_LEFT;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CWorm::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WORM_BBOX_WIDTH;
	b = y - WORM_BBOX_HEIGHT;
}

void CWorm::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case WORM_STATE_WALKING_RIGHT:
		vx = WORM_SPEED;
		nx = 1;
		break;
	case WORM_STATE_WALKING_LEFT:
		vx = -WORM_SPEED;
		nx = -1;
		break;
	case WORM_STATE_DIE:
		vy = -0.15;
		vx = 0;
		nx = 0;
		break;
	}
}