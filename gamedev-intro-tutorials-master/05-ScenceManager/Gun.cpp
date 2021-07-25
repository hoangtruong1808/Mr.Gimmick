#include "Gun.h"
#include "Gimmick.h"
#include "Brick.h"
#include "SlopeBrick.h"
#include "Bullet.h"
#include "PlayScence.h"
#include "Fire.h"

void CGun::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + GUN_BBOX_WIDTH;
	bottom = y - GUN_BBOX_HEIGHT;
}

void CGun::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += GUN_GRAVITY * dt;
	CGameObject::Update(dt);
	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		else if (dynamic_cast<CPlatformsMoving*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));

		if (dynamic_cast<CSlopeBrick*>(coObjects->at(i))) {
			CSlopeBrick* brick = dynamic_cast<CSlopeBrick*>(coObjects->at(i));
			brick->Collision(this, dy, dx);
		}

		/*if (dynamic_cast<CPlatformsMoving*>(coObjects->at(i))) {
			CPlatformsMoving* brick = dynamic_cast<CPlatformsMoving*>(coObjects->at(i));
			brick->Collision(this, dy, dx, dt);
		}*/
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

		float y0, x0;
		x0 = x;
		y0 = y;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0)
				{
					vx = 0;
					x = x0 + min_tx * dx + nx * 0.2f;
				}
				if (e->ny != 0)
				{
					vy = 0;
					y = y0 + min_ty * dy + ny * 0.2f;

				}
			}

		}
	}
	Attack();
}

void CGun::Render()
{
	int alpha = 255;
	animation_set->at(0)->Render(x, y, alpha);
}

void CGun::Attack()
{
	time -=	dt;
	if (time <= 0)
	{
		time = GUN_ATTACK_TIME;
		CBullet* bullet = new CBullet();
		bullet->SetPosition(x + 16, y);
		bullet->SetSpeed(0.075, 0);
		LPANIMATION_SET ani = CAnimationSets::GetInstance()->Get(170001);
		bullet->SetAnimationSet(ani);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(bullet);
	}
}

