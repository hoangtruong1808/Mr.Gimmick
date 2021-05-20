#include "SlopeBrick.h"


void CSlopeBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CSlopeBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (slope.start_x <= x <= slope.end_x)
	{
		if (slope.start_y <= y <= slope.end_y)
		{

		}
	}
	l = x;
	t = y;
	r = x + slope_width;
	b = y + slope_height;
}

void CSlopeBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	
	}
}