#include "BlackEnemy.h"
#include "Gimmick.h"
#include "Brick.h"
#include "Camera.h"
#include "SlopeBrick.h"



CBlackEnemy::CBlackEnemy() : CGameObject()
{
	SetState(BLACKENEMY_STATE_WALKING_LEFT);
}



void CBlackEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += BLACKENEMY_GRAVITY * dt;


	CGameObject::Update(dt);

	if (state == BLACKENEMY_STATE_DIE)
	{
		y = y + dy;
		return;
	}

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i)))
		{
			CBrick* brick = dynamic_cast<CBrick*>(coObjects->at(i));
			brick->Collision(this, dy, dx);
		}
		else
			if (dynamic_cast<CSlopeBrick*>(coObjects->at(i))) {
				CSlopeBrick* brick = dynamic_cast<CSlopeBrick*>(coObjects->at(i));
				brick->Collision(this, dy, dx);
			}
	}

	//CGimmick* gimmick = CGimmick::GetInstance();
	float cx, cy;
	//gimmick->GetPosition(cx, cy);
	CCamera* camera = CCamera::GetInstance();
	camera->GetPlayerPos(cx, cy);

	if (bol_fly == false)
	{
		if (cy > y)
		{
			if (start_change_fly == 0)
				start_change_fly = GetTickCount();
			else if (GetTickCount() - start_change_fly > BLACKENEMY_CHANGE_FLY_TIME)
			{
				bol_fly = true;
			}
		}
		else start_change_fly = 0;
	}


	switch (state)
	{
	case BLACKENEMY_STATE_WALKING_RIGHT: Update_BLACKENEMY_STATE_WALKING_RIGHT(dt, coObjects, cx, cy); break;
	case BLACKENEMY_STATE_WALKING_LEFT: Update_BLACKENEMY_STATE_WALKING_LEFT(dt, coObjects, cx, cy); break;
	case BLACKENEMY_STATE_FLY_RIGHT:	Update_BLACKENEMY_STATE_FLY_RIGHT(dt, coObjects, cx, cy); break;
	case BLACKENEMY_STATE_FLY_LEFT:		Update_BLACKENEMY_STATE_FLY_LEFT(dt, coObjects, cx, cy); break;
		//case BLACKENEMY_STATE_WALKING_FLY_RIGHT:	Update_BLACKENEMY_STATE_WALKING_FLY_RIGHT(dt, coObjects, cx, cy); break;
		//case BLACKENEMY_STATE_WALKING_FLY_LEFT:	Update_BLACKENEMY_STATE_WALKING_FLY_LEFT(dt, coObjects, cx, cy); break;
	case BLACKENEMY_STATE_FLY_SETTING_RIGHT:	Update_BLACKENEMY_STATE_FLY_SETTING_RIGHT(dt, coObjects, cx, cy); break;
	case BLACKENEMY_STATE_FLY_SETTING_LEFT:		Update_BLACKENEMY_STATE_FLY_SETTING_LEFT(dt, coObjects, cx, cy); break;
	}
}

void CBlackEnemy::Render()
{
	int ani = -1;
	switch (state)
	{
	case BLACKENEMY_STATE_WALKING_RIGHT:
		ani = BLACKENEMY_ANI_WALKING_RIGHT;
		break;
	case BLACKENEMY_STATE_WALKING_LEFT:
		ani = BLACKENEMY_ANI_WALKING_LEFT;
		break;
	case BLACKENEMY_STATE_FLY_RIGHT:
		ani = BLACKENEMY_ANI_FLY_RIGHT;
		break;
	case BLACKENEMY_STATE_FLY_LEFT:
		ani = BLACKENEMY_ANI_FLY_LEFT;
		break;
	case BLACKENEMY_STATE_FLY_SETTING_RIGHT:
		ani = BLACKENEMY_ANI_FLY_SETTING_RIGHT;
		break;
	case BLACKENEMY_STATE_FLY_SETTING_LEFT:
		ani = BLACKENEMY_ANI_FLY_SETTING_LEFT;
		break;
	case BLACKENEMY_STATE_WALKING_FLY_RIGHT:
		ani = BLACKENEMY_ANI_WALKING_FLY_RIGHT;
		break;
	case BLACKENEMY_STATE_WALKING_FLY_LEFT:
		ani = BLACKENEMY_ANI_WALKING_FLY_LEFT;
		break;
	case BLACKENEMY_STATE_DIE:
		ani = BLACKENEMY_ANI_DIE;
		break;
	}
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CBlackEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BLACKENEMY_BBOX_WIDTH;
	bottom = y - BLACKENEMY_BBOX_HEIGHT;
}

