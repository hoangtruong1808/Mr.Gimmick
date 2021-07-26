#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "MapGame.h"
#include "Gimmick.h"
#include "PlatformsMoving.h"
#include "Camera.h"
#include "Grid.h"
#include "Quadtree.h"
#include "MagicStar.h"

class CPlayScene : public CScene
{
protected:
	CGimmick* player;					// A play scene has to have player, right? 
	MapGame* map;
	CCamera* camera;
	CQuadtree* quadtree;
	CMagicStar* star;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> objectsquadtree;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_ZONES(string line);
	void _ParseSection_SOUNDS(string line);
	vector<CZone*> zones;
	float zone_l = -9999.0f;
	float zone_t = 9999.0f;
	float zone_r = 9999.0f;
	float zone_b = -9999.0f;
	float revival_x = 0;
	float revival_y = 0;


public:	
	
	CPlayScene(int id, LPCWSTR filePath);
	void GetZonePosition(float& l, float& t, float& r, float& b);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void AddObject(CGameObject* a);
	void SetZone();
	CGimmick* GetPlayer() { return player; }
	CMagicStar* GetStar() { return star; }
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};
