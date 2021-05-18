#include "Brick.h"


void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + brick_width;
	b = y + brick_height;
}

void CBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case BRICK_STATE_UP:
		brick_speed = BRICK_SPEED;
		break;
	case BRICK_STATE_BACK:
		brick_speed = -BRICK_SPEED;
		break;
	}
}