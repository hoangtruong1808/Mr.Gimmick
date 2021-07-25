#include "Bird.h"
#include "Game.h"
#include "PlayScence.h"
#include "Gimmick.h"


CBird::CBird(int direction, float end)
{
	this->direction = direction;
	this->end = end;
	vx = direction * BIRD_VX;
	state = BIRD_STATE_IDLE;
	onBird = false;
}

void CBird::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGimmick* player = ((CPlayScene*)(CGame::GetInstance()->GetCurrentScene()))->GetPlayer();

	if (state == BIRD_STATE_IDLE)
	{
		if (start == 0)
		{
			Start();
		}
		if (GetTickCount() - start >= 4000)
		{
			state = BIRD_STATE_FLY;
		}
	}
	if (state == BIRD_STATE_FLY)
	{
		vx = BIRD_VX * direction;
		sinX += dt;
		vy = BIRD_VY * 2 * sin(3.14 * sinX / 5 / 180);

		CGameObject::Update(dt);
		x += dx;
		y += dy;

		if (onBird == true)
		{
			player->x += dx;
			player->y += dy;
			onBird == false;
		}
		if (onBird == false)
		{
			player->x += 0;
			player->y += 0;
		}
		vy = 0;
		state = BIRD_STATE_FLY;
	}
	if (FlyHigher())
	{
		vx = BIRD_VX * direction * 1.5;
		vy = BIRD_VY * 5;

		CGameObject::Update(dt);
		x += dx;
		y += dy;

		state = BIRD_STATE_FLYHIGHER;
	}
}


void CBird::Render()
{
	int ani = -1;
	if (direction == 1)
	{
		if (state == BIRD_STATE_IDLE)
		{
			ani = BIRD_ANI_IDLE_RIGHT;
			animation_set->at(ani)->Render(x, y + 27, 255);
		}
		else
		{
			ani = BIRD_ANI_FLY_RIGHT;
			animation_set->at(ani)->Render(x - 5, y + 15, 255);
		}
	}
	if (direction == -1)
	{
		if (state == BIRD_STATE_IDLE)
		{
			ani = BIRD_ANI_IDLE_LEFT;
			animation_set->at(ani)->Render(x, y + 27, 255);
		}
		else
		{
			ani = BIRD_ANI_FLY_LEFT;
			animation_set->at(ani)->Render(x, y + 15, 255);
		}
	}
}


void CBird::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = l + BIRD_BBOX_WIDTH;
	b = t - BIRD_BBOX_HEIGHT;
}


void CBird::isPlayerOn(bool onbird)
{
	onBird = onbird;
}

bool CBird::FlyHigher()
{
	if (direction > 0)
	{
		if (x >= end)
		{
			return true;
		}
		else return false;
	}
	else if (direction < 0)
	{
		if (x <= end)
		{
			return true;
		}
		else return false;
	}
	else return false;
}