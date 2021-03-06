#pragma once

#include <Windows.h>
#include <d3dx9.h>
#include <vector>

#include "Sprites.h"
#include "Animations.h"


using namespace std;

#define ID_TEX_BBOX -100		// special texture to draw object bounding box

class CGameObject;
typedef CGameObject* LPGAMEOBJECT;

struct CCollisionEvent;
typedef CCollisionEvent* LPCOLLISIONEVENT;
struct CCollisionEvent
{
	LPGAMEOBJECT obj;
	float t, nx, ny;

	float dx, dy;		// *RELATIVE* movement distance between this object and obj

	CCollisionEvent(float t, float nx, float ny, float dx = 0, float dy = 0, LPGAMEOBJECT obj = NULL)
	{
		this->t = t;
		this->nx = nx;
		this->ny = ny;
		this->dx = dx;
		this->dy = dy;
		this->obj = obj;
	}

	static bool compare(const LPCOLLISIONEVENT& a, LPCOLLISIONEVENT& b)
	{
		return a->t < b->t;
	}
};


class CGameObject
{
public:

	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	int nx;

	float cam_x = 0.0f;
	float cam_y = 0.0f;

	float screenWidth = 0.0f;
	float screenHeight = 0.0f;

	int state;

	DWORD dt;

	LPANIMATION_SET animation_set;

public:
	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetPosX(float x) { this->x = x; }
	void SetPosY(float y) { this->y = y; }
	void SetVX(float vx) { this->vx = vx; }
	void SetVY(float vy) { this->vy = vy; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void SetNX(int nx) { this->nx = nx; }
	void GetPosition(float& x, float& y) { x = this->x; y = this->y; }
	float GetPosition_x() { return x; }
	float GetPosition_y() { return y; }
	float GetPosition_vx() { return vx; }
	float GetPosition_vy() { return vy; }
	float GetPosition_dx() { return dx; }
	float GetPosition_dy() { return dy; }
	void GetSpeed(float& vx, float& vy) { vx = this->vx; vy = this->vy; }

	int GetState() { return this->state; }

	void RenderBoundingBox();

	void SetAnimationSet(LPANIMATION_SET ani_set) { animation_set = ani_set; }

	void SetCamera(float camx, float camy)
	{
		this->cam_x = camx;
		this->cam_y = camy;
	};
	void SetScreen(float width, float height)
	{
		this->screenHeight = height;
		this->screenWidth = width;
	}

	bool isContain(CGameObject* obj);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT>* coObjects, vector<LPCOLLISIONEVENT>& coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT>& coEvents,
		vector<LPCOLLISIONEVENT>& coEventsResult,
		float& min_tx,
		float& min_ty,
		float& nx,
		float& ny,
		float& rdx,
		float& rdy);

	CGameObject();

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state) { this->state = state; };
	~CGameObject();
};