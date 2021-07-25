#include "CannonBullet.h"
#include "Gimmick.h"
#include "Gun.h"
#include "Brick.h"
#include "SlopeBrick.h"
#include "Fire.h"
#include "PlayScence.h"

CCannonBullet::CCannonBullet()
{
	SetState(CANNONBULLET_STATE_MOVE);
}

void CCannonBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + CANNONBULLET_BOX_WIDTH;
	bottom = top - CANNONBULLET_BOX_HEIGHT;
}

void CCannonBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));

	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CalcPotentialCollisions(&newCoObjects, coEvents);

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
				if (e->nx != 0)
				{
					SetState(CANNONBULLET_STATE_IMPACT);
					vx = 0.05f;
					vy = CANNONBULLET_GRAVITY * dt;
				}
			}
		}
	}
	Die();
	for (UINT i = 0; i < newCoObjects.size(); i++)
		newCoObjects[i] = nullptr;
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}
void CCannonBullet::Render()
{
	int ani = -1;
	int alpha = 255;

	if (state == CANNONBULLET_STATE_DIE)
		return;
	else ani = CANNONBULLET_ANI_MOVE;
	
	animation_set->at(ani)->Render(x, y, alpha);

}

void CCannonBullet::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case CANNONBULLET_STATE_MOVE:
		nx = -1;
		break;
	case CANNONBULLET_STATE_IMPACT:
		nx = 1;
		break;
	}
}
void CCannonBullet::Die()
{
	if (this->y <= CANNONBULLET_DIE_Y)
	{
		SetState(CANNONBULLET_STATE_DIE);
	}
}
void CCannonBullet::Collision()
{
	if (state == CANNONBULLET_STATE_MOVE)
	{
		CGimmick* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		float pl, pt, pr, pb;
		player->GetBoundingBox(pl, pt, pr, pb);
		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		if (pr >= l && pl <= r)
		{
			player->x += dx;
			if (!player->bol_jump)
			{
				player->y = y + 16;
				player->vy = 0;
			}
		}
	}
}

