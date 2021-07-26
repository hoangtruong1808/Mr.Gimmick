#include "Bullet.h"
#include "Gimmick.h"
#include "Gun.h"
#include "Brick.h"
#include "SlopeBrick.h"
#include "Fire.h"

CBullet::CBullet()
{
	SetState(BULLET_ANI_MOVE);
}

void CBullet::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BULLET_BOX_WIDTH;
	b = t - BULLET_BOX_HEIGHT;
}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += BULLET_GRAVITY * dt;

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		else if (dynamic_cast<CFire*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		if (dynamic_cast<CSlopeBrick*>(coObjects->at(i))) {
			CSlopeBrick* brick = dynamic_cast<CSlopeBrick*>(coObjects->at(i));
			brick->Collision(this, dy, dx);
		}
		else
			if (dynamic_cast<CGimmick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));

	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(&newCoObjects, coEvents);

	Die();
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx, ny;
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
				x = x0 + min_tx * dx + nx * 0.1f;
				y = y0 + min_ty * dy + ny * 0.1f;
				
			}
			if (dynamic_cast<CFire*>(e->obj))
			{
				SetState(BULLET_STATE_EXPLODE);
			}
			if (dynamic_cast<CGimmick*>(e->obj))
			{
				SetState(BULLET_STATE_EXPLODE);
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
void CBullet::Render()
{
	int ani = -1;
	if (state == BULLET_STATE_DIE)
		return;
	else if (state == BULLET_STATE_MOVE)
		ani = BULLET_ANI_MOVE;
	else if(state == BULLET_STATE_EXPLODE)
		ani = BULLET_ANI_EXPLODE;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);

}

void CBullet::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BULLET_STATE_MOVE:
		nx = 1;
		break;
	case BULLET_STATE_EXPLODE:
		vx = 0;
		break;
	}
}
void CBullet::Die()
{
	if (state == BULLET_STATE_EXPLODE)
	{
		if (explode_start == 0)
		{
			StartExplode();
		}
		if (GetTickCount() - explode_start >= BULLET_EXPLODE_TIME)
		{
			SetState(BULLET_STATE_DIE);
		}
	}
}

