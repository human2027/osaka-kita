#include "MapLoader.h"
#include "InitialValue.h"   // ★ さっきの定数を使う
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
            tile.value = TILE_DAMAGE_VALUE;  
            break;
        case TileType::Heal:
            tile.value = TILE_HEAL_VALUE;  
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
