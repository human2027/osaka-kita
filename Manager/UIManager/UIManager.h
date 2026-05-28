#pragma once

#include <memory>
#include <string>

#include "Player.h"
#include "AIPlayer.h"
#include "ItemType.h"
#include "UIEvent.h"

#include "UIImageBank.h"
#include "UIMessageManager.h"
#include "HandUI.h"
#include "ButtonUI.h"
#include "StatusPanelUI.h"
#include "UIFontBank.h"

class UIManager
{
public:
    UIManager() = default;

    bool LoadImages();
    void ClearImages();
    void ClearMessages();
    void Draw(
        const std::shared_ptr<Player>& player,
        const std::shared_ptr<AIPlayer>& ai,
        int round,
        int plays,
        int lastPlayerCard,
        int lastAICard
    );

    void DrawTitleScreen() const;

    void DrawPlayerHand(const std::shared_ptr<Player>& player, int selectedCard) const;
    void DrawAIHand(const std::shared_ptr<AIPlayer>& ai) const;
    int HitTestPlayerCard(int mouseX, int mouseY, const std::shared_ptr<Player>& player) const;

    void DrawConfirmButton() const;
    bool HitTestConfirmButton(int mouseX, int mouseY) const;

    void DrawUseItemButton(const std::shared_ptr<Player>& player) const;
    bool HitTestUseItemButton(int mx, int my) const;

    void PushSystemMessage(const std::string& text, int duration);
    void PushEvent(const UIEvent& e);
    void DrawRightImage() const;
    void ShowRoundStart(int round);
    void ShowPlayerGoalWin();
    void ShowAIGoalWin();
    void ShowPlayerHPZeroLose();
    void ShowAIHPZeroLose();

    void ShowItemPickup(bool isPlayer, ItemType item);
    void ShowItemUse(bool isPlayer, ItemType item);

private:
    UIImageBank imageBank;
    UIMessageManager messageManager;
    HandUI handUI;
    ButtonUI buttonUI;
    StatusPanelUI statusPanelUI;
    UIFontBank fontBank;
};