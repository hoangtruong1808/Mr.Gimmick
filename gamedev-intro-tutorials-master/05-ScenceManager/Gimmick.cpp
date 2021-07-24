#include <iostream>
#include <fstream>

#include "Gimmick.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"
#include "Brick.h"
#include "Portal.h"
#include "PlatformsMoving.h"
#include "SlopeBrick.h"
#include "Swing.h"


CGimmick::CGimmick() : CGameObject()
{
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);
}


void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += GIMMICK_GRAVITY * dt;


	switch (state)
	{
	case GIMMICK_STATE_IDLE: UpdateSpeed_IDLE(vx, vy, nx); break;
	case GIMMICK_STATE_WALKING_RIGHT: UpdateSpeed_WALKING_RIGHT(vx, vy); break;
	case GIMMICK_STATE_WALKING_LEFT: UpdateSpeed_WALKING_LEFT(vx, vy); break;
	case GIMMICK_STATE_JUMP: UpdateSpeed_JUMP(vx, vy, nx); break;
	case GIMMICK_STATE_JUMP_LEFT: UpdateSpeed_JUMP_LEFT(vx, vy); break;
	case GIMMICK_STATE_JUMP_RIGHT: UpdateSpeed_JUMP_RIGHT(vx, vy); break;
	}

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CBrick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		else if (dynamic_cast<CPlatformsMoving*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		else if (dynamic_cast<CSwing*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));

		if (dynamic_cast<CSlopeBrick*>(coObjects->at(i))) {
			newCoObjects.push_back(coObjects->at(i));
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

	// turn off collision when die 
	if (state != GIMMICK_STATE_DIE)
		CalcPotentialCollisions(&newCoObjects, coEvents);



	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > GIMMICK_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	// No collision occured, proceed normally
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

		/*if (nx != 0)
			x = x0 + min_tx * dx + nx * 0.4f;
		else x = x + dx;
		if (ny != 0)
			y = y0 + min_ty * dy + ny * 0.4f;
		else y = y + dy;
		*/
		if (nx == 0)
			x = x + dx;
		if (ny == 0)
			y = y + dy;

		if (ny > 0)
		{
			if (state == GIMMICK_STATE_JUMP || state == GIMMICK_STATE_JUMP_RIGHT || state == GIMMICK_STATE_JUMP_LEFT)
			{
				state = GIMMICK_STATE_IDLE;

				if (this->nx > 0)
				{
					if (vx >= 0.05)
						vx = 0.1f;
					else if (vx > 0.01f)
						vx = 0.07f;
				}
				else
				{
					if (vx <= -0.05)
						vx = -0.1f;
					else if (vx < -0.01f)
						vx = -0.07f;
				}
			}
		}

		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];

			//Brick
			if (dynamic_cast<CBrick*>(e->obj))
			{
				CBrick* brick = dynamic_cast<CBrick*>(e->obj);

				if (e->ny != 0)
				{
					vy = 0;
					if (e->ny > 0)
						if (brick->GetState() != NULL)
							x += brick->GetBrickSpeed() * dt;
					y = y0 + min_ty * dy + ny * 0.2f;
				}
				else
				{
					if (e->nx != 0)
					{
						if (brick->GetState() == NULL)
						{
							vx = 0;
							x = x0 + min_tx * dx + nx * 0.2f;
						}

					}
				}

			}
			else if (dynamic_cast<CPlatformsMoving*>(e->obj))
			{
				CPlatformsMoving* pm = dynamic_cast<CPlatformsMoving*>(e->obj);
				if (e->nx != 0)
				{
					vx = 0;
					x = x0 + min_tx * dx + nx * 0.2f;

				}
				else if (e->ny != 0)
				{
					vy = -0.0015;
					if (e->ny > 0)
					{
						if (pm->GetState() == PM_STATE_WIDTH)
						{
							y = y0 + min_ty * dy + ny * 0.2f;
							x += pm->GetPosition_dx();
							//pm->Collision(this, this->dy, this->dx, dt);
						}
						else
						{
							if (pm->GetPosition_dy() < 0)
							{
								y = y0 + min_ty * dy + ny * 2.0f;
							}
							else
							{
								y = y0 + min_ty * dy + ny * 5.0f;
								y += pm->GetPosition_dy();
							}

							//pm->Collision(this, y);
						}
					}
					else {
						y = y0 + min_ty * dy + ny * 0.2f;
					}
				}


			}
			else if (dynamic_cast<CSlopeBrick*>(e->obj))
			{
				if (e->nx != 0)
					x += dx;
				else if (e->ny != 0)
					y += dy;

			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}

		}
	}

	if (magic_star != NULL)
	{
		if (bol_star == true)
		{
			magic_star->Update(dt, coObjects);
			magic_star->SetPosition(x, y + 16);
		}
		else
		{
			if (magic_star->GetState() == MAGICSTAR_STATE_DIE1)
				magic_star = NULL;
			else
				magic_star->Update(dt, coObjects);
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CGimmick::Render()
{
	int ani = -1;

	switch (state)
	{
	case GIMMICK_STATE_DIE:
		ani = GIMMICK_ANI_DIE;
		break;
	case GIMMICK_STATE_JUMP:
		if (nx > 0)
			ani = GIMMICK_ANI_JUMP_RIGHT;
		else
			ani = GIMMICK_ANI_JUMP_LEFT;
		break;
	case GIMMICK_STATE_JUMP_RIGHT:
		ani = GIMMICK_ANI_JUMP_RIGHT;
		break;
	case GIMMICK_STATE_JUMP_LEFT:
		ani = GIMMICK_ANI_JUMP_LEFT;
		break;
	case GIMMICK_STATE_IDLE:
		if (nx > 0)
		{
			if (vx != 0)
				ani = GIMMICK_ANI_WALKING_RIGHT;
			else
				ani = GIMMICK_ANI_IDLE_RIGHT;
		}
		else {
			if (vx != 0)
				ani = GIMMICK_ANI_WALKING_LEFT;
			else
				ani = GIMMICK_ANI_IDLE_LEFT;
		}
		break;
	case GIMMICK_STATE_WALKING_RIGHT:
		ani = GIMMICK_ANI_WALKING_RIGHT;
		break;
	case GIMMICK_STATE_WALKING_LEFT:
		ani = GIMMICK_ANI_WALKING_LEFT;
		break;

	}

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y + 3, alpha);
	if (magic_star != NULL)
		magic_star->Render();

	//RenderBoundingBox();
}

void CGimmick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GIMMICK_STATE_WALKING_RIGHT:
		nx = 1;
		break;
	case GIMMICK_STATE_WALKING_LEFT:
		nx = -1;
		break;
	case GIMMICK_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		//vy = GIMMICK_JUMP_SPEED;
		break;
	case GIMMICK_STATE_IDLE:
		break;
	case GIMMICK_STATE_JUMP_RIGHT:
		vx = GIMMICK_WALKING_SPEED;
		nx = 1;
		break;
	case GIMMICK_STATE_JUMP_LEFT:
		vx = -GIMMICK_WALKING_SPEED;
		nx = -1;
		break;
	}
}

void CGimmick::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	if (state == GIMMICK_STATE_JUMP)
	{
		right = x + GIMMICK_BBOX_JUMP_WIDTH;
		bottom = y - GIMMICK_BBOX_JUMP_HEIGHT;
	}
	else
	{
		right = x + GIMMICK_BBOX_WIDTH;
		bottom = y - GIMMICK_BBOX_HEIGHT;
	}
}


void CGimmick::UpdateSpeed_IDLE(float& vx, float& vy, int nx)
{
	if (vx == 0) return;

	if (nx > 0)
	{
		vx = vx - GIMMICK_INERTIA;
		if (vx < 0)
			vx = 0;
		return;
	}

	if (nx < 0)
	{
		vx = vx + GIMMICK_INERTIA;
		if (vx > 0)
			vx = 0;
		return;
	}
}

void CGimmick::UpdateSpeed_WALKING_RIGHT(float& vx, float& vy)
{
	if (vx == GIMMICK_WALKING_SPEED) return;

	vx = vx + GIMMICK_INERTIA;

	if (vx > GIMMICK_WALKING_SPEED)
		vx = GIMMICK_WALKING_SPEED;
	return;

}

void CGimmick::UpdateSpeed_WALKING_LEFT(float& vx, float& vy)
{
	if (vx == -GIMMICK_WALKING_SPEED) return;

	vx = vx - GIMMICK_INERTIA;

	if (vx < -GIMMICK_WALKING_SPEED)
		vx = -GIMMICK_WALKING_SPEED;
	return;
}

void CGimmick::UpdateSpeed_JUMP(float& vx, float& vy, int nx)
{
	if (vx != 0)
	{
		if (nx > 0)
		{
			vx = vx - GIMMICK_INERTIA / 2;
			if (vx < 0)
				vx = 0;
			return;
		}

		if (nx < 0)
		{
			vx = vx + GIMMICK_INERTIA / 2;
			if (vx > 0)
				vx = 0;
			return;
		}
	}

	if (bol_jump == TRUE || vy <= 0) return;
	vy = 0;

}

void CGimmick::UpdateSpeed_JUMP_RIGHT(float& vx, float& vy)
{
	if (bol_jump == TRUE || vy <= 0) return;
	vy = 0;

}

void CGimmick::UpdateSpeed_JUMP_LEFT(float& vx, float& vy)
{
	if (bol_jump == TRUE || vy <= 0) return;
	vy = 0;
}

void CGimmick::JUMP()
{
	if (state != GIMMICK_STATE_JUMP && state != GIMMICK_STATE_JUMP_RIGHT && state != GIMMICK_STATE_JUMP_LEFT)
	{
		bol_jump = true;
		SetState(GIMMICK_STATE_JUMP);
		vy = GIMMICK_JUMP_SPEED;
	}
}
void CGimmick::STOP_JUMP()
{
	bol_jump = false;
}

void CGimmick::MAGICSTAR()
{
	if (bol_star == true || magic_star != NULL) return;
	bol_star = true;
	magic_star = new CMagicStar();
	magic_star->SetPosition(x, y + 16);
}

void CGimmick::STOP_MAGICSTAR()
{
	bol_star = false;
	if (magic_star != NULL)
	{
		if (magic_star->GetState() == MAGICSTAR_STATE_BIG)
		{
			magic_star->SetState(MAGICSTAR_STATE_BIG_RUN);
			magic_star->SetSpeed(magic_star->GetPosition_vx() + vx, magic_star->GetPosition_vy() + vy);
			magic_star->SetNX(nx);
		}
		else if (magic_star->GetState() != MAGICSTAR_STATE_BIG_RUN)  magic_star = NULL;
	}
}
