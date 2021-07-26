#include "OrangeBoss.h"
#include "Utils.h"
#include "MagicStar.h"

COrangeBoss::COrangeBoss()
{
	nx = -1;
	SetState(ORANGEBOSS_STATE_WALK);
}

void COrangeBoss::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CGimmick* player = ((CPlayScene*)scene)->GetPlayer();

	CGameObject::Update(dt, coObjects);

	vx += ax * dt;
	vy -= ORANGEBOSS_GRAVITY * dt;

	if (vx != 0)
	{
		if (x + ORANGEBOSS_BBOX_WIDTH >= ORANGEBOSS_MAP_END_RIGHT && ax > 0)
		{
			vx = 0;
		}
		if (x <= ORANGEBOSS_MAP_END_LEFT && ax < 0)
		{
			vx = 0;
		}
	}

	if (life_counts == 3) // dinh 2 vien dan la boss se crazy
	{
		crazy = true;
	}
	else 
	{
		crazy = false;
	}
	if (life_counts == 1)
	{
		SetState(ORANGEBOSS_STATE_DIE);
	}
	if (player->x + GIMMICK_BBOX_WIDTH / 2 < x + ORANGEBOSS_BBOX_WIDTH / 2)
		nx = -1;
	else
		nx = 1;

	CalculateSpeed();
	CalculateState();

	onGround = false;
	render_onground = false;

	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		if (dynamic_cast<CGimmick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != ORANGEBOSS_STATE_DIE)
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
			if (e->ny > 0) onGround = true;

			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				x = x0 + min_tx * dx + nx * 0.1f;
				y = y0 + min_ty * dy + ny * 0.1f;
				if (e->nx != 0)
				{
					vx = 0;
				}
				if (e->ny > 0) {
					vy = 0;
					onGround = true;
				}
			}
			if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				gimmick->StartUntouchable();
			}
		}
	}
	// clean up newCoObjects
	for (UINT i = 0; i < newCoObjects.size(); i++) newCoObjects[i] = nullptr;
	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void COrangeBoss::Render()
{
	int ani = 0;
	if (state == ORANGEBOSS_STATE_WALK) {
		if (vx == 0)
		{
			if (nx > 0)
				ani = ORANGEBOSS_ANI_IDLE_RIGHT;
			else
				ani = ORANGEBOSS_ANI_IDLE_LEFT;
		}
		else
		{
			if (nx > 0)
				ani = ORANGEBOSS_ANI_WALK_RIGHT;
			else
				ani = ORANGEBOSS_ANI_WALK_LEFT;
		}
	}
	else if (state == ORANGEBOSS_STATE_ATTACK)
	{
		if (nx > 0)
		{
			ani = ORANGEBOSS_ANI_ATTACK_RIGHT;
			animation_set->at(ani)->Render(x - 13, y - 2);
			return;
		}
		else
		{
			ani = ORANGEBOSS_ANI_ATTACK_LEFT;
			animation_set->at(ani)->Render(x - 13, y - 2);
			return;
		}
	}
	else if (state == ORANGEBOSS_STATE_STUN)
	{
		if (nx > 0)
		{
			ani = ORANGEBOSS_ANI_STUN_RIGHT;
		}
		else
		{
			ani = ORANGEBOSS_ANI_STUN_LEFT;
		}
	}
	else if (state == ORANGEBOSS_STATE_DIE)
	{
		if (nx > 0)
		{
			ani = ORANGEBOSS_ANI_DIE_RIGHT;
		}
		else
		{
			ani = ORANGEBOSS_ANI_DIE_LEFT;
		}
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void COrangeBoss::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case ORANGEBOSS_STATE_WALK:
		break;
	case ORANGEBOSS_STATE_ATTACK:
		attack_start = GetTickCount64();
		break;
	case ORANGEBOSS_STATE_STUN:
		stun_start = GetTickCount64();
		ax = 0;
		if (nx < 0) {
			vx = ORANGEBOSS_DEFLECT_SPEED_X;
		}
		else {
			vx = -ORANGEBOSS_DEFLECT_SPEED_X;
		}
		vy = ORANGEBOSS_DEFLECT_SPEED_Y;
		break;
	case ORANGEBOSS_STATE_DIE:
		CScene* scene = CGame::GetInstance()->GetCurrentScene();
		ax = 0;
		if (nx < 0) {
			vx = ORANGEBOSS_DEFLECT_SPEED_X;
		}
		else {
			vx = -ORANGEBOSS_DEFLECT_SPEED_X;
		}
		vy = -ORANGEBOSS_DEFLECT_SPEED_Y;
		break;

	}
}

void COrangeBoss::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + ORANGEBOSS_BBOX_WIDTH;
	b = y - ORANGEBOSS_BBOX_HEIGHT;
}

void COrangeBoss::Jump()
{
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CGimmick* player = ((CPlayScene*)scene)->GetPlayer();

	if (nx < 0)
	{
		vx = -ORANGEBOSS_JUMP_SPEED_X;
		ax = -ORANGEBOSS_ACCELERATION;
	}
	else
	{
		vx = ORANGEBOSS_JUMP_SPEED_X;
		ax = ORANGEBOSS_ACCELERATION;
	}
	vy = ORANGEBOSS_JUMP_SPEED;
}

