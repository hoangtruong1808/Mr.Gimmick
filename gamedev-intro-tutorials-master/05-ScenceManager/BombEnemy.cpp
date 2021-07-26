#include "BombEnemy.h"
#include "MagicStar.h"
#include "Brick.h"
#include "Utils.h"
#include "Game.h"
#include "PlayScence.h"
#include "Gimmick.h"
#include "BlackBomb.h"


CBombEnemy::CBombEnemy(float left, float right) : CGameObject()
{
	this->SetState(BOMBENEMY_STATE_WALKING_RIGHT);
	this->left = left;
	this->right = right;
	this->vy = 0;
}

void CBombEnemy::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + BOMBENEMY_BBOX_WIDTH;
	bottom = top - BOMBENEMY_BBOX_HEIGHT;
}

void CBombEnemy::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CScene* scene = CGame::GetInstance()->GetCurrentScene();


	CGimmick* player = ((CPlayScene*)scene)->GetPlayer();

	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	if (this->state == BOMBENEMY_STATE_DIE_RIGHT || this->state == BOMBENEMY_STATE_DIE_LEFT)
		vy -= BOMBENEMY_GRAVITY * dt;
	else
		vy = 0;

	float x0 = x;
	float y0 = y;

	x = x0 + dx;
	y = y0 + dy;

	if (this->state == BOMBENEMY_STATE_DIE_RIGHT || this->state == BOMBENEMY_STATE_DIE_LEFT)
		return;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny = 0;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				this->y = y0 + min_ty * dy + e->ny * 0.2f;
				this->x = x0 + min_tx * dx + e->nx * 0.2f;

				if (e->nx > 0 && this->state == BOMBENEMY_STATE_WALKING_LEFT)
					this->SetState(BOMBENEMY_STATE_WALKING_RIGHT);
				if (e->nx < 0 && this->state == BOMBENEMY_STATE_WALKING_RIGHT)
					this->SetState(BOMBENEMY_STATE_WALKING_LEFT);

				if (e->ny != 0)
					this->vy = 0;
			}
			if (dynamic_cast<CMagicStar*>(e->obj))
			{
				Shot();
				if (this->state == BOMBENEMY_STATE_WALKING_RIGHT)
					this->SetState(BOMBENEMY_STATE_DIE_RIGHT);
				else
					this->SetState(BOMBENEMY_STATE_DIE_LEFT);
			}
		}
	}

	if (this->x < this->left && this->state == BOMBENEMY_STATE_WALKING_LEFT)
		this->SetState(BOMBENEMY_STATE_WALKING_RIGHT);
	if (this->x + BOMBENEMY_BBOX_WIDTH > this->right && this->state == BOMBENEMY_STATE_WALKING_RIGHT)
		this->SetState(BOMBENEMY_STATE_WALKING_LEFT);

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}

void CBombEnemy::Shot() {
	CBlackBomb* bomb = new CBlackBomb();

	bomb->SetPosition(this->x, this->y);
	if (this->state == BOMBENEMY_STATE_WALKING_RIGHT)
		bomb->SetState(BLACKBOMB_STATE_WALKING_RIGHT);
	else
		bomb->SetState(BLACKBOMB_STATE_WALKING_LEFT);

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();
	LPANIMATION_SET ani_set = animation_sets->Get(410001);
	bomb->SetAnimationSet(ani_set);
	((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->AddObject(bomb);
}

void CBombEnemy::Render()
{
	int alpha = 255;

	int ani = -1;

	switch (this->GetState())
	{
	case BOMBENEMY_STATE_WALKING_RIGHT:
		ani = BOMBENEMY_ANI_WALKING_RIGHT;
		break;
	case BOMBENEMY_STATE_WALKING_LEFT:
		ani = BOMBENEMY_ANI_WALKING_LEFT;
		break;
	case BOMBENEMY_STATE_DIE_RIGHT:
		ani = BOMBENEMY_ANI_DIE_RIGHT;
		break;
	case BOMBENEMY_STATE_DIE_LEFT:
		ani = BOMBENEMY_ANI_DIE_RIGHT;
		break;
	default:
		ani = BOMBENEMY_ANI_WALKING_RIGHT;
		break;
	}

	animation_set->at(ani)->Render(x, y, alpha);
	//RenderBoundingBox();
}

void CBombEnemy::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BOMBENEMY_STATE_WALKING_RIGHT:
		this->vx = BOMBENEMY_WALK_SPEED;
		break;
	case BOMBENEMY_STATE_WALKING_LEFT:
		this->vx = -BOMBENEMY_WALK_SPEED;
		break;
	case BOMBENEMY_STATE_DIE_RIGHT:
		this->y -= 16;
		this->vy = BOMBENEMY_DIE_SPEED;
		break;
	case BOMBENEMY_STATE_DIE_LEFT:
		this->y -= 16;
		this->vy = BOMBENEMY_DIE_SPEED;
		break;
	}
}