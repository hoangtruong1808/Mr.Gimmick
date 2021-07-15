#include "Window.h"
#include "Utils.h"
#include "Gimmick.h"
#include "PlayScence.h"

CWindow::CWindow() : CGameObject()
{
	SetState(WINDOW_STATE_CLOSE);
}

void CWindow::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + WINDOW_BBOX_WIDTH;
	bottom = top - WINDOW_BBOX_HEIGHT;
}

void CWindow::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	CGimmick* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (state != WINDOW_STATE_OPEN)
	{
		if (player->x - x <= 50)
		{
			SetState(WINDOW_STATE_OPEN);
		}
	}
}

void CWindow::Render()
{
	int ani;
	if (state == WINDOW_STATE_CLOSE) {
		ani = WINDOW_ANI_CLOSE;
	}
	else 
	{
		ani = WINDOW_ANI_OPEN;
	}
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
	RenderBoundingBox();
}

void CWindow::SetState(int state)
{
	CGameObject::SetState(state);
}