void CBlackEnemy::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BLACKENEMY_STATE_WALKING_RIGHT:
		nx = 1;
		break;
	case BLACKENEMY_STATE_WALKING_LEFT:
		nx = -1;
		vx = -BLACKENEMY_WALKING_SPEED;
		break;
	case BLACKENEMY_STATE_FLY_SETTING_RIGHT:
		start_change_state = GetTickCount();
		break;
	case BLACKENEMY_STATE_FLY_SETTING_LEFT:
		start_change_state = GetTickCount();
		break;
	case BLACKENEMY_STATE_FLY_RIGHT:
		nx = 1;
		vx = 0;
		break;
	case BLACKENEMY_STATE_FLY_LEFT:
		nx = -1;
		vx = 0;
		break;
	}

	start_change_state = 0;
}

void CBlackEnemy::Update_BLACKENEMY_STATE_WALKING_RIGHT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy)
{


	if (x > cx)
	{
		if (start_change_state == 0)
			start_change_state = GetTickCount();
		else
		{
			if (GetTickCount() - start_change_state > BLACKENEMY_CHANGE_STATE_TIME)
			{
				SetState(BLACKENEMY_STATE_WALKING_LEFT);
			}
		}
	}

	vx = BLACKENEMY_WALKING_SPEED;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(objects, coEvents);

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		float y0, x0;
		x0 = x;
		y0 = y;

		if (nx == 0)
			x = x + dx;
		if (ny == 0)
			y = y + dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];

			//Brick
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				float l, r, t, b;
				brick->GetBoundingBox(l, t, r, b);
				if (e->ny != 0)
				{
					vy = 0;

					if (e->ny > 0)
					{
						if (bol_fly == true)
							SetState(BLACKENEMY_STATE_FLY_SETTING_RIGHT);
						else {
							if (brick->GetState() != NULL)
								x += brick->GetBrickSpeed() * dt;
							if (((x > l - 18 && x < l + 2) || (x > r - 18 && x < r + 2)))
								vy = 0.2f;
						}
					}
					y = y0 + min_ty * dy + ny * 0.1f;


				}
				else
				{
					if (e->nx != 0)
					{
						if (brick->GetState() == NULL)
						{
							x = x0 + min_tx * dx + nx * 0.1f;
						}
					}
				}

			}
			else if (dynamic_cast<CSlopeBrick*>(e->obj)) {
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
			else if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				if (e->ny != -1)
					gimmick->StartUntouchable();
			}
			else if (dynamic_cast<CBlackEnemy*>(e->obj)) {
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
		}
	}
}

void CBlackEnemy::Update_BLACKENEMY_STATE_WALKING_LEFT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy)
{


	if (x < cx)
	{
		if (start_change_state == 0)
			start_change_state = GetTickCount();
		else
		{
			if (GetTickCount() - start_change_state > BLACKENEMY_CHANGE_STATE_TIME)
			{
				SetState(BLACKENEMY_STATE_WALKING_RIGHT);
			}
		}
	}

	vx = -BLACKENEMY_WALKING_SPEED;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(objects, coEvents);

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		float y0, x0;
		x0 = x;
		y0 = y;

		if (nx == 0)
			x = x + dx;
		if (ny == 0)
			y = y + dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];

			//Brick
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				float l, r, t, b;
				brick->GetBoundingBox(l, t, r, b);
				if (e->ny != 0)
				{
					vy = 0;
					if (e->ny > 0)
					{
						if (bol_fly == true)
							SetState(BLACKENEMY_STATE_FLY_SETTING_LEFT);
						else
						{
							if (brick->GetState() != NULL)
								x += brick->GetBrickSpeed() * dt;
							if ((x > l - 2 && x < l + 2) || (x > r - 2 && x < r + 2))
								vy = 0.2f;
						}
					}
					y = y0 + min_ty * dy + ny * 0.1f;


				}
				else
				{
					if (e->nx != 0)
					{
						if (brick->GetState() == NULL)
						{
							x = x0 + min_tx * dx + nx * 0.1f;
						}
					}
				}

			}
			else if (dynamic_cast<CSlopeBrick*>(e->obj)) {
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
			else if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				if (e->ny != -1)
					gimmick->StartUntouchable();
			}
			else if (dynamic_cast<CBlackEnemy*>(e->obj)) {
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
		}
	}
}

