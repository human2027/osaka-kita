#include "NormalMap.h"
#include "MapLoader.h"
#include "InitialValue.h"
#include <iostream>    

//コンストラクタ
NormalMap::NormalMap()
{
    path = "Data/Map.csv";
}

// CSVパス指定版
NormalMap::NormalMap(const std::string& csvPath)
    : path(csvPath)
{
}

//CSV読み込み
void NormalMap::Initialize()
{
    std::vector<Tile> loadedTiles;

    // CSV読み込み
    if (!MapLoader::LoadCSV(path, loadedTiles))
    {
        return;
    }

    tiles = loadedTiles;

    std::cout << "マップ読み込み成功: " << path
        << "  タイル数 = " << tiles.size()
        << "  ゴール = " << Goal_pos << "\n";
}
