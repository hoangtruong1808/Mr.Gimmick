#include <iostream>
#include <fstream>

#include "Gimmick.h"
#include <algorithm>
#include <assert.h>
#include "Utils.h"
#include "Game.h"
#include "Brick.h"
#include "Gun.h"
#include "Portal.h"
#include "PlatformsMoving.h"
#include "SlopeBrick.h"
#include "Swing.h"
#include "Water.h"
#include "Turtle.h"
#include "Tube.h"
#include "Bird.h"
#include "Boat.h"
#include "Cannon.h"
#include "CannonBullet.h"
#include "BlackEnemy.h"
#include "MoveArea.h"
#include "PlayScence.h"


CGimmick::CGimmick() : CGameObject()
{
	lever = 10;
	untouchable = 0;
	SetState(GIMMICK_STATE_IDLE);
}

void CGimmick::StartUntouchable()
{
	if (!untouchable)
	{
		if (lever >= 1) {
			untouchable = 1;
			untouchable_start = GetTickCount64();
			SetState(GIMMICK_STATE_STUNNED);
			lever = lever - 1;
		}
		else SetState(GIMMICK_STATE_DIE);
	}
}

void CGimmick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (state == GIMMICK_STATE_DIE && GetTickCount64() - die_start >= GIMMICK_REVIVAL_TIME)
	{
		Revival();
	}

	if (state == GIMMICK_STATE_DIE)
	{
		return;
	}
	else
		if (state == GIMMICK_STATE_STUNNED)
		{

			if (GetTickCount() - untouchable_start > 900)
				SetState(GIMMICK_STATE_IDLE);
			else return;
		}
		else
		{
			if (untouchable)
			{
				if (GetTickCount() - untouchable_start > GIMMICK_UNTOUCHABLE_TIME)
				{
					untouchable_start = 0;
					untouchable = 0;
				}
			}

			vy += GIMMICK_GRAVITY * dt;

			CGameObject::Update(dt);

			switch (state)
			{
			case GIMMICK_STATE_IDLE: UpdateSpeed_IDLE(vx, vy, nx); break;
			case GIMMICK_STATE_WALKING_RIGHT: UpdateSpeed_WALKING_RIGHT(vx, vy); break;
			case GIMMICK_STATE_WALKING_LEFT: UpdateSpeed_WALKING_LEFT(vx, vy); break;
			case GIMMICK_STATE_JUMP: UpdateSpeed_JUMP(vx, vy, nx); break;
			case GIMMICK_STATE_JUMP_LEFT: UpdateSpeed_JUMP_LEFT(vx, vy); break;
			case GIMMICK_STATE_JUMP_RIGHT: UpdateSpeed_JUMP_RIGHT(vx, vy); break;
			}

		
			

			// Simple fall down
			vector<LPGAMEOBJECT> newCoObjects;
			if (magic_star != NULL)
			{
				if (bol_star == true)
				{
					magic_star->SetPosition(x, y + 16);
				}
				else
				{
					if (magic_star->GetState() == MAGICSTAR_STATE_DIE1)
						magic_star = NULL;
					else
					{
						newCoObjects.push_back(magic_star);
					}
				}
			}

			for (UINT i = 0; i < coObjects->size(); i++)
			{
				if (dynamic_cast<CBrick*>(coObjects->at(i)))
				{
					CBrick* brick = dynamic_cast<CBrick*>(coObjects->at(i));
					brick->Collision(this, dy, dx);
					newCoObjects.push_back(coObjects->at(i));
				}
				else if (dynamic_cast<CPlatformsMoving*>(coObjects->at(i)))
				{
					CPlatformsMoving* brick = dynamic_cast<CPlatformsMoving*>(coObjects->at(i));
					brick->Collision(dt, this, dy, dx);
					newCoObjects.push_back(coObjects->at(i));
				}
				else if (dynamic_cast<CWater*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				else if (dynamic_cast<CPortal*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				else if (dynamic_cast<CTurtle*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				else if (dynamic_cast<CGun*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				else if (dynamic_cast<CSwing*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				else if (dynamic_cast<CMoveArea*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				//else if (dynamic_cast<CTube*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				else if (dynamic_cast<CBird*>(coObjects->at(i)))
				{
					if (!((CBird*)(coObjects->at(i)))->FlyHigher())
						newCoObjects.push_back(coObjects->at(i));
				}
				else if (dynamic_cast<CBoat*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				else if (dynamic_cast<CCannon*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
				else if (dynamic_cast<CSlopeBrick*>(coObjects->at(i))) {
					newCoObjects.push_back(coObjects->at(i));
					CSlopeBrick* brick = dynamic_cast<CSlopeBrick*>(coObjects->at(i));
					brick->Collision(this, dy, dx);
				}
				else if (dynamic_cast<CCannonBullet*>(coObjects->at(i))) {
					newCoObjects.push_back(coObjects->at(i));
					CCannonBullet* bullet = dynamic_cast<CCannonBullet*>(coObjects->at(i));
					bullet->Collision();
				}
				else if (dynamic_cast<CBlackEnemy*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));

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
				float tempy = vy;
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
								{
									x += brick->GetBrickSpeed() * dt;
									y = y0 + min_ty * dy + ny * 0.2f;
								}
							y = y0 + min_ty * dy + ny * 0.1f;
						}
						else
						{
							if (e->nx != 0)
							{
								if (brick->GetState() == NULL)
								{
									//vx = 0;
									x = x0 + min_tx * dx + nx * 0.1f;
								}
								else x += dx;

							}
						}

					}
					else if (dynamic_cast<CPlatformsMoving*>(e->obj))
					{
						CPlatformsMoving* pm = dynamic_cast<CPlatformsMoving*>(e->obj);
						if (e->nx != 0)
						{
							x = x0 + min_tx * dx + nx * 0.1f;

						}
						else if (e->ny != 0)
						{
							y = y0 + min_ty * dy + ny * 0.2f;
						}

					}
					else if (dynamic_cast<CSlopeBrick*>(e->obj))
					{
						if (e->nx != 0)
							x += dx;
						else if (e->ny != 0)
							y += dy;

					}
					if (dynamic_cast<CGun*>(e->obj))
					{
						CGun* gun = dynamic_cast<CGun*>(e->obj);
						if (e->ny != 0)
						{
							vy = 0;
						}
						if (e->nx != 0)
						{
							gun->x += dx;
							x += dx;
						}
					}
					if (dynamic_cast<CSwing*>(e->obj)) {
						CSwing* swing = dynamic_cast<CSwing*>(e->obj);

						if (swing->GetState() == SWING_STATE_OPEN || swing->GetState() == SWING_STATE_STOP)
						{
							x += dx;
							y += dy;
						}
						else
						{
							if (e->ny > 0)
							{
								if (state == GIMMICK_STATE_WALKING_RIGHT || state == GIMMICK_STATE_WALKING_LEFT)
								{
									x = x0 + min_tx * dx + nx * 0.1f;
								}
								else
								{
									x = x0 + swing->dx;
								}
								vy = 0;
							}
							y = y0 + min_ty * dy + ny * 0.1f;
						}
					}
					if (dynamic_cast<CBoat*>(e->obj))
					{
						CBoat* boat = dynamic_cast<CBoat*>(e->obj);
						boat->SetState(BOAT_STATE_DRIFT);

						if (boat->GetState() == BOAT_STATE_IDLE)
						{
							x += dx;
							y += dy;
						}
						else
						{
							if (e->ny > 0)
							{
								if (state == GIMMICK_STATE_WALKING_RIGHT || state == GIMMICK_STATE_WALKING_LEFT)
								{
									x = x0 + min_tx * dx + nx * 0.1f;
								}
								else
								{
									x = x0 + boat->dx;
								}
								vy = 0;
							}

							y = y0 + min_ty * dy + ny * 0.1f;
						}

					}
					if (dynamic_cast<CCannon*>(e->obj))
					{
						CCannon* cannon = dynamic_cast<CCannon*>(e->obj);
						if (e->ny != 0)
						{
							vy = 0;
						}
						if (e->nx != 0)
						{
							//this->vy = -100.0f;
							cannon->state = CANNON_STATE_MOVE;
							if (state == GIMMICK_STATE_IDLE)
							{
								cannon->state = CANNON_STATE_IDLE;
							}
							cannon->x += dx;
							x += dx;
						}
					}
					if (dynamic_cast<CBird*>(e->obj))
					{
						CBird* bird = dynamic_cast<CBird*>(e->obj);
						x = x0 + min_tx * dx + nx * 0.1f;
						y = y0 + min_ty * dy + ny * 0.1f;
						vy = 0;

						bird->isPlayerOn(true);

					}
					/*if (dynamic_cast<CMoveArea*>(e->obj)) {
						CMoveArea* movearea = dynamic_cast<CMoveArea*>(e->obj);
						x = 100;
						y = 100;
					}*/
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

}

void CGimmick::Render()
{
	int ani = -1;
	int d = 3;

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
	case GIMMICK_STATE_STUNNED:
		d = 8;
		if (nx > 0)
			ani = GIMMICK_ANI_STUNNED_RIGHT;
		else
			ani = GIMMICK_ANI_STUNNED_LEFT;
		break;

	}

	int alpha = 255;
	if (untouchable && state != GIMMICK_STATE_STUNNED)	alpha = 128;

	animation_set->at(ani)->Render(x, y + d, alpha);
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
	case GIMMICK_STATE_DIE:
		vx = 0;
		vy = 0;
		this->die_start = GetTickCount64();
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
			magic_star->SetNX(nx);
			magic_star->SetState(MAGICSTAR_STATE_BIG_RUN);
			magic_star->SetSpeed(magic_star->GetPosition_vx() + vx, magic_star->GetPosition_vy() + vy);
			
		}
		else if (magic_star->GetState() != MAGICSTAR_STATE_BIG_RUN)  magic_star = NULL;
	}
}
void CGimmick::Revival() {
	float revival_x, revival_y;
	CScene* scene = CGame::GetInstance()->GetCurrentScene();
	((CPlayScene*)scene)->GetRevivalPosition(revival_x, revival_y);
	this->SetPosition(revival_x, revival_y);
	this->SetSpeed(0, 0);
	this->SetState(GIMMICK_STATE_IDLE);
	this->untouchable = 0;
}
