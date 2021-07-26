#include "MagicStar.h"
#include "Brick.h"
#include "BlackEnemy.h"
#include "SlopeBrick.h"
#include "Gimmick.h"
#include "Fire.h"

CMagicStar::CMagicStar() : CGameObject()
{
	rally_start = 0;
	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(40100);
	SetAnimationSet(ani_set);
	SetState(MAGICSTAR_STATE_SMALL);
	star1.x = 0;  star1.y = 48;
	star2.x = -45.6; star2.y = 14.8;
	star3.x = 45.6; star3.y = 14.8;
	star4.x = -28.2; star4.y = -38.8;
	star5.x = 28.2; star5.y = -38.8;
	star_len = 48.0;
}

void CMagicStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

	switch (state)
	{
	case MAGICSTAR_STATE_SMALL:
	{
		float len = star_len;
		star_len = star_len - 0.1 * dt;
		if (star_len <= 0)
		{
			state = MAGICSTAR_STATE_SMALL_1;
			D3DXVECTOR2 star;
			star.x = 0;
			star.y = 0;
			star1 = star;
			star2 = star;
			star3 = star;
			star4 = star;
			star5 = star;
		}
		else
		{
			star1.y = star_len;

			star2.y = star_len / len * star2.y;
			star2.x = star_len / len * star2.x;

			star3.y = star_len / len * star3.y;
			star3.x = star_len / len * star3.x;

			star4.y = star_len / len * star4.y;
			star4.x = star_len / len * star4.x;

			star5.y = star_len / len * star5.y;
			star5.x = star_len / len * star5.x;
		}
	}
	break;
	case MAGICSTAR_STATE_SMALL_1:
	{
		rally_start = rally_start + dt;
		if (rally_start > 400)
			state = MAGICSTAR_STATE_BIG;
	}
	break;
	case MAGICSTAR_STATE_BIG:
		break;
	case MAGICSTAR_STATE_BIG_RUN:
	{
		if (y < 0) { state = MAGICSTAR_STATE_DIE1; return; }

		vy += MAGICSTAR_GRAVITY * dt;
		CGameObject::Update(dt);


		vector<LPGAMEOBJECT> newCoObjects;
		for (UINT i = 0; i < coObjects->size(); i++)
		{
			if (dynamic_cast<CBrick*>(coObjects->at(i)))
			{
				CBrick* brick = dynamic_cast<CBrick*>(coObjects->at(i));
				brick->Collision(this, dy, dx);
				newCoObjects.push_back(coObjects->at(i));
			}
			else if (dynamic_cast<CSlopeBrick*>(coObjects->at(i))) {
				CSlopeBrick* brick = dynamic_cast<CSlopeBrick*>(coObjects->at(i));
				brick->Collision(this, dy, dx);
				newCoObjects.push_back(coObjects->at(i));
			}
			else if (dynamic_cast<CFire*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
			else if (dynamic_cast<CBlackEnemy*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
		}

		vector<LPCOLLISIONEVENT> coEvents;
		vector<LPCOLLISIONEVENT> coEventsResult;

		coEvents.clear();

		if (state == MAGICSTAR_STATE_DIE) return;

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

			// TODO: This is a very ugly designed function!!!!
			FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


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

			for (UINT i = 0; i < coEventsResult.size(); i++)
			{

				LPCOLLISIONEVENT e = coEventsResult[i];

				//Brick
				if (dynamic_cast<CBrick*>(e->obj))
				{
					CBrick* brick = dynamic_cast<CBrick*>(e->obj);

					if (e->ny != 0)
					{

						if (e->ny > 0)
						{
							if (brick->GetState() != NULL)
								x += brick->GetBrickSpeed() * dt;
							if (vy > -0.02f && vy < 0.02f)
								SetState(MAGICSTAR_STATE_DIE);

							if (vy < 0 && vy < -0.3f)
								vy = 0.3f;
							else vy = -vy / 1.25;

						}
						else vy = -vy / 1.25;

						y = y0 + min_ty * dy + ny * 0.2f;
					}
					else
					{
						if (e->nx != 0)
						{
							vx = -vx;
							x = x0 + min_tx * dx + nx * 0.2f;
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
				else if (dynamic_cast<CBlackEnemy*>(e->obj))
				{
					CBlackEnemy* enemy = dynamic_cast<CBlackEnemy*>(e->obj);
					enemy->SetState(BLACKENEMY_STATE_DIE);
					this->SetState(MAGICSTAR_STATE_DIE);
				}
				else if (dynamic_cast<CFire*>(e->obj))
				{
					this->SetState(MAGICSTAR_STATE_DIE);
				}
			}
		}
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
	}
	break;
	case MAGICSTAR_STATE_DIE:
		rally_start = rally_start + dt;
		if (rally_start > 400)
			state = MAGICSTAR_STATE_DIE1;
		break;
	}
}

void CMagicStar::Render()
{
	switch (state)
	{
	case MAGICSTAR_STATE_SMALL:
		animation_set->at(0)->Render(x + 4 + star1.x, y - 8 + star1.y);
		animation_set->at(0)->Render(x + 4 + star2.x, y - 8 + star2.y);
		animation_set->at(0)->Render(x + 4 + star3.x, y - 8 + star3.y);
		animation_set->at(0)->Render(x + 4 + star4.x, y - 8 + star4.y);
		animation_set->at(0)->Render(x + 4 + star5.x, y - 8 + star5.y);
		break;
	case MAGICSTAR_STATE_SMALL_1:
		animation_set->at(1)->Render(x - 5, y + 7);
		break;
	case MAGICSTAR_STATE_BIG:
		animation_set->at(2)->Render(x, y);
		break;
	case MAGICSTAR_STATE_BIG_RUN:
		animation_set->at(2)->Render(x, y);
		break;
	case MAGICSTAR_STATE_DIE:
		animation_set->at(3)->Render(x, y);
		break;
	}

	//RenderBoundingBox();
}

void CMagicStar::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MAGICSTAR_STATE_BIG_RUN:
		if (nx > 0)
			vx = 0.1f;
		else vx = -0.1f;
		break;
	case MAGICSTAR_STATE_DIE:
		rally_start = 0;
		break;
	}
}

void CMagicStar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + MAGICSTAR_BBOX_BIG_WIDTH;
	bottom = y - MAGICSTAR_BBOX_BIG_HEIGHT;
}

