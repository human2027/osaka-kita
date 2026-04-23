#include "MapBase.h"
#include "CharacterBase.h"

const Tile& MapBase::GetTile(int index) const
{
    static Tile empty{};
    if (index < 0 || index >= static_cast<int>(tiles.size()))
        return empty;

    return tiles[index];
}

void MapBase::ApplyTileEffect(CharacterBase& character)
{
    const int pos = character.GetPos();
    if (pos < 0 || pos >= static_cast<int>(tiles.size()))
        return;

    Tile& tile = tiles[pos];

    switch (tile.type)
    {
    case TileType::Damage:
        character.SetHP(character.GetHP() - tile.value);
        break;

    case TileType::Heal:
        character.AddHP(tile.value);
        break;

    case TileType::Item:
        if (!character.HasItem() && tile.itemType != ItemType::Item_none)
        {
            character.SetHeldItem(tile.itemType);

            // èEÇ¡ÇΩÇÁè¡Ç∑
            tile.type = TileType::Normal;
            tile.itemType = ItemType::Item_none;
            tile.value = 0;
        }
        break;

    case TileType::Event:
    case TileType::Normal:
    default:
        break;
    }
}

void MapBase::SetTile(int index, const Tile& tile)
{
    if (index < 0 || index >= static_cast<int>(tiles.size()))
        return;

    tiles[index] = tile;
}

void MapBase::SetItemTile(int index, ItemType itemType, int value)
{
    if (index < 0 || index >= static_cast<int>(tiles.size()))
        return;

    tiles[index].type = TileType::Item;
    tiles[index].itemType = itemType;
    tiles[index].value = value;
}