#include "Swing.h"
#include "PlayScence.h"
#include "algorithm"

CSwing::CSwing() : CGameObject()
{
	SetState(SWING_STATE_IDLE);
}

void CSwing::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y - 62.0f;
	right = x + SWING_BBOX_WIDTH;
	bottom = y - SWING_BBOX_HEIGHT;
}

void CSwing::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	x += dx;

	CGimmick* gimmick = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	if (gimmick->x - this->x >= 0)
	{
		if (state == SWING_STATE_IDLE)
		{
			SetState(SWING_STATE_MOVE);
		}
	}
	if (x >= SWING_STOP_POSITION)
	{
		if (state == SWING_STATE_MOVE)
			SetState(SWING_STATE_OPEN);
	}

	if (state == SWING_STATE_OPEN)
	{
		SetState(SWING_STATE_STOP);
	}
}

void CSwing::Render()
{
	int ani = -1;

	switch (state)
	{
	case SWING_STATE_IDLE:
		ani = SWING_ANI_IDLE;
		break;
	case SWING_STATE_MOVE:
		ani = SWING_ANI_MOVE;
		break;
	case SWING_STATE_STOP:
		ani = SWING_ANI_STOP;
		break;
	case SWING_STATE_OPEN:
		ani = SWING_ANI_OPEN;
		break;
	default:
		ani = SWING_ANI_IDLE;
		break;
	}


	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);

	RenderBoundingBox();
}

void CSwing::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SWING_STATE_IDLE:
		vx = 0;
		break;
	case SWING_STATE_MOVE:
		vx = SWING_SPEED;
		break;
	case SWING_STATE_OPEN:
		vx = 0;
		break;
	}
}