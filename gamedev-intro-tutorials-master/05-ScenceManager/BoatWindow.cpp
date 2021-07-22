#include "BoatWindow.h"
#include "PlayScence.h"

void CBoatWindow::Render()
{
	int ani = BOATWINDOW_ANI;
	int alpha = 255;
	animation_set->at(ani)->Render(x, y, alpha);
}

void CBoatWindow::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + BOATWINDOW_BBOX_WIDTH;
	bottom = y - BOATWINDOW_BBOX_HEIGHT;
}