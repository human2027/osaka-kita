#include "StatusPanelUI.h"

#include "DxLib.h"
#include "UIConfig.h"
#include "ItemType.h"
#include "InitialValue.h"

namespace
{
    int ClampInt(int value, int minValue, int maxValue)
    {
        if (value < minValue) return minValue;
        if (value > maxValue) return maxValue;
        return value;
    }

    void DrawTextShadow(int x, int y, const char* text, int color, int shadowColor, int fontHandle)
    {
        DrawStringToHandle(x + 2, y + 2, text, shadowColor, fontHandle);
        DrawStringToHandle(x, y, text, color, fontHandle);
    }

    void DrawFormatTextShadow(
        int x,
        int y,
        int color,
        int shadowColor,
        int fontHandle,
        const char* format,
        int value1,
        int value2
    )
    {
        DrawFormatStringToHandle(x + 2, y + 2, shadowColor, fontHandle, format, value1, value2);
        DrawFormatStringToHandle(x, y, color, fontHandle, format, value1, value2);
    }

    void DrawHpGauge(
        int x,
        int y,
        int width,
        int height,
        const char* name,
        int hp,
        int maxHp,
        int nameColor,
        int hpColor,
        int titleFont,
        int textFont
    )
    {
        if (maxHp <= 0)
        {
            maxHp = 1;
        }

        hp = ClampInt(hp, 0, maxHp);

        const int white = GetColor(255, 255, 255);
        const int gray = GetColor(110, 110, 110);
        const int dark = GetColor(18, 18, 18);
        const int shadow = GetColor(0, 0, 0);

        const float hpRate = static_cast<float>(hp) / static_cast<float>(maxHp);
        const int fillWidth = static_cast<int>(width * hpRate);

        // 名前
        DrawTextShadow(
            x,
            y,
            name,
            nameColor,
            shadow,
            titleFont
        );

        // HP数値
        DrawFormatStringToHandle(
            x + width - 64,
            y + 2,
            white,
            textFont,
            "%3d",
            hp
        );

        const int barY = y + 26;

        // HPバー背景
        DrawBox(
            x,
            barY,
            x + width,
            barY + height,
            dark,
            TRUE
        );

        // HPバー本体
        DrawBox(
            x,
            barY,
            x + fillWidth,
            barY + height,
            hpColor,
            TRUE
        );

        // HPバー枠
        DrawBox(
            x,
            barY,
            x + width,
            barY + height,
            gray,
            FALSE
        );

        // 残量部分の区切り感
        DrawLine(
            x,
            barY + height + 2,
            x + width,
            barY + height + 2,
            GetColor(45, 45, 45)
        );

        // 少しだけ光沢
        if (fillWidth > 4)
        {
            DrawLine(
                x + 2,
                barY + 2,
                x + fillWidth - 2,
                barY + 2,
                GetColor(230, 230, 230)
            );
        }
    }

    void DrawItemText(
        int x,
        int y,
        const char* itemName,
        int textFont
    )
    {
        const int labelColor = GetColor(150, 150, 150);
        const int valueColor = GetColor(230, 230, 230);
        const int shadow = GetColor(0, 0, 0);

        DrawStringToHandle(
            x + 2,
            y + 2,
            "ITEM",
            shadow,
            textFont
        );

        DrawStringToHandle(
            x,
            y,
            "ITEM",
            labelColor,
            textFont
        );

        DrawStringToHandle(
            x + 58 + 2,
            y + 2,
            itemName,
            shadow,
            textFont
        );

        DrawStringToHandle(
            x + 58,
            y,
            itemName,
            valueColor,
            textFont
        );
    }
}

const char* StatusPanelUI::GetItemName(ItemType item) const
{
    switch (item)
    {
    case ItemType::Item_heal:
        return "回復";

    case ItemType::Item_boost:
        return "ブースト";

    case ItemType::Item_reverse:
        return "反転";

    case ItemType::Item_none:
    default:
        return "なし";
    }
}

void StatusPanelUI::Draw(
    const std::shared_ptr<Player>& player,
    const std::shared_ptr<AIPlayer>& ai,
    int round,
    int plays,
    int lastPlayerCard,
    int lastAICard,
    const UIFontBank& fontBank
) const
{
    const int titleFont = fontBank.GetStatusTitleFont();
    const int textFont = fontBank.GetStatusTextFont();

    const int yellow = GetColor(255, 255, 0);
    const int shadow = GetColor(0, 0, 0);

    const int baseX = UIConfig::StatusPanelX + 18;
    const int baseY = UIConfig::StatusPanelY + 14;

    // ===== ROUND / TURN =====
    DrawFormatTextShadow(
        baseX,
        baseY,
        yellow,
        shadow,
        titleFont,
        "ROUND %d   TURN %d",
        round,
        plays
    );

    // ===== PLAYER =====
    const int playerY = baseY + 48;

    if (player)
    {
        DrawHpGauge(
            baseX,
            playerY,
            250,
            15,
            "PLAYER",
            player->GetHP(),
            MAX_Player_HP,
            GetColor(80, 180, 255),
            GetColor(70, 220, 120),
            titleFont,
            textFont
        );

        DrawItemText(
            baseX,
            playerY + 50,
            GetItemName(player->GetHeldItem()),
            textFont
        );
    }

    // ===== AI =====
    const int aiY = playerY + 92;

    if (ai)
    {
        DrawHpGauge(
            baseX,
            aiY,
            250,
            15,
            "AI",
            ai->GetHP(),
            MAX_Player_HP,
            GetColor(255, 120, 120),
            GetColor(255, 80, 80),
            titleFont,
            textFont
        );

        DrawItemText(
            baseX,
            aiY + 50,
            GetItemName(ai->GetHeldItem()),
            textFont
        );
    }

    // ===== 操作ヒント =====
    DrawStringToHandle(
        baseX,
        aiY + 86,
        "TAB : MAP",
        GetColor(120, 120, 120),
        textFont
    );
}