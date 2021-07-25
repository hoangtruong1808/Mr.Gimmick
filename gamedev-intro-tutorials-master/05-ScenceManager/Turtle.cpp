#include "Turtle.h"
#include "Utils.h"
#include "Gimmick.h"
#include "MagicStar.h"
#include "Brick.h"

CTurtle::CTurtle() : CGameObject()
{
	SetState(TURTLE_STATE_WALKING_LEFT);
}


void CTurtle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	r = x + TURTLE_BBOX_WIDTH;
	if (state == TURTLE_STATE_WALKING_LEFT || state == TURTLE_STATE_WALKING_RIGHT)
	{
		t = y;
		b = y - TURTLE_BBOX_HEIGHT;
	}
	else
	{
		t = y;
		b = y - TURTLE_BBOX_HEIGHT;
	}
}

void CTurtle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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
			SetState(TURTLE_STATE_WALKING_RIGHT);
		}
		else if (x > end)
		{
			x = end;
			SetState(TURTLE_STATE_WALKING_LEFT);
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
			if (dynamic_cast<CBrick*>(e->obj))
			{
				y = y0 + min_ty * dy + ny * 0.1f;
				if (e->ny != 0)
				{
					vy = 0;
				}
			}
			x = x0 + min_tx * dx + nx * 0.1f;
			if (dynamic_cast<CMagicStar*>(e->obj))
			{
				SetState(TURTLE_STATE_DIE_RIGHT);
			}
		}
	}
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CTurtle::Render()
{
	int ani = -1;
	switch (state)
	{
	case TURTLE_STATE_WALKING_RIGHT:
		ani = TURTLE_ANI_WALKING_RIGHT;
		break;
	case TURTLE_STATE_WALKING_LEFT:
		ani = TURTLE_ANI_WALKING_LEFT;
		break;
	case TURTLE_STATE_DIE_RIGHT:
		ani = TURTLE_ANI_DIE_RIGHT;
		break;
	case TURTLE_STATE_DIE_LEFT:
		ani = TURTLE_ANI_DIE_LEFT;
		break;
	}
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}


void CTurtle::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case TURTLE_STATE_WALKING_RIGHT:
		vx = TURTLE_SPEED;
		nx = 1;
		break;
	case TURTLE_STATE_WALKING_LEFT:
		vx = -TURTLE_SPEED;
		nx = -1;
		break;
	case TURTLE_STATE_DIE_RIGHT:
		vx = 0;
		nx = 1;
		break;
	case TURTLE_STATE_DIE_LEFT:
		vx = 0;
		nx = -1;
		break;
	}
}