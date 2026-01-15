#pragma once
#include <vector>
#include <string>
#include <fstream>     
#include <sstream>    
#include "MapBase.h"

class MapLoader
{
public:
    static bool LoadCSV(const std::string& path, std::vector<Tile>& outTiles);
};
