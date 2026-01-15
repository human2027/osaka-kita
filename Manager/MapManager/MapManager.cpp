#include "MapManager.h"
#include "DxLib.h"

void MapManager::Initialize()
{
    if (currentMap)
        currentMap->Initialize();
}

bool MapManager::LoadCSV(const std::string& path)
{
    if (!currentMap) return false;

    std::vector<Tile> tiles;
    if (!MapLoader::LoadCSV(path, tiles))
        return false;

    currentMap->SetTiles(tiles);
    return true;
}

void MapManager::ApplyTileEffect(CharacterBase& character)
{
    if (currentMap)
        currentMap->ApplyTileEffect(character);
}

bool MapManager::IsGoal(int pos) const
{
    if (!currentMap) return false;
    return currentMap->IsGoal(pos);
}

void MapManager::Draw()
{
    if (!currentMap) return;

    for (int i = 0; i < currentMap->GetSize(); i++)
    {
        const Tile& t = currentMap->GetTile(i);
        unsigned int color = 0xffffff;

        switch (t.type)
        {
        //ここら辺は仮なので後で変える部分(予定)
        case TileType::Damage: color = GetColor(255, 50, 50); break;
        case TileType::Heal:   color = GetColor(50, 255, 50); break;
        case TileType::Item:   color = GetColor(200, 200, 50); break;
        default:               color = GetColor(255, 255, 255); break;
        }

        DrawBox(20 + i * 12, 400, 30 + i * 12, 420, color, TRUE);
    }
}

const Tile& MapManager::GetTile(int index) const
{
    static Tile empty;  // 範囲外用のダミー

    if (!currentMap) return empty;
    if (index < 0 || index >= currentMap->GetSize()) return empty;

    return currentMap->GetTile(index);
}