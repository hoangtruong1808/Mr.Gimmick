#include "Camera.h"

CCamera::CCamera()
{
	this->cam_x = NULL;
	this->cam_y = NULL;
}
void CCamera::SetCamera(float CamX, float CamY)
{
	this->cam_x = CamX;
	this->cam_y = CamY;
}
void CCamera::GetCamera(float CamX, float CamY)
{
	CamX = this->cam_x;
	CamY = this->cam_y;
}
