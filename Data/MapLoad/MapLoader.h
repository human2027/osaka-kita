#pragma once
#include <string>
#include <vector>
#include "MapBase.h"

class MapLoader
{
public:
    static bool LoadCSV(const std::string& path, std::vector<Tile>& outTiles);
};