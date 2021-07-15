#include "Tunnel.h"

CTunnel::CTunnel(int t)
{
	this->type = t;
}
void CTunnel::Render()
{
	int ani;
	switch (this->type)
	{
	case 0:
		ani = TUNNEL_ANI_HEAD;
		break;
	case 1:
		ani = TUNNEL_ANI_BODY;
		break;
	case 2:
		ani = TUNNEL_ANI_END;
		break;
	default:
		ani = TUNNEL_ANI_BODY;
		break;
	}

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CTunnel::GetBoundingBox(float& l, float& t, float& r, float& b)
{

	l = x;
	t = y;
	b = y - TUNNEL_BBOX_HEIGHT;
	switch (this->type)
	{
	case 0:
		r = x + TUNNEL_HEAD_BBOX_WIDTH;
		break;
	case 1:
		r = x + TUNNEL_BODY_BBOX_WIDTH;
		break;
	case 2:
		r = x + TUNNEL_END_BBOX_WIDTH;
		break;
	default:
		r = x + TUNNEL_BODY_BBOX_WIDTH;
		break;
	}
}