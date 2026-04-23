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

    const int size = currentMap->GetSize();
    for (int i = 0; i < size; i++)
    {
        const Tile& t = currentMap->GetTile(i);
        unsigned int color = GetColor(255, 255, 255);

        switch (t.type)
        {
        case TileType::Damage: color = GetColor(255, 50, 50);  break;
        case TileType::Heal:   color = GetColor(50, 255, 50);  break;
        case TileType::Item:   color = GetColor(200, 200, 50); break;
        default: break;
        }

        DrawBox(20 + i * 12, 400, 30 + i * 12, 420, color, TRUE);
    }
}

const Tile& MapManager::GetTile(int index) const
{
    static Tile empty = []()
        {
            Tile t{};
            t.type = TileType::Normal;
            t.value = 0;
            return t;
        }();

    if (!currentMap) return empty;

    const int size = currentMap->GetSize();
    if (index < 0 || index >= size) return empty;

    return currentMap->GetTile(index);
}

void MapManager::SetItemTile(int index, ItemType itemType, int value)
{
    if (!currentMap) return;

    const int size = currentMap->GetSize();
    if (index < 0 || index >= size) return;

    Tile tile = currentMap->GetTile(index);
    tile.type = TileType::Item;
    tile.itemType = itemType;
    tile.value = value;

    currentMap->SetTile(index, tile);
}