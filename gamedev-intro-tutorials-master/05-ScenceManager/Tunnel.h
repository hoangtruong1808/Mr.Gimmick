#pragma once
#include "GameObject.h"

#define TUNNEL_ANI_HEAD		0
#define TUNNEL_ANI_BODY		1
#define TUNNEL_ANI_END	2

#define TUNNEL_BBOX_HEIGHT	32
#define TUNNEL_HEAD_BBOX_WIDTH	32
#define TUNNEL_BODY_BBOX_WIDTH	80
#define TUNNEL_END_BBOX_WIDTH	32

class CTunnel :	public CGameObject
{
	int type;
public:
	CTunnel(int t);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void Render();
	void SetState(int state) {};
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};

