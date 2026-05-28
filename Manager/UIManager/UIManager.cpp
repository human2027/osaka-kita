#include "UIManager.h"

#include "DxLib.h"
#include "UIConfig.h"
#include "InitialValue.h"

bool UIManager::LoadImages()
{
    const bool imageOk = imageBank.Load();
    const bool fontOk = fontBank.Load();

    return imageOk && fontOk;
}
void UIManager::ClearImages()
{
    imageBank.Clear();
    fontBank.Clear();
}

void UIManager::Draw(
    const std::shared_ptr<Player>& player,
    const std::shared_ptr<AIPlayer>& ai,
    int round,
    int plays,
    int lastPlayerCard,
    int lastAICard
)
{
    statusPanelUI.Draw(
        player,
        ai,
        round,
        plays,
        lastPlayerCard,
        lastAICard,
        fontBank
    );

    DrawRightImage();

    messageManager.Draw(
        UIConfig::StatusPanelX + 8,
        UIConfig::StatusPanelY + UIConfig::StatusPanelH + 12,
        UIConfig::LineHeight
    );
}
void UIManager::DrawRightImage() const
{
    const int right = imageBank.GetRightHandle();

    if (right == -1)
    {
        DrawString(1600, 180, "Right.png load failed", GetColor(255, 0, 0));
        return;
    }

    int imageW = 0;
    int imageH = 0;
    GetGraphSize(right, &imageW, &imageH);

    const int marginRight = 80;
    const int y = 180;

    const int x = static_cast<int>(Window_screen_W) - imageW - marginRight;

    DrawGraph(x, y, right, TRUE);
}
void UIManager::DrawTitleScreen() const
{
    const int title = imageBank.GetTitleHandle();

    if (title != -1)
    {
        DrawGraph(0, 0, title, TRUE);
    }
    else
    {
        DrawBox(
            0,
            0,
            static_cast<int>(Window_screen_W),
            static_cast<int>(Window_screen_H),
            GetColor(0, 0, 0),
            TRUE
        );

        DrawString(100, 100, "CARD BOARD GAME", GetColor(255, 255, 255));
        DrawString(100, 160, "Press ENTER or Click to Start", GetColor(255, 255, 255));
    }
}

void UIManager::DrawPlayerHand(
    const std::shared_ptr<Player>& player,
    int selectedCard
) const
{
    handUI.DrawPlayerHand(player, selectedCard, imageBank);
}

void UIManager::DrawAIHand(const std::shared_ptr<AIPlayer>& ai) const
{
    handUI.DrawAIHand(ai, imageBank);
}

int UIManager::HitTestPlayerCard(
    int mouseX,
    int mouseY,
    const std::shared_ptr<Player>& player
) const
{
    return handUI.HitTestPlayerCard(mouseX, mouseY, player);
}

void UIManager::DrawConfirmButton() const
{
    buttonUI.DrawConfirmButton(imageBank);
}

bool UIManager::HitTestConfirmButton(int mouseX, int mouseY) const
{
    return buttonUI.HitTestConfirmButton(mouseX, mouseY);
}

void UIManager::DrawUseItemButton(const std::shared_ptr<Player>& player) const
{
    buttonUI.DrawUseItemButton(player, imageBank);
}

bool UIManager::HitTestUseItemButton(int mx, int my) const
{
    return buttonUI.HitTestUseItemButton(mx, my);
}

void UIManager::PushSystemMessage(const std::string& text, int duration)
{
    messageManager.PushSystemMessage(text, duration);
}

void UIManager::PushEvent(const UIEvent& e)
{
    messageManager.PushEvent(e);
}

void UIManager::ShowRoundStart(int round)
{
    messageManager.ShowRoundStart(round);
}

void UIManager::ShowPlayerGoalWin()
{
    messageManager.ShowPlayerGoalWin();
}

void UIManager::ShowAIGoalWin()
{
    messageManager.ShowAIGoalWin();
}

void UIManager::ShowPlayerHPZeroLose()
{
    messageManager.ShowPlayerHPZeroLose();
}

void UIManager::ShowAIHPZeroLose()
{
    messageManager.ShowAIHPZeroLose();
}

void UIManager::ShowItemPickup(bool isPlayer, ItemType item)
{
    messageManager.ShowItemPickup(isPlayer, item);
}

void UIManager::ShowItemUse(bool isPlayer, ItemType item)
{
    messageManager.ShowItemUse(isPlayer, item);
}
void UIManager::ClearMessages()
{
    messageManager.ClearMessages();
}