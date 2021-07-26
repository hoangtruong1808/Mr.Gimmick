#include "MoveArea.h"
#include "Gimmick.h"

CMoveArea::CMoveArea(float x, float y)
{
	this->move_x = x;
	this->move_y = y;
}

void CMoveArea::Render()
{
	RenderBoundingBox();
}

void CMoveArea::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + MOVEAREA_BBOX_WIDTH;
	b = y - MOVEAREA_BBOX_HEIGHT;
}
void CMoveArea::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	vector<LPGAMEOBJECT> newCoObjects;
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CGimmick*>(coObjects->at(i))) newCoObjects.push_back(coObjects->at(i));
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(&newCoObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		float y0, x0;
		x0 = x;
		y0 = y;
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{

			LPCOLLISIONEVENT e = coEventsResult[i];

			//Brick
		if (dynamic_cast<CGimmick*>(e->obj))
			{
				CGimmick* gimmick = dynamic_cast<CGimmick*>(e->obj);
				gimmick->x = this->move_x;
				gimmick->y = this->move_y;
				gimmick->vx = 0;
				gimmick->vy = 0;
			}

		}
	}
	for (UINT i = 0; i < newCoObjects.size(); i++)
		newCoObjects[i] = nullptr;
	for (UINT i = 0; i < coEvents.size(); i++)
		delete coEvents[i];
}