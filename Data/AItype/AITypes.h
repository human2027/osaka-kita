#pragma once
#include "MapBase.h" 

struct TileInfoForAI
{
    int index = -1;
    TileType type = TileType::Normal;
    int value = 0;
    int itemId = -1;
};