void CBlackEnemy::Update_BLACKENEMY_STATE_FLY_RIGHT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy)
{


	if (x > cx)
	{
		if (start_change_state == 0)
			start_change_state = GetTickCount();
		else
		{
			if (GetTickCount() - start_change_state > BLACKENEMY_CHANGE_STATE_TIME)
			{
				start_change_state = 0;
				SetState(BLACKENEMY_STATE_FLY_LEFT);

			}
		}
		if (vx >= 0)
			vx = vx - 0.001f;
	}
	else
	{
		vx = vx + 0.001f;
		if (y > cy)
		{
			if (cy - y > 32)
				vy = -0.1f;
		}
		else vy = 0.1f;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(objects, coEvents);

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		float y0, x0;
		x0 = x;
		y0 = y;

		if (nx == 0)
			x = x + dx;
		if (ny == 0)
			y = y + dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];

			//Brick
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				float l, r, t, b;
				brick->GetBoundingBox(l, t, r, b);
				if (e->ny != 0)
				{
					//vy = 0;

					if (e->ny > 0)
					{

						if (brick->GetState() != NULL)
							x += brick->GetBrickSpeed() * dt;

					}
					y = y0 + min_ty * dy + ny * 0.1f;


				}
				else
				{
					if (e->nx != 0)
					{
						if (brick->GetState() == NULL)
						{
							x = x0 + min_tx * dx + nx * 0.1f;
						}
					}
				}

			}
			else if (dynamic_cast<CSlopeBrick*>(e->obj)) {
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
			else if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				if (e->ny != -1)
					gimmick->StartUntouchable();
			}
			else if (dynamic_cast<CBlackEnemy*>(e->obj)) {
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
		}
	}
}

void CBlackEnemy::Update_BLACKENEMY_STATE_FLY_LEFT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy)
{


	if (x < cx)
	{
		if (start_change_state == 0)
			start_change_state = GetTickCount();
		else
		{
			if (GetTickCount() - start_change_state > BLACKENEMY_CHANGE_STATE_TIME)
			{
				start_change_state = 0;
				SetState(BLACKENEMY_STATE_FLY_RIGHT);
			}
		}
		if (vx <= 0)
			vx = vx + 0.001f;
	}
	else
	{
		if (vx > -0.1f)
			vx = vx - 0.002f;
		else vx = -0.1f;
		if (y > cy)
		{
			vy = -0.1f;
		}
		else vy = 0.1f;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	CalcPotentialCollisions(objects, coEvents);

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

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		float y0, x0;
		x0 = x;
		y0 = y;

		if (nx == 0)
			x = x + dx;
		if (ny == 0)
			y = y + dy;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];

			//Brick
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);
				float l, r, t, b;
				brick->GetBoundingBox(l, t, r, b);
				if (e->ny != 0)
				{
					//vy = 0;

					if (e->ny > 0)
					{

						if (brick->GetState() != NULL)
							x += brick->GetBrickSpeed() * dt;

					}
					y = y0 + min_ty * dy + ny * 0.1f;


				}
				else
				{
					if (e->nx != 0)
					{
						if (brick->GetState() == NULL)
						{
							x = x0 + min_tx * dx + nx * 0.1f;
						}
					}
				}

			}
			else if (dynamic_cast<CSlopeBrick*>(e->obj)) {
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
			else if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				if (e->ny != -1)
					gimmick->StartUntouchable();
			}
			else if (dynamic_cast<CBlackEnemy*>(e->obj)) {
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;
			}
		}
	}
}

void CBlackEnemy::Update_BLACKENEMY_STATE_FLY_SETTING_RIGHT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy)
{
	if (GetTickCount() - start_change_state > 1250)
	{
		SetState(BLACKENEMY_STATE_FLY_RIGHT);
		start_change_state = 0;
	}
}

void CBlackEnemy::Update_BLACKENEMY_STATE_FLY_SETTING_LEFT(DWORD dt, vector<LPGAMEOBJECT>* objects, float cx, float cy)
{
	if (GetTickCount() - start_change_state > 1250)
	{
		SetState(BLACKENEMY_STATE_FLY_LEFT);
		start_change_state = 0;
	}
}