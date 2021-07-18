#include <iostream>
#include <fstream>

#include "PlayScence.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "SlopeBrick.h"
#include "FallenBomb.h"
#include "Fire.h"
#include "SmallPinion.h"
#include "CPinion.h"
#include "Worm.h"
#include "Cord.h"
#include "Swing.h"
#include "Window.h"
#include "Tunnel.h"
#include "Tube.h"
#include "Bullet.h"
#include "Gun.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP	7

#define OBJECT_TYPE_GIMMICK	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_PLATFORMSMOVING		4
#define OBJECT_TYPE_SLOPE	5

#define OBJECT_TYPE_FALLENBOMB	7
#define OBJECT_TYPE_FIRE	8
#define OBJECT_TYPE_SMALLPINION	9
#define OBJECT_TYPE_PINION	10
#define OBJECT_TYPE_CORD	11
#define OBJECT_TYPE_WORM	12
#define OBJECT_TYPE_SWING	13
#define OBJECT_TYPE_TUNNEL	14
#define OBJECT_TYPE_TUBE	15
#define OBJECT_TYPE_GUN	16
#define OBJECT_TYPE_BULLET	17
#define OBJECT_TYPE_WINDOW	20

#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations* animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CPlayScene::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets* animation_sets = CAnimationSets::GetInstance();

	CGameObject* obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_GIMMICK:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CGimmick();
		player = (CGimmick*)obj;

		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_BRICK:
	{
		if (tokens.size() < 6) return;
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());

		obj = new CBrick(width, height);
		if (tokens.size() > 6)
		{
			int brick_state = atoi(tokens[6].c_str());
			DebugOut(L"[INFO] brick_state: %d\n", brick_state);
			obj->SetState(brick_state);
		}

	}
	break;
	case OBJECT_TYPE_PLATFORMSMOVING:
	{
		if (tokens.size() < 9) return;
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		obj = new CPlatformsMoving(width, height);
		CPlatformsMoving* pm = (CPlatformsMoving*)obj;
		int start = atoi(tokens[6].c_str());
		int end = atoi(tokens[7].c_str());
		pm->SetStart(start);
		pm->SetEnd(end);
		int id_set_state = atoi(tokens[8].c_str());
		obj->SetState(id_set_state);
	}
	break;
	case OBJECT_TYPE_WORM:
	{

		obj = new CWorm();
		CWorm* pm = (CWorm*)obj;
		int start = atoi(tokens[4].c_str());
		int end = atoi(tokens[5].c_str());
		pm->SetStart(start);
		pm->SetEnd(end);
	}
	break;
	case OBJECT_TYPE_SLOPE:
	{
		if (tokens.size() < 6) return;
		float width = atof(tokens[4].c_str());
		float height = atof(tokens[5].c_str());
		obj = new CSlopeBrick(width, height);
		int brick_state = atoi(tokens[6].c_str());
		obj->SetState(brick_state);
	}
	break;

	case OBJECT_TYPE_FALLENBOMB:
	{
		obj = new CFallenBomb();
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	case OBJECT_TYPE_FIRE:
	{
		obj = new CFire();
		break;
	}
	case OBJECT_TYPE_SMALLPINION:
	{
		obj = new CSmallPinion();
		break;
	}
	case OBJECT_TYPE_PINION:
	{
		int t = atof(tokens[4].c_str());
		obj = new CPinion(t);
		break;
	}
	case OBJECT_TYPE_CORD:
	{
		int t = atof(tokens[4].c_str());
		obj = new CCord(t);
		break;
	}
	case OBJECT_TYPE_SWING:
	{
		obj = new CSwing();
		break;
	}
	case OBJECT_TYPE_TUNNEL:
	{
		int t = atof(tokens[4].c_str());
		obj = new CTunnel(t);
		break;
	}
	case OBJECT_TYPE_TUBE:
	{
		int t = atof(tokens[4].c_str());
		obj = new CTube(t);
		break;
	}
	case OBJECT_TYPE_WINDOW:
	{
		obj = new CWindow();
		break;
	}
	case OBJECT_TYPE_GUN:
	{
		obj = new CGun();
		break;
	}
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);
}

