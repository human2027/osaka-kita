#include "MapManager.h"
#include "DxLib.h"
#include "InitialValue.h"
#include "AnimationManager.h"

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
    // 通常画面ではマップを常時表示しない
    // TABキー押下時などに DrawLargeMap() を呼ぶ
}

const Tile& MapManager::GetTile(int index) const
{
    static Tile empty = []()
        {
            Tile t{};
            t.type = TileType::Normal;
            t.value = 0;
            t.itemType = ItemType::Item_none;
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

void MapManager::DrawLargeMap(
    int playerPos,
    int aiPos,
    const AnimationManager& animationManager
) const
{
    if (!currentMap) return;

    // ===== 背景を暗くする =====
    // TABマップ表示中に、背後のゲーム画面を少し暗くして
    // マップUIを「開いている画面」っぽく見せる
    SetDrawBlendMode(DX_BLENDMODE_ALPHA, 140);

    DrawBox(
        0,
        0,
        static_cast<int>(Window_screen_W),
        static_cast<int>(Window_screen_H),
        GetColor(0, 0, 0),
        TRUE
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

    const int mapTileSize = currentMap->GetSize();

    // 0 = START, 1～Goal_pos = マス表示
    // Goal_pos が 50 なら、0～50 の51個を描画する
    const int drawSize = Goal_pos + 1;

    if (drawSize <= 0) return;

    // ===== 大きいマップUI設定 =====
    constexpr int columns = 10;      // 横10マスで折り返し
    constexpr int tileSize = 56;     // 1マスの大きさ
    constexpr int gap = 6;           // マス間の隙間

    constexpr int startX = 620;
    constexpr int startY = 120;

    const int rows = (drawSize + columns - 1) / columns;

    constexpr int panelPadding = 30;

    const int panelX1 = startX - panelPadding;
    const int panelY1 = startY - 70;
    const int panelX2 = startX + columns * (tileSize + gap) + panelPadding;
    const int panelY2 = startY + rows * (tileSize + gap) + panelPadding;

    // ===== 背景パネル =====
    DrawBox(
        panelX1,
        panelY1,
        panelX2,
        panelY2,
        GetColor(0, 0, 0),
        TRUE
    );

    DrawBox(
        panelX1,
        panelY1,
        panelX2,
        panelY2,
        GetColor(255, 255, 255),
        FALSE
    );

    // ===== タイトル =====
    DrawString(
        panelX1 + 20,
        panelY1 + 20,
        "TACTICAL MAP",
        GetColor(255, 255, 255)
    );

    DrawString(
        panelX1 + 220,
        panelY1 + 20,
        "P = Player   A = AI",
        GetColor(180, 180, 180)
    );

    for (int i = 0; i < drawSize; ++i)
    {
        Tile t{};

        if (i < mapTileSize)
        {
            t = currentMap->GetTile(i);
        }
        else
        {
            t.type = TileType::Normal;
            t.value = 0;
            t.itemType = ItemType::Item_none;
        }

        const int col = i % columns;
        const int row = i / columns;

        const int x1 = startX + col * (tileSize + gap);
        const int y1 = startY + row * (tileSize + gap);
        const int x2 = x1 + tileSize;
        const int y2 = y1 + tileSize;

        unsigned int color = GetColor(230, 230, 230);

        if (i == 0)
        {
            color = GetColor(120, 180, 255); // START
        }
        else if (i == Goal_pos)
        {
            color = GetColor(255, 180, 80);  // GOAL
        }
        else
        {
            switch (t.type)
            {
            case TileType::Damage:
                color = GetColor(255, 80, 80);
                break;

            case TileType::Heal:
                color = GetColor(80, 255, 80);
                break;

            case TileType::Item:
                color = GetColor(230, 220, 60);
                break;

            case TileType::Event:
                color = GetColor(120, 120, 255);
                break;

            case TileType::Normal:
            default:
                color = GetColor(230, 230, 230);
                break;
            }
        }

        // ===== マス本体 =====
        DrawBox(x1, y1, x2, y2, color, TRUE);
        DrawBox(x1, y1, x2, y2, GetColor(40, 40, 40), FALSE);

        // ===== 表示用マス番号 =====
        if (i == 0)
        {
            DrawString(
                x1 + 4,
                y1 + 4,
                "START",
                GetColor(0, 0, 0)
            );
        }
        else if (i == Goal_pos)
        {
            DrawString(
                x1 + 4,
                y1 + 4,
                "GOAL",
                GetColor(0, 0, 0)
            );

            DrawFormatString(
                x1 + 4,
                y1 + 24,
                GetColor(0, 0, 0),
                "%d",
                i
            );
        }
        else
        {
            DrawFormatString(
                x1 + 4,
                y1 + 4,
                GetColor(0, 0, 0),
                "%d",
                i
            );
        }

        const bool isPlayer = (i == playerPos);
        const bool isAI = (i == aiPos);

        // ===== プレイヤー位置 =====
        if (isPlayer)
        {
            const int offsetX = isAI ? -14 : 0;
            const int offsetY = isAI ? -14 : 0;

            const int centerX = x1 + tileSize / 2 + offsetX;
            const int centerY = y1 + tileSize / 2 + offsetY;

            animationManager.DrawPlayerMapChar(centerX, centerY);

            DrawBox(
                x1 - 4,
                y1 - 4,
                x2 + 4,
                y2 + 4,
                GetColor(0, 180, 255),
                FALSE
            );
        }

        // ===== AI位置：敵マップキャラアニメ =====
        if (isAI)
        {
            // 同じマスにいる時は少しずらす
            const int offsetX = isPlayer ? 14 : 0;
            const int offsetY = isPlayer ? 14 : 0;

            const int centerX = x1 + tileSize / 2 + offsetX;
            const int centerY = y1 + tileSize / 2 + offsetY;

            animationManager.DrawEnemyMapChar(centerX, centerY);

            // AIの現在位置を強調する枠だけ残す
            DrawBox(
                x1 - 7,
                y1 - 7,
                x2 + 7,
                y2 + 7,
                GetColor(255, 80, 80),
                FALSE
            );
        }
    }
}