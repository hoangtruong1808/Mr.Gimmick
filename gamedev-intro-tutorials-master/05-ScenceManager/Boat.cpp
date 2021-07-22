#include "Boat.h"
#include "Gimmick.h"
#include "PlayScenCe.h"

void CBoat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y-2;
	right = x + BOAT_BBOX_WIDTH;
	bottom = y - BOAT_BBOX_HEIGHT;
}
void CBoat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) 
{
	CGameObject::Update(dt);
	x += dx;
	if (x >= this->end)
	{
		x = this->end;
		SetState(BOAT_STATE_IDLE);
	}
}
void CBoat::Render()
{
	int ani = BOAT_ANI_DRIFT;
	int alpha = 255;

	animation_set->at(ani)->Render(x, y, alpha);
}
void CBoat::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BOAT_STATE_IDLE:
		vx = 0;
		break;
	case BOAT_STATE_DRIFT:
		vx = BOAT_SPEED;
		break;
	}
}