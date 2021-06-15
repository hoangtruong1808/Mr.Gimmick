#pragma once
#include <Windows.h>
#include "GameObject.h"

#define MAX_LEVEL                3
#define MAX_OBJECT_IN_REGION     8


typedef struct Rect
{
    int x, y;
    int width, height;
};

class CQuadtree
{
private:
    int       level;
    Rect* region;
    vector<LPGAMEOBJECT>* objects_list;
    CQuadtree** nodes;

public:
    
   
    CQuadtree(int level, int x, int y, int width, int height)
    {
        this->level = level;
        region = new Rect;
        objects_list = new  vector<LPGAMEOBJECT>;
        region->x =x;
        region->y = y;
        region->width = width;
        region->height = height;
       
    };
    ~CQuadtree() {};

    void           Clear();
    void           Insert(LPGAMEOBJECT obj);
    bool           IsContain(LPGAMEOBJECT obj);
    bool           IsContain(RECT obj);
    void           Split(); 
    void Retrieve(vector<LPGAMEOBJECT>* return_objects_list, LPGAMEOBJECT obj);
    vector<LPGAMEOBJECT> SortDuplicateObj(vector<LPGAMEOBJECT> obj);

    
};

