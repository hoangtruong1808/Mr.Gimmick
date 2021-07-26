#include "BlackBomb.h"
#include "Brick.h"
#include "Game.h"
#include "Gimmick.h"
#include "PlayScence.h"

CBlackBomb::CBlackBomb() : CGameObject()
{
	alive = 1;
	alive_start = GetTickCount64();
	explosive = 0;
	explosive_start = NULL;
}

void CBlackBomb::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BLACKBOMB_BBOX_WIDTH;
	bottom = top - BLACKBOMB_BBOX_HEIGHT;
}

void CBlackBomb::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	CScene* scene = CGame::GetInstance()->GetCurrentScene();

	CGimmick* player = ((CPlayScene*)scene)->GetPlayer();


	if (this->state == BLACKBOMB_STATE_EXPLOSIVE) {
		if (explosive == 1 && GetTickCount64() - explosive_start > BLACKBOMB_EXPLOSIVE_TIME)
		{
			explosive = 0;
			explosive_start = NULL;
			this->SetState(BLACKBOMB_STATE_DIE);
		}
	}
	else {
		if (alive == 1 && GetTickCount64() - alive_start > BLACKBOMB_ALIVE_TIME)
		{
			alive = 0;
			alive_start = NULL;
			this->SetState(BLACKBOMB_STATE_EXPLOSIVE);
		}

		// Calculate dx, dy 
		CGameObject::Update(dt);

		// Simple fall down
		vy -= BLACKBOMB_GRAVITY * dt;

		float x0 = x;
		float y0 = y;

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		CalcPotentialCollisions(coObjects, coEvents);

		if (coEvents.size() == 0)
		{
			x = x0 + dx;
			y = y0 + dy;
		}
		else
		{
			float min_tx, min_ty, nx = 0, ny = 0;
			float rdx = 0;
			float rdy = 0;

			x = x0 + dx;
			y = y0 + dy;

			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{
				LPCOLLISIONEVENT e = coEventsResult[i];

				if (dynamic_cast<CBrick*>(e->obj))
				{
					y = y0 + min_ty * dy + ny * 0.1f;
					x = x0 + min_tx * dx + nx * 0.1f;

					if (e->ny != 0) {
						vy = -0.5 * vy;
					}

					if (e->nx > 0 && this->state == BLACKBOMB_STATE_WALKING_LEFT)
						this->SetState(BLACKBOMB_STATE_WALKING_RIGHT);
					if (e->nx < 0 && this->state == BLACKBOMB_STATE_WALKING_RIGHT)
						this->SetState(BLACKBOMB_STATE_WALKING_LEFT);
				}
			}
		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
}

void CBlackBomb::Render()
{
	if (state == BLACKBOMB_STATE_DIE)
		return;

	int alpha = 255;
	int ani = -1;
	switch (this->state)
	{
	case BLACKBOMB_STATE_WALKING_LEFT:
		ani = BLACKBOMB_ANI_WALKING;
		break;
	case BLACKBOMB_STATE_WALKING_RIGHT:
		ani = BLACKBOMB_ANI_WALKING;
		break;
	case BLACKBOMB_STATE_EXPLOSIVE:
		ani = BLACKBOMB_ANI_EXPLOSIVE;
		break;
	default:
		break;
	}

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CBlackBomb::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BLACKBOMB_STATE_WALKING_RIGHT:
		this->vx = BLACKBOMB_WALKING_SPEED;
		break;
	case BLACKBOMB_STATE_WALKING_LEFT:
		this->vx = -BLACKBOMB_WALKING_SPEED;
		break;
	case BLACKBOMB_STATE_EXPLOSIVE:
		this->vx = 0;
		this->vy = 0;
		if (explosive == 0) {
			explosive = 1;
			explosive_start = GetTickCount64();
		}
		break;
	}
}