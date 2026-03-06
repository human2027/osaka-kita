#include "MapLoader.h"
#include "InitialValue.h"   
#include <fstream>
#include <sstream>
#include <iostream>

bool MapLoader::LoadCSV(const std::string& path, std::vector<Tile>& outTiles)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return false;
    }

    outTiles.clear();

    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty()) continue;

        int typeNum = std::stoi(line);

        Tile tile;
        tile.type = static_cast<TileType>(typeNum);

        //タイプに応じて「固定値」を設定
        switch (tile.type)
        {
        case TileType::Damage:
            tile.value = Tile_Damage_Value;  
            break;
        case TileType::Heal:
            tile.value = Tile_Heal_Value;  
            break;
        case TileType::Item:
        case TileType::Event:
        case TileType::Normal:
        default:
            tile.value = 0;                  // 数値は使わない
            break;
        }

        outTiles.push_back(tile);
    }

    return true;
}
