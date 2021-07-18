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


class CPlayScene : public CScene
{
protected:
	CGimmick* player;					// A play scene has to have player, right? 
	MapGame* map;
	CCamera* camera;
	CQuadtree* quadtree;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> objectsquadtree;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);


public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void AddObject(CGameObject* a);
	CGimmick* GetPlayer() { return player; }

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