void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines
	int TileSetID = atoi(tokens[0].c_str());
	int TotalRowsOfMap = atoi(tokens[1].c_str());
	int TotalColumnsOfMap = atoi(tokens[2].c_str());
	int TotalRowsOfTileSet = atoi(tokens[3].c_str());
	int TotalColumnsOfTileSet = atoi(tokens[4].c_str());
	wstring mapMatrixPath = ToWSTR(tokens[5]);

	this->map = new MapGame(TileSetID, TotalRowsOfMap, TotalColumnsOfMap, TotalRowsOfTileSet, TotalColumnsOfTileSet, mapMatrixPath);
	DebugOut(L"[INFO] Load map OK\n");
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));


	camera = CCamera::GetInstance();
	camera->SetCamMap(map->GetMapWidth(), map->GetMapHeight());
	quadtree = new CQuadtree(0, 0, 0, map->GetMapWidth(), map->GetMapHeight());

}

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	quadtree = new CQuadtree(0, 0, 0, map->GetMapWidth(), map->GetMapHeight());
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (camera->isContain(objects[i]))
		{
			coObjects.push_back(objects[i]);
			if (!dynamic_cast<CGimmick*>(objects[i]))
				quadtree->Insert(objects[i]);
		}
	}



	for (size_t i = 0; i < coObjects.size(); i++)
	{
		if (dynamic_cast<CGimmick*>(coObjects[i]))
		{
			vector<LPGAMEOBJECT> coObjects_quadtree;
			quadtree->Retrieve(&coObjects_quadtree, coObjects[i]);
			coObjects[i]->Update(dt, &coObjects_quadtree);
		}
		else coObjects[i]->Update(dt, &coObjects);
	}


	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario

	if (map != NULL)
	{
		float cx, cy;
		player->GetPosition(cx, cy);

		CGame* game = CGame::GetInstance();
		camera->Update(cx, cy);
		D3DXVECTOR2 camPos = camera->GetCamPos();
		map->SetCamera(camPos.x, camPos.y);
		map->SetScreen(game->GetScreenWidth(), game->GetScreenHeight());
	}

	quadtree->Clear();

}
void CPlayScene::AddObject(CGameObject* a) {
	objects.push_back(a);
}

void CPlayScene::Render()
{
	if (map)
		map->Render();

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CGimmick* gimmick = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		gimmick->SetState(GIMMICK_STATE_JUMP);
		gimmick->JUMP();
		break;
	case DIK_A:
		break;
	}
}

void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);

	CGimmick* gimmick = ((CPlayScene*)scence)->GetPlayer();
	switch (KeyCode)
	{
	case DIK_SPACE:
		gimmick->STOP_JUMP();
		break;
	case DIK_A:
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE* states)
{
	CGame* game = CGame::GetInstance();
	CGimmick* gimmick = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (gimmick->GetState() == GIMMICK_STATE_DIE) return;
	else if (gimmick->GetState() != GIMMICK_STATE_JUMP && gimmick->GetState() != GIMMICK_STATE_JUMP_RIGHT && gimmick->GetState() != GIMMICK_STATE_JUMP_LEFT)
	{
		gimmick->SetState(GIMMICK_STATE_IDLE);

		if (game->IsKeyDown(DIK_RIGHT))
			gimmick->SetState(GIMMICK_STATE_WALKING_RIGHT);
		else if (game->IsKeyDown(DIK_LEFT))
			gimmick->SetState(GIMMICK_STATE_WALKING_LEFT);
	}
	else
	{

		if (game->IsKeyDown(DIK_RIGHT))
			gimmick->SetState(GIMMICK_STATE_JUMP_RIGHT);
		else if (game->IsKeyDown(DIK_LEFT))
			gimmick->SetState(GIMMICK_STATE_JUMP_LEFT);
	}
}