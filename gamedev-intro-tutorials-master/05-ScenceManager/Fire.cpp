#include "Fire.h"

void CFire::Render()
{
	int ani = FIRE_ANI_BURN;

	animation_set->at(ani)->Render(x, y);
}

void CFire::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + FIRE_BBOX_WIDTH;
	b = y - FIRE_BBOX_HEIGHT;
}
