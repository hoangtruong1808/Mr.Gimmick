#include "Cord.h"

CCord::CCord(int t)
{
	this->type = t;
}
void CCord::Render()
{
	int ani;
	switch (this->type)
	{
	case 0:
		ani = CORD_ANI_TURN_TOPLEFT;
		break;
	case 1:
		ani = CORD_ANI_TURN_TOPRIGHT;
		break;
	case 2:
		ani = CORD_ANI_TURN_BOTRIGHT;
		break;
	case 3:
		ani = CORD_ANI_TURN_BOTLEFT;
		break;
	default:
		ani = CORD_ANI_TURN_TOPLEFT;
		break;
	}
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CCord::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CORD_BBOX_WIDTH;
	b = y - CORD_BBOX_HEIGHT;
}