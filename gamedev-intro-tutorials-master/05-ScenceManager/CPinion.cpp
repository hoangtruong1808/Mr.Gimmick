#include "CPinion.h"

CPinion::CPinion(int t)
{
	this->type = t;
}
void CPinion::Render()
{
	int ani;
	switch (this->type)
	{
	case 0:
		ani = PINION_ANI_TURN_TOPLEFT;
		break;
	case 1:
		ani = PINION_ANI_TURN_TOPRIGHT;
		break;
	case 2:
		ani = PINION_ANI_TURN_BOTRIGHT;
		break;
	case 3:
		ani = PINION_ANI_TURN_BOTLEFT;
		break;
	default:
		ani = PINION_ANI_TURN_TOPLEFT;
		break;
	}

	animation_set->at(ani)->Render(x, y);
}

void CPinion::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PINION_BBOX_WIDTH;
	b = y - PINION_BBOX_HEIGHT;
}