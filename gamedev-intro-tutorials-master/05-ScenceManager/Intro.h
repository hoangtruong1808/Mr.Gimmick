#pragma once
#include "Game.h"
#include "Textures.h"
#include "MapGame.h"
#include "Scence.h"
#include "GameObject.h""

#define INTRO_ANI	10001
#define INTRO_WIDTH		128
#define INTRO_HEIGHT	80


class CIntro : public CScene
{
protected:	
	float intro_x; 
	float intro_y;
	MapGame* map = nullptr;

	DWORD start = 0;

	vector<LPGAMEOBJECT> objects;
	vector<LPGAMEOBJECT> objectsquadtree;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_MAP(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);


public:
	CIntro(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	void SetCamPos();
	void SetIntroPos();

	//friend class CPlayScenceKeyHandler;
};

class CIntroKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE* states) {};
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CIntroKeyHandler(CScene* s) :CScenceKeyHandler(s) {};
};

