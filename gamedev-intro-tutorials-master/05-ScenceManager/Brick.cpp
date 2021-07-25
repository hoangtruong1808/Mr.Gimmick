#include "Brick.h"


void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CBrick::Collision(LPGAMEOBJECT object, float dy, float dx)
{
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);

	float obj_l, obj_t, obj_r, obj_b;
	object->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);


	if (obj_r   < l || obj_l  > r || obj_t  < b || obj_b + 4> t)
		return;

	float obj_width = obj_r - obj_l;
	float obj_height = obj_t - obj_b;

	float check_x;
	float check_y;

	if (obj_r > l && obj_l < l)
	{
		float d = l - obj_r;
		check_x = obj_r + d - 0.2 - obj_width;
		object->SetPosX(check_x);
	}
	else if (obj_l  < r && obj_r  > r)
	{
		float d = r - obj_l;
		check_x = obj_l + d + 0.2f;
		object->SetPosX(check_x);
	}

	if (obj_t > b && obj_b < b)
	{
		float d = b - obj_t;
		check_y = obj_t + d - 0.2f;
		object->SetPosY(check_y);
	}
	else if (obj_b  < t && obj_t  > t)
	{
		float d = t - obj_b;
		check_y = obj_b + d + obj_height + 0.2f;
		object->SetPosY(check_y);
	}
}


void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + brick_width;
	b = y - brick_height;
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