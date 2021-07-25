#include "Cannon.h"
#include "Gimmick.h"
#include "Brick.h"
#include "Bullet.h"
#include "PlayScence.h"
#include "CannonBullet.h"

CCannon::CCannon()
{
	SetState(CANNON_STATE_IDLE);
}

void CCannon::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + CANNON_BBOX_WIDTH;
	bottom = y - CANNON_BBOX_HEIGHT;
}

void CCannon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += CANNON_GRAVITY * dt;
	CGameObject::Update(dt);

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

void CCannon::Render()
{
	int ani = -1;
	int alpha = 255;
	switch (state)
	{
	case CANNON_STATE_IDLE:
		ani = CANNON_ANI_IDLE;
		break;
	case CANNON_STATE_MOVE:
		ani = CANNON_ANI_MOVE;
		break;
	default:
		ani = CANNON_ANI_IDLE;
		break;
	}
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
}
void CCannon::SetState(int state) 
{

}
void CCannon::Attack()
{
	time -= dt;
	if (time <= 0)
	{
		time = CANNON_ATTACK_TIME;
		CCannonBullet* bullet = new CCannonBullet();
		bullet->SetPosition(x-15 , y);
		bullet->SetSpeed(-0.1f, 0);
		LPANIMATION_SET ani = CAnimationSets::GetInstance()->Get(350001);
		bullet->SetAnimationSet(ani);
		((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(bullet);
	}
}

