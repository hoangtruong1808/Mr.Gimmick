#include "SlopeBrick.h"
#include "Gimmick.h"


void CSlopeBrick::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CSlopeBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + slope_width;
	b = y - slope_height;
}

void CSlopeBrick::Collision(LPGAMEOBJECT object, float dy, float dx)
{
	float l, t, r, b;
	this->GetBoundingBox(l, t, r, b);

	float obj_l, obj_t, obj_r, obj_b;
	object->GetBoundingBox(obj_l, obj_t, obj_r, obj_b);

	float obj_width = obj_r - obj_l;
	float obj_height = obj_t - obj_b;
	
	//if (!this->isContain(object))
		//return;
	if (state == SLOPE_STATE_BACK)
	{
		if (obj_r + dx  < l || obj_r - 4 + dx > r || obj_b + dy > t)
			return;
		if (dynamic_cast<CGimmick*>(object))
		{
			CGimmick* gimmick = dynamic_cast<CGimmick*>(object);
			if ((gimmick->GetState() == GIMMICK_STATE_JUMP || gimmick->GetState() == GIMMICK_STATE_JUMP_RIGHT || gimmick->GetState() == GIMMICK_STATE_JUMP_LEFT)&&gimmick->GetPosition_vy() <0 )
				gimmick->SetState(GIMMICK_STATE_IDLE);
		}
		if (object->nx > 0)
			object->SetVX(object->GetPosition_vx() - brick_speed);
		else
			object->SetVX(object->GetPosition_vx() - BRICK_SPEED);

		float check_x = obj_r + dx ;
		float check_y = obj_b + dy;

		float y_col = this->y - (r - check_x) * this->ratio_hw;
		
		if (check_y < y_col)
		{
			check_x = check_x - obj_width;
			check_y = y_col + obj_height;

			object->SetPosition(check_x - dx, check_y - dy);
		}
	}
	else if (state == SLOPE_STATE_UP) {
		if (obj_l + 4 + dx  < l || obj_l + dx > r || obj_b + dy > t)
			return;
		if (dynamic_cast<CGimmick*>(object))
		{
			CGimmick* gimmick = dynamic_cast<CGimmick*>(object);
			if ((gimmick->GetState() == GIMMICK_STATE_JUMP || gimmick->GetState() == GIMMICK_STATE_JUMP_RIGHT || gimmick->GetState() == GIMMICK_STATE_JUMP_LEFT) && gimmick->GetPosition_vy() < 0)
				gimmick->SetState(GIMMICK_STATE_IDLE);
		}
		if (object->nx < 0)
			object->SetVX(object->GetPosition_vx() + brick_speed);
		else
			object->SetVX(object->GetPosition_vx() + BRICK_SPEED);

		float check_x = obj_l + dx;
		float check_y = obj_b + dy;

		float y_col = this->y - (check_x - l) * this->ratio_hw;
		
		if (check_y < y_col)
		{
			check_y = y_col + obj_height;
			object->SetPosition(check_x - dx, check_y - dy);
		}
	}
		
	
}

void CSlopeBrick::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	
	}
}