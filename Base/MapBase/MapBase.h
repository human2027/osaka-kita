#pragma once
#include <vector>
#include <string>
#include "InitialValue.h"

enum class TileType {
    Normal = 0,
    Damage,
    Heal,
    Item,
    Event
};

struct Tile {
    TileType type = TileType::Normal;
    int value = 0;
};

class CharacterBase;

class MapBase {
public:
    virtual ~MapBase() = default;

    virtual int GetSize() const {
        return static_cast<int>(tiles.size());
    }

    //Goal_posを返す
    virtual int GetGoal() const {
        return Goal_pos;
    }

    virtual const Tile& GetTile(int index) const;
    virtual void ApplyTileEffect(CharacterBase& character);

    //ゴール到達
    virtual bool IsGoal(int pos) const {
        return pos >= Goal_pos;
    }

    // マップ内容は派生で決める
    virtual void Initialize() = 0;

    void SetTiles(const std::vector<Tile>& newTiles)
    {
        tiles = newTiles;
    }

protected:
    std::vector<Tile> tiles;
};
