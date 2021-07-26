#include "BlackBoss.h"
#include "Utils.h"
#include "BlackEnemy.h"

CBlackBoss::CBlackBoss()
{
	SetState(BLACKBOSS_STATE_IDLE);
}

void CBlackBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	vx += ax * dt;
	vy -= BLACKBOSS_GRAVITY * dt;
	if (state == BLACKBOSS_STATE_STUN)
	{
		if (ax > 0 && vx >= 0)
			SetState(BLACKBOSS_STATE_BLINK);
	}
	CalculateState();

	CalculateShooting();
	if (life_count == 1)
	{
		SetState(BLACKBOSS_STATE_DIE);
	}
	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != BLACKBOSS_STATE_DIE)
		CalcPotentialCollisions(&newCoObjects, coEvents);

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

		float x0 = x;
		float y0 = y;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		float l, t, r, b;
		GetBoundingBox(l, t, r, b);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			float ol, ot, or , ob;
			e->obj->GetBoundingBox(ol, ot, or , ob);


			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				x = x0 + min_tx * dx + nx * 0.1f;
				y = y0 + min_ty * dy + ny * 0.1f;
				if (e->nx != 0)
				{
				}
				if (e->ny != 0)
				{
					vy = 0;
				}
			}
			else if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				if (e->ny != -1)
					gimmick->StartUntouchable();
			}

		}
	}
	// clean up newCoObjects
	for (UINT i = 0; i < newCoObjects.size(); i++) newCoObjects[i] = nullptr;
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBlackBoss::Render()
{
	int ani = 0;
	if (state == BLACKBOSS_STATE_IDLE) { 
		ani = BLACKBOSS_ANI_IDLE;
		RenderEars();
	}
	else if (state == BLACKBOSS_STATE_SHOOTING) {
		ani = BLACKBOSS_ANI_SHOOTING;
		animation_set->at(ani)->Render(x, y + BLACKBOSS_SHOOTING_EARS_HEIGHT);
		return;
	}
	else if (state == BLACKBOSS_STATE_STUN || state == BLACKBOSS_STATE_DIE) {
		ani = BLACKBOSS_ANI_STUN;
		animation_set->at(ani)->Render(x, y + BLACKBOSS_STUN_EARS_HEIGHT);
		return;
	}
	else if (state == BLACKBOSS_STATE_BLINK) {
		ani = BLACKBOSS_ANI_BLINK;
		RenderEars();
	}
	else if (state == BLACKBOSS_STATE_WALK) {
		ani = BLACKBOSS_ANI_WALK;
		RenderEars();
	}

	animation_set->at(ani)->Render(x, y);

}

void CBlackBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BLACKBOSS_STATE_IDLE:
		ax = 0;
		vx = 0;
		idle_time = GetTickCount64();
		break;
	case BLACKBOSS_STATE_STUN:
		vx = -BLACKBOSS_DEFLECT_SPEED;
		ax = BLACKBOSS_BLACKENEMY_ACCELERATION;
		break;
	case BLACKBOSS_STATE_BLINK:
		vx = 0;
		ax = 0;
		blink_time = GetTickCount64();
		break;
	case BLACKBOSS_STATE_WALK:
		vx = BLACKBOSS_WALK_SPEED;
		ax = 0;
		break;
	case BLACKBOSS_STATE_DIE:
		ax = 0;
		vy = -BLACKBOSS_DEFLECT_SPEED;
		vx = -BLACKBOSS_DEFLECT_SPEED;
		break;
	}

}

void CBlackBoss::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + BLACKBOSS_BBOX_WIDTH;
	b = y - BLACKBOSS_BBOX_HEIGHT;
}

void CBlackBoss::RenderEars()
{
	animation_set->at(BLACKBOSS_EAR_ANI)->Render(x + (BLACKBOSS_BBOX_WIDTH - BLACKBOSS_EARS_WIDTH) / 2, y + BLACKBOSS_EARS_HEIGHT);
}

void CBlackBoss::CalculateState()
{
	if (state == BLACKBOSS_STATE_IDLE && GetTickCount64() - idle_time >= BLACKBOSS_IDLE_TIME) {
		idle_time = 0;
		SetState(BLACKBOSS_STATE_SHOOTING);
		if (shots_count == 0) FirstShoot();

	}

	if (state == BLACKBOSS_STATE_BLINK && GetTickCount64() - blink_time >= BLACKBOSS_BLINK_TIME) {
		blink_time = 0;
		SetState(BLACKBOSS_STATE_WALK);
	}

	if (state == BLACKBOSS_STATE_WALK && x >= BLACKBOSS_STOP_POSITION_X) {
		SetState(BLACKBOSS_STATE_IDLE);
	}
	if (state != BLACKBOSS_STATE_DIE && life_count <= 0)
	{
		SetState(BLACKBOSS_STATE_DIE);
	}
}

void CBlackBoss::Shoot(float init_vx, float init_vy)
{
	CBlackEnemy* enemy = new CBlackEnemy();
	//Vi tri cho quai
	enemy->SetPosition(x + (BLACKBOSS_BBOX_WIDTH - BLACKENEMY_BBOX_WIDTH) / 2, y + BLACKBOSS_SHOOTING_EARS_HEIGHT);

	//Animation set cho quai
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(50100);
	enemy->SetAnimationSet(ani_set);

	//them vao list object cua playscene
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	((CPlayScene*)scene)->AddObject(enemy);
	//Bay 1 doan
	enemy->vx = init_vx;
	enemy->vy = init_vy;
	enemy->nx = 1;

	//enemy->SetState(BLACKENEMY_STATE_BULLET);
}

void CBlackBoss::FirstShoot()
{
	Shoot(0.1, 0.07f);
	Shoot(0.4, 0.1f);
	Shoot(0.7f, 0.13f);
	Shoot(1.0f, 0.16f);
	Shoot(1.3f, 0.19f);
	Shoot(1.7f, 0.22f);
	Shoot(2.0f, 0.25f);

	//Bat dau ban tung vien
	shots_count += 1;
	shoot_time = GetTickCount64();
}

void CBlackBoss::CalculateShooting()
{
	if (state == BLACKBOSS_STATE_SHOOTING && shots_count != 0 && GetTickCount64() - shoot_time >= BLACKBOSS_SHOOT_TIME) {
		if (shots_count % 3 == 1) //tam gan
		{
			Shoot(-0.1, 0.07f);
			shots_count += 1;
			shoot_time = GetTickCount64();
		}
		else if (shots_count % 3 == 2) //tam trung
		{
			Shoot(-1.0f, 0.16f);
			shots_count += 1;
			shoot_time = GetTickCount64();
		}
		else if (shots_count % 3 == 0) //tam xa
		{
			Shoot(2.0f, 0.25f);
			shots_count += 1;
			shoot_time = GetTickCount64();
		}
	}
}

