#include "SmallPinion.h"

void CSmallPinion::Render()
{
	int ani = SMALLPINION_ANI_TURN;

	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CSmallPinion::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + SMALLPINION_BBOX_WIDTH;
	b = y - SMALLPINION_BBOX_HEIGHT;
}