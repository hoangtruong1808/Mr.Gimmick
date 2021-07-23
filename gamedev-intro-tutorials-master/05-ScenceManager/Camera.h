#pragma once
#include <cstddef>
#include "Game.h"
#include "GameObject.h"
#include "Gimmick.h"

class CCamera
{
	static CCamera* __intance;

	D3DXVECTOR2 camPos;

	D3DXVECTOR2 playerPos;

	int ScreenWidth;
	int ScreenHeight;
	float MapWidth;
	float MapHeight;

	RECT camBound;		  //giới hạn camera dựa  map

public:
	static CCamera* GetInstance();
	CCamera(int w, int h);
	~CCamera();

	void SetPosition(const D3DXVECTOR2& pos);
	void SetDefaultCam();
	D3DXVECTOR2 GetCamPos() { return camPos; }

	int GetWidth() { return this->ScreenWidth; }
	int GetHeight() { return this->ScreenHeight; }
	RECT GetCamBound();
	void GetPlayerPos(float& x, float& y) { x = playerPos.x; y = playerPos.y;}

	bool isContain(RECT rect);
	bool isContain(LPGAMEOBJECT obj);

	void SetCamMap(float mapWidth, float mapHeight);
	void Update(float x, float y);
};