void COrangeBoss::CalculateSpeed()
{
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	CGimmick* player = ((CPlayScene*)scene)->GetPlayer();

	float player_boss_jump_distance = 0;

	if (crazy) player_boss_jump_distance = ORANGEBOSS_PLAYER_CRAZY_CANJUMP_DISTANCE; // nhay lien tuc hon
	else player_boss_jump_distance = ORANGEBOSS_PLAYER_CANJUMP_DISTANCE;

	// ===== WALK:
	// Gioi han toc do:
	if (state == ORANGEBOSS_STATE_WALK || state == ORANGEBOSS_STATE_ATTACK)
	{
		if (vx > ORANGEBOSS_WALK_SPEED)
			vx = ORANGEBOSS_WALK_SPEED;
		if (vx < -ORANGEBOSS_WALK_SPEED)
			vx = -ORANGEBOSS_WALK_SPEED;
	}

	//duoi theo gimmick:
	// WALK
	if (state == ORANGEBOSS_STATE_WALK && onGround)
	{
		if (nx < 0)
		{
			if (abs(player->x + ORANGEBOSS_PLAYER_DISTANCE - x) <= 1.0f)
			{
				vx = 0;
				ax = 0;
			}
			else if (player->x + ORANGEBOSS_PLAYER_DISTANCE < x)
				ax = -ORANGEBOSS_ACCELERATION;
			else if (player->x + ORANGEBOSS_PLAYER_DISTANCE > x)
				ax = ORANGEBOSS_ACCELERATION;
		}
		else
		{
			if (abs(player->x - ORANGEBOSS_PLAYER_DISTANCE - x) <= 1.0f)
			{
				vx = 0;
				ax = 0;
			}
			else if (player->x - ORANGEBOSS_PLAYER_DISTANCE > x)
				ax = ORANGEBOSS_ACCELERATION;
			else if (player->x - ORANGEBOSS_PLAYER_DISTANCE < x)
				ax = -ORANGEBOSS_ACCELERATION;
		}
	}
	// duoi theo player
	if (state == ORANGEBOSS_STATE_ATTACK && onGround)
	{
		if (nx < 0)
		{
			if (player->x + ORANGEBOSS_PLAYER_DISTANCE == x)
			{
				vx = 0;
				ax = 0;
			}
			else if (player->x + ORANGEBOSS_PLAYER_ATTACK_DISTANCE < x)
				ax = -ORANGEBOSS_ACCELERATION;
			else if (player->x + ORANGEBOSS_PLAYER_ATTACK_DISTANCE > x)
				ax = ORANGEBOSS_ACCELERATION;
		}
		else
		{
			if (player->x + ORANGEBOSS_PLAYER_DISTANCE == x)
			{
				vx = 0;
				ax = 0;
			}
			else if (player->x - ORANGEBOSS_PLAYER_ATTACK_DISTANCE > x)
				ax = ORANGEBOSS_ACCELERATION;
			else if (player->x - ORANGEBOSS_PLAYER_ATTACK_DISTANCE < x)
				ax = -ORANGEBOSS_ACCELERATION;
		}
	}

	if (state == ORANGEBOSS_STATE_WALK && onGround)
	{
		if (nx < 0)
		{
			if (player->x + player_boss_jump_distance >=x)
			{
				if (x + ORANGEBOSS_BBOX_WIDTH >= ORANGEBOSS_MAP_END_RIGHT - 2) 
					Jump();
				else
				{
					if (!crazy)
					{
						if (can_attack)
							SetState(ORANGEBOSS_STATE_ATTACK);
					}
					else
						Jump();
				}
			}
		}
		else
		{
			if (player->x - player_boss_jump_distance <= x)
			{
				if (x <= ORANGEBOSS_MAP_END_LEFT + 2)
					Jump();
				else
				{
					if (!crazy)
					{
						if (can_attack)
							SetState(ORANGEBOSS_STATE_ATTACK);
					}
					else
						Jump();
				}
			}
		}
	}
}


void COrangeBoss::CalculateState()
{
	// tinh thoi gian tan cogn
	if (state == ORANGEBOSS_STATE_ATTACK && GetTickCount64() - attack_start >= ORANGEBOSS_ATTACK_TIME)
	{
		SetState(ORANGEBOSS_STATE_WALK);
		attack_start = 0;
		if (can_attack) {
			can_attack = false;
			canattack_start = GetTickCount64();
		}
	}

	if (state == ORANGEBOSS_STATE_WALK && GetTickCount64() - autoattack_timing >= ORANGEBOSS_TIME_BETWEEN_AUTOATTACKS) 
	{
		SetState(ORANGEBOSS_STATE_ATTACK);
		autoattack_timing = GetTickCount64();
	}

	if (state == ORANGEBOSS_STATE_STUN && GetTickCount64() - stun_start >= 300)
	{
		SetState(ORANGEBOSS_STATE_WALK);
		stun_start = 0;
	}
}



