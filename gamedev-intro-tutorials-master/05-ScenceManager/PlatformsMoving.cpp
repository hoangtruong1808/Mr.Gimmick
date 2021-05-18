#include "PlatformsMoving.h"
#include "Utils.h"

void CPlatformsMoving::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 

	x += dx;
	y += dy;

	if (state == PM_STATE_WIDTH)
	{
		if (x < start)
		{
			x = start;
			vx = PM_SPEED;
		}
		else if (x > end)
		{
			x = end;
			vx = -PM_SPEED;
		}
		else
		{
			if (vx > 0)
			{
				if (x < (start+ 5 +float((end - start)/2)))
					vx += PM_SPEED_ACCELERATION;
				else vx -= PM_SPEED_ACCELERATION;		
			}
			else if (vx < 0)
			{
				if (x > (start - 5 + float((end - start) / 2)))
					vx -= PM_SPEED_ACCELERATION;
				else vx += PM_SPEED_ACCELERATION;
			}
		}
	}

	if (state == PM_STATE_HEIGHT)
	{
		if (y < start)
		{
			y = start;
			vy = PM_SPEED;
		}
		else if (y > end)
		{
			y = end;
			vy = -PM_SPEED;
		}
		else
		{
			if (vy > 0)
			{
				if (y < (start + 5 + float((end - start) / 2)))
					vy += PM_SPEED_ACCELERATION;
				else vy -= PM_SPEED_ACCELERATION;
			}
			else if (vy < 0)
			{
				if (y > (start - 5 + float((end - start) / 2)))
					vy -= PM_SPEED_ACCELERATION;
				else vy += PM_SPEED_ACCELERATION;
			}
		}
	}

}

void CPlatformsMoving::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CPlatformsMoving::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + PM_BBOX_WIDTH;
	b = y + PM_BBOX_HEIGHT;
}

void CPlatformsMoving::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case PM_STATE_WIDTH:
		vx = PM_SPEED;
		vy = 0;
		break;
	case PM_STATE_HEIGHT:
		vy = PM_SPEED;
		vx = 0;
		break;
	}
}