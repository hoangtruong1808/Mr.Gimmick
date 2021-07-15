#include "Tube.h"

CTube::CTube(int t)
{
	this->type = t;
}
void CTube::Render()
{
	int ani;
	switch (this->type)
	{
	case 0:
		ani = TUBE_ANI_TURN_RIGHT;
		break;
	case 1:
		ani = TUBE_ANI_TURN_TOP;
		break;
	case 2:
		ani = TUBE_ANI_TURN_LEFT_BOT;
		break;
	case 3:
		ani = TUBE_ANI_TURN_RIGHT_BOT;
		break;
	case 4:
		ani = TUBE_ANI_TURN_RIGHT_TOP;
		break;
	case 5:
		ani = TUBE_ANI_TURN_LEFT_TOP;
		break;
	default:
		ani = TUBE_ANI_TURN_RIGHT;
		break;
	}

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CTube::GetBoundingBox(float& l, float& t, float& r, float& b) 
{
	l = x;
	t = y;
	r = x + TUBE_BBOX_WIDTH;
	b = y - TUBE_BBOX_HEIGHT;
}