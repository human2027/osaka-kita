#pragma once
#include <memory>
#include <string>
#include <vector>

#include "MapBase.h"
#include "MapLoader.h"
#include "CharacterBase.h" 
class MapManager
{
public:
    MapManager() = default;
    ~MapManager() = default;

    // マップ切り替え
    void SetMap(std::shared_ptr<MapBase> map)
    {
        currentMap = std::move(map);
    }

    // CSV読み込み → MapBaseに反映
    bool LoadCSV(const std::string& path);

    // マップ初期化
    void Initialize();

    //
    void Draw();

    // タイル取得（範囲外はダミーを返す）
    const Tile& GetTile(int index) const;

    // マスの効果をキャラクターに適用
    void ApplyTileEffect(CharacterBase& character);

    // ゴール判定
    bool IsGoal(int pos) const;

    // 目標地点（ゴール）
    int GetGoal() const
    {
        return currentMap ? currentMap->GetGoal() : 0;
    }

    // マップサイズ
    int GetSize() const
    {
        return currentMap ? currentMap->GetSize() : 0;
    }

    void SetItemTile(int index, ItemType itemType, int value = 0);
private:
    std::shared_ptr<MapBase> currentMap;
};