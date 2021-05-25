#pragma once
#include <algorithm>
#include "GameObject.h"
#include "MapGame.h"
#include "Camera.h"
#include "Brick.h"
#include "Portal.h"
#include "Gimmick.h"


using namespace std;

#define CELL_SIZE 272

class CGrid
{
	static CGrid* __instance;
	vector<LPGAMEOBJECT>** Cell;

	int Rows;
	int Columns;
public:

	CGrid(float map_w, float map_h);
	~CGrid();
	void AddObject(LPGAMEOBJECT obj);
	void RemoveObject(LPGAMEOBJECT obj);
	void Update(vector<LPGAMEOBJECT> obj);
	void UnloadGrid();
	vector<LPGAMEOBJECT> SortDuplicateObj(vector<LPGAMEOBJECT> obj);

	vector<LPGAMEOBJECT> GetActiveObj();


};



