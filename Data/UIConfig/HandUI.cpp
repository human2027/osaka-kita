#include "HandUI.h"

#include "DxLib.h"
#include "UIConfig.h"

void HandUI::DrawPlayerHand(
    const std::shared_ptr<Player>& player,
    int selectedCard,
    const UIImageBank& imageBank
) const
{
    if (!player) return;

    const auto& hand = player->GetHand();

    const int yellow = GetColor(255, 230, 80);
    const int white = GetColor(255, 255, 255);
    const int shadow = GetColor(15, 15, 15);

    const int panelX = UIConfig::HandStartX - 24;
    const int panelY = UIConfig::HandY - 46;

    const int panelW =
        static_cast<int>(hand.size()) * UIConfig::CardW +
        static_cast<int>(hand.size() - 1) * UIConfig::CardGap +
        48;

    const int panelH = UIConfig::CardH + 72;

    DrawBox(
        panelX,
        panelY,
        panelX + panelW,
        panelY + panelH,
        GetColor(20, 20, 20),
        TRUE
    );

    DrawBox(
        panelX,
        panelY,
        panelX + panelW,
        panelY + panelH,
        GetColor(120, 120, 120),
        FALSE
    );

    DrawString(panelX + 14, panelY + 12, "HAND", yellow);

    for (int i = 0; i < static_cast<int>(hand.size()); ++i)
    {
        const int card = hand[i];

        int x = UIConfig::HandStartX + i * (UIConfig::CardW + UIConfig::CardGap);
        int y = UIConfig::HandY;

        const bool isSelected = (card == selectedCard);

        if (isSelected)
        {
            y -= 12;
        }

        const int x2 = x + UIConfig::CardW;
        const int y2 = y + UIConfig::CardH;

        const int cardHandle = imageBank.GetCardHandle(card);

        if (cardHandle != -1)
        {
            DrawExtendGraph(
                x,
                y,
                x2,
                y2,
                cardHandle,
                TRUE
            );
        }
        else
        {
            DrawBox(x + 5, y + 5, x2 + 5, y2 + 5, shadow, TRUE);
            DrawBox(x, y, x2, y2, GetColor(235, 235, 235), TRUE);
            DrawBox(x, y, x2, y2, white, FALSE);

            DrawFormatString(
                x + UIConfig::CardW / 2 - 6,
                y + 42,
                GetColor(0, 0, 0),
                "%d",
                card
            );

            DrawFormatString(
                x + 12,
                y + 76,
                GetColor(45, 45, 45),
                "+%d",
                card
            );
        }

        if (isSelected)
        {
            DrawBox(x - 4, y - 4, x2 + 4, y2 + 4, yellow, FALSE);
            DrawBox(x - 5, y - 5, x2 + 5, y2 + 5, yellow, FALSE);
        }
    }
}

void HandUI::DrawAIHand(
    const std::shared_ptr<AIPlayer>& ai,
    const UIImageBank& imageBank
) const
{
    if (!ai) return;

    const auto& hand = ai->GetHand();

    const int white = GetColor(255, 255, 255);
    const int yellow = GetColor(255, 230, 80);
    const int darkGray = GetColor(45, 45, 45);
    const int shadow = GetColor(15, 15, 15);

    const int panelX = UIConfig::AIHandStartX - 18;
    const int panelY = UIConfig::AIHandY - 38;

    const int panelW =
        static_cast<int>(hand.size()) * UIConfig::AICardW +
        static_cast<int>(hand.size() - 1) * UIConfig::AICardGap +
        36;

    const int panelH = UIConfig::AICardH + 58;

    DrawBox(
        panelX,
        panelY,
        panelX + panelW,
        panelY + panelH,
        GetColor(20, 20, 20),
        TRUE
    );

    DrawBox(
        panelX,
        panelY,
        panelX + panelW,
        panelY + panelH,
        GetColor(120, 120, 120),
        FALSE
    );

    DrawString(panelX + 12, panelY + 10, "AI HAND", yellow);

    for (int i = 0; i < static_cast<int>(hand.size()); ++i)
    {
        const int card = hand[i];

        const int x = UIConfig::AIHandStartX + i * (UIConfig::AICardW + UIConfig::AICardGap);
        const int y = UIConfig::AIHandY;
        const int x2 = x + UIConfig::AICardW;
        const int y2 = y + UIConfig::AICardH;

        const int cardHandle = imageBank.GetCardHandle(card);

        if (cardHandle != -1)
        {
            DrawExtendGraph(
                x,
                y,
                x2,
                y2,
                cardHandle,
                TRUE
            );
        }
        else
        {
            DrawBox(x + 4, y + 4, x2 + 4, y2 + 4, shadow, TRUE);
            DrawBox(x, y, x2, y2, GetColor(235, 235, 235), TRUE);
            DrawBox(x, y, x2, y2, white, FALSE);

            DrawFormatString(
                x + UIConfig::AICardW / 2 - 6,
                y + 30,
                GetColor(0, 0, 0),
                "%d",
                card
            );

            DrawFormatString(
                x + 8,
                y + 56,
                darkGray,
                "+%d",
                card
            );
        }
    }
}

int HandUI::HitTestPlayerCard(
    int mouseX,
    int mouseY,
    const std::shared_ptr<Player>& player
) const
{
    if (!player) return 0;

    const auto& hand = player->GetHand();

    for (int i = 0; i < static_cast<int>(hand.size()); ++i)
    {
        const int card = hand[i];

        const int x = UIConfig::HandStartX + i * (UIConfig::CardW + UIConfig::CardGap);
        const int y = UIConfig::HandY;

        if (mouseX >= x &&
            mouseX <= x + UIConfig::CardW &&
            mouseY >= y &&
            mouseY <= y + UIConfig::CardH)
        {
            return card;
        }
    }

    return 0;
}