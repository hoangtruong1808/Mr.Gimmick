
#include "Game.h"
#include "MapGame.h"
#include "Textures.h"
#include <iostream>
#include <fstream>
#include "Utils.h"




void MapGame::LoadMap(wstring mappath)
{

	ifstream f;
	f.open(mappath);
	int data = 0;

	for (int i = 0; i < game_map.MAP_Y; i++)
	{
		for (int j = 0; j < game_map.MAP_X; j++)
		{
			f >> data;
			game_map.ArrayTile.push_back(data);
			DebugOut(L"[ERROR] data %d not found!\n", data);
		}
	}
	f.close();
}



void MapGame::DrawTiles(int val, float x, float y)
{
	float x0, y0;

	x0 = int((val - 1) % game_map.TILE_SIZE_X) * 16;
	y0 =int((val - 1) / game_map.TILE_SIZE_X) * 16;

	CGame::GetInstance()->Draw(x, y, TileSetImg, x0, y0, x0 + 16, y0 + 16);
}




void MapGame::Render()
{
	int x0, y0;
	int temp = 0;
	if (cam_y <= (this->GetMapHeight() - 16) )
		cam_y += 16;

	for (int i = 0; i < screenHeight/16	 ; i++)
	{
		for (int j = 0; j < screenWidth/16; j++)
		{
				x0 = j * 16 + int(cam_x/16) * 16; // lấy vị trí chia hết cho 16
				y0 = int(cam_y/16) * 16 - i*16;
				
				temp = int(game_map.MAP_Y - y0/16)* game_map.MAP_X  + int(x0/16) ; // lấy vị trí  tile   
				int val = game_map.ArrayTile[temp];
				DrawTiles(val, x0, y0);
				
					
		}
	}

}
float MapGame::GetMapWidth() {
	return game_map.MAP_X * TILE_WIDTH;
}

float MapGame::GetMapHeight() {
	return game_map.MAP_Y * TILE_HEIGHT;
}