#include "Quadtree.h"
#include "Utils.h"
#include <algorithm>


void CQuadtree::Clear()
{
    // Clear all nodes
    if (nodes)
    {
        for (int i = 0; i < 4; i++)
        {
            nodes[i]->Clear();
            delete nodes[i];
            nodes[i] = nullptr;
        }
        delete[] nodes;
        nodes = nullptr;
    }

    // Clear current Quadtree
    objects_list->clear();
    delete objects_list;
}

bool CQuadtree::IsContain(LPGAMEOBJECT obj)
{
    RECT Rect;
    float left, top, right, bottom;
    obj->GetBoundingBox(left, top, right, bottom);
    Rect.left = int(left);
    Rect.top = int(top);
    Rect.right = int(right);
    Rect.bottom = int(bottom);

    return !(Rect.left > region->width || Rect.right < region->x || Rect.top < region->y || Rect.bottom > region->height);

}

bool CQuadtree::IsContain(RECT obj)
{

    return !(obj.left > region->width || obj.right < region->x || obj.top < region->y || obj.bottom > region->height);

}

void CQuadtree::Split()
{
    nodes = new CQuadtree * [4];

    nodes[0] = new CQuadtree(level + 1, region->x, region->y, region->x + (region->width - region->x) / 2, region->y + (region->height - region->y) / 2); // left bottom
    nodes[1] = new CQuadtree(level + 1, region->x + (region->width - region->x) / 2, region->y, region->width, region->y + (region->height - region->y) / 2); // right bottom
    nodes[2] = new CQuadtree(level + 1, region->x, region->y + (region->height - region->y) / 2, region->x + (region->width - region->x) / 2, region->height);
    nodes[3] = new CQuadtree(level + 1, region->x + (region->width - region->x) / 2, region->y + (region->height - region->y) / 2, region->width, region->height);
}

void CQuadtree::Insert(LPGAMEOBJECT obj)
{
    // Insert entity into corresponding nodes
    if (nodes)
    {
        if (nodes[0]->IsContain(obj))
            nodes[0]->Insert(obj);
        if (nodes[1]->IsContain(obj))
            nodes[1]->Insert(obj);
        if (nodes[2]->IsContain(obj))
            nodes[2]->Insert(obj);
        if (nodes[3]->IsContain(obj))
            nodes[3]->Insert(obj);

        return; // Return here to ignore rest.
    }

    // Insert entity into current quadtree
    if (this->IsContain(obj))
    {
        objects_list->push_back(obj);
    }
    // Split and move all objects in list into it’s corresponding nodes
    if (objects_list->size() > MAX_OBJECT_IN_REGION && level < MAX_LEVEL)
    {
        Split();

        while (!objects_list->empty())
        {
            if (nodes[0]->IsContain(objects_list->back()))
                nodes[0]->Insert(objects_list->back());
            if (nodes[1]->IsContain(objects_list->back()))
                nodes[1]->Insert(objects_list->back());
            if (nodes[2]->IsContain(objects_list->back()))
                nodes[2]->Insert(objects_list->back());
            if (nodes[3]->IsContain(objects_list->back()))
                nodes[3]->Insert(objects_list->back());

            objects_list->pop_back();
        }
    }
}

void CQuadtree::Retrieve(vector<LPGAMEOBJECT>* return_objects_list, LPGAMEOBJECT obj)
{
    if (nodes)
    {
        if (nodes[0]->IsContain(obj))
            nodes[0]->Retrieve(return_objects_list, obj);
        if (nodes[1]->IsContain(obj))
            nodes[1]->Retrieve(return_objects_list, obj);
        if (nodes[2]->IsContain(obj))
            nodes[2]->Retrieve(return_objects_list, obj);
        if (nodes[3]->IsContain(obj))
            nodes[3]->Retrieve(return_objects_list, obj);

        return; // Return here to ignore rest.
    }

    // Add all entities in current region into return_objects_list
    if (this->IsContain(obj))
    {
        for (size_t i = 0; i < objects_list->size(); i++)
        {
            if (obj == objects_list->at(i))
                continue;
            return_objects_list->push_back(objects_list->at(i));
        }
    }

}

vector<LPGAMEOBJECT> CQuadtree::SortDuplicateObj(vector<LPGAMEOBJECT> obj)
{
    sort(obj.begin(), obj.end());
    obj.erase(unique(obj.begin(), obj.end()), obj.end());
    return obj;
}