#include "Water.h"

void CWater::Render()
{
	int ani = WATER_ANI_WAVE;
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CWater::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + WATER_BBOX_WIDTH;
	b = y - WATER_BBOX_HEIGHT;
}