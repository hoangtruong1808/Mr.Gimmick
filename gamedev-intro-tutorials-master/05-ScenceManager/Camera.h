#pragma once

#include "Game.h"
class CCamera
{
private:
	float cam_x;
	float cam_y;
public:
	CCamera();
	void SetCamera(float CamX, float CamY);
	void GetCamera(float CamX, float CamY);
	float GetCam_X() { return cam_x; }
	float GetCam_Y() { return cam_y; }
};

