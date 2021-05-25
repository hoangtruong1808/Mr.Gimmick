#include "Camera.h"
#include <cstddef>
#include "Gimmick.h"


CCamera* CCamera::__intance = NULL;

CCamera* CCamera::GetInstance()
{
	if (__intance == NULL)
	{
		__intance = new CCamera(CGame::GetInstance()->GetScreenWidth(),
			CGame::GetInstance()->GetScreenHeight());
	}

	return __intance;
}

CCamera::CCamera(int width, int height)
{
	this->ScreenWidth = width;
	this->ScreenHeight = height;
	this->MapHeight = NULL;
	this->MapWidth = NULL;
}

CCamera::~CCamera()
{
}

RECT CCamera::GetCamBound()
{
	camBound.left = camPos.x;
	camBound.top = camPos.y;
	camBound.right = camPos.x + ScreenWidth;
	camBound.bottom = camPos.y + ScreenHeight;
	return camBound;
}

void CCamera::Update(float x, float y)
{
	camPos.x = x;
	camPos.y = y;
	camPos.x -= ScreenWidth / 2;
	camPos.y -= ScreenHeight / 2;

	if (camPos.x < 0)
		camPos.x = 0;

	if (camPos.y < 0)
		camPos.y = 0;
	
	if (camPos.y > MapHeight - ScreenHeight) camPos.y = MapHeight - ScreenHeight;
	
	if (camPos.x > MapWidth - ScreenWidth) camPos.x = MapWidth - ScreenWidth;
	CGame::GetInstance()->SetCamPos(camPos.x, camPos.y);
}

void CCamera::SetCamMap(float mapWidth, float mapHeight)
{
	this->MapWidth = mapWidth;
	this->MapHeight = mapHeight;
}

bool CCamera::isContain(RECT rect)
{
	//Lấy biên giới hạn camera
	RECT camRect;
	camRect.left = camPos.x;
	camRect.top = camPos.y;
	camRect.right = camPos.x + ScreenWidth;
	camRect.bottom = camPos.y + ScreenHeight;

	//Kiểm tra rect có giao với Camera hay không
	if (rect.left > camRect.right || rect.right < camRect.left || rect.top > camRect.bottom || rect.bottom < camRect.top)
	{
		return false;
	}

	return true;
}

bool CCamera::isContain(D3DXVECTOR2 pos)
{
	//Lấy biên giới hạn camera
	RECT camRect;
	camRect.left = camPos.x;
	camRect.top = camPos.y;
	camRect.right = camPos.x + ScreenWidth;
	camRect.bottom = camPos.y + ScreenHeight;

	//Kiểm tra Pos có nằm trong Camera hay không
	if (pos.x > camRect.right || pos.x < camRect.left || pos.y > camRect.bottom || pos.y < camRect.top)
	{
		return false;
	}

	return true;
}