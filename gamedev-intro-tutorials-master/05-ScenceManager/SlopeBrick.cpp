#include "SlopeBrick.h"


void CSlopeBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CSlopeBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
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