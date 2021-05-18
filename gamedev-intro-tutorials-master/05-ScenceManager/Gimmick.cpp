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


CGimmick::CGimmick() : CGameObject()
{
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);
}

void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += GIMMICK_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != GIMMICK_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

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

		x = x0 + min_tx * dx + nx * 0.4f;
		y = y0 + min_ty * dy + ny * 0.4f;


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
				if (e->nx != 0)
				{
					vx = 0;
				}
				if (e->ny != 0)
				{	
					vy = 0;
					if (e->ny < 0)
					{
						if (vx > 0) state = GIMMICK_STATE_WALKING_RIGHT;
						else if (vx < 0) state = GIMMICK_STATE_WALKING_LEFT;
						else if (vx == 0) state = GIMMICK_STATE_IDLE;

					}
					
				}
				if (e->ny <= 0 && e->nx == 0)
				{
					if (brick->GetState() != NULL)
					{
						x += brick->GetBrickSpeed()*dt;
					}
				}

			}
			else if (dynamic_cast<CPlatformsMoving*>(e->obj))
			{
				CPlatformsMoving* pm = dynamic_cast<CPlatformsMoving*>(e->obj);
				if (e->nx != 0)
				{
					vx = 0;
				}
				if (e->ny != 0)
				{
					vy = 0;
					if (e->ny < 0)
					{
						if (vx > 0) state = GIMMICK_STATE_WALKING_RIGHT;
						else if (vx < 0) state = GIMMICK_STATE_WALKING_LEFT;
						else if (vx == 0) state = GIMMICK_STATE_IDLE;
					}
				}

			}
			else if (dynamic_cast<CPortal*>(e->obj))
			{
				CPortal* p = dynamic_cast<CPortal*>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CGimmick::Render()
{
	int ani = -1;
	if (state == GIMMICK_STATE_DIE)
		ani = GIMMICK_ANI_DIE;
	else {
		if (state == GIMMICK_STATE_JUMP) {
			if (nx > 0)
				ani = GIMMICK_ANI_JUMP_RIGHT;
			else
				ani = GIMMICK_ANI_JUMP_LEFT;
		}
		else if (state == GIMMICK_STATE_JUMP_RIGHT) ani = GIMMICK_ANI_JUMP_RIGHT;
		else if (state == GIMMICK_STATE_JUMP_LEFT) ani = GIMMICK_ANI_JUMP_LEFT;
		else {
			if (vx == 0)
			{
				if (nx > 0) ani = GIMMICK_ANI_IDLE_RIGHT;
				else ani = GIMMICK_ANI_IDLE_LEFT;
			}
			else if (vx > 0)
				ani = GIMMICK_ANI_WALKING_RIGHT;
			else ani = GIMMICK_ANI_WALKING_LEFT;
			
		}
	}	

	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y-3, alpha);

	//RenderBoundingBox();
}

void CGimmick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case GIMMICK_STATE_WALKING_RIGHT:
		vx = GIMMICK_WALKING_SPEED;
		nx = 1;
		break;
	case GIMMICK_STATE_WALKING_LEFT:
		vx = -GIMMICK_WALKING_SPEED;
		nx = -1;
		break;
	case GIMMICK_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -GIMMICK_JUMP_SPEED_Y;
		break;
	case GIMMICK_STATE_IDLE:
		vx = 0;
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
		bottom = y + GIMMICK_BBOX_JUMP_HEIGHT;
	}
	else
	{
		right = x + GIMMICK_BBOX_WIDTH;
		bottom = y + GIMMICK_BBOX_HEIGHT;
	}
}

