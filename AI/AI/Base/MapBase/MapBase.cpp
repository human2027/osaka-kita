#include "MapBase.h"
#include "CharacterBase.h"

const Tile& MapBase::GetTile(int index) const
{
    static Tile empty;  // 範囲外用の空タイル

    if (index < 0 || index >= (int)tiles.size())
        return empty;

    return tiles[index];
}

void MapBase::ApplyTileEffect(CharacterBase& character)
{
    int pos = character.GetPos();

    if (pos < 0 || pos >= (int)tiles.size())
        return;

    const Tile& t = tiles[pos];

    switch (t.type)
    {
    case TileType::Damage:
        character.AddHP(-t.value);
        break;

    case TileType::Heal:
        character.AddHP(t.value);
        break;

    case TileType::Item:
       
        break;

    case TileType::Event:
        break;

    case TileType::Normal:
    default:
        break;
    }
}
