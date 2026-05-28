#pragma once

#include <memory>

#include "Player.h"
#include "AIPlayer.h"
#include "UIImageBank.h"

class HandUI
{
public:
    void DrawPlayerHand(
        const std::shared_ptr<Player>& player,
        int selectedCard,
        const UIImageBank& imageBank
    ) const;

    void DrawAIHand(
        const std::shared_ptr<AIPlayer>& ai,
        const UIImageBank& imageBank
    ) const;

    int HitTestPlayerCard(
        int mouseX,
        int mouseY,
        const std::shared_ptr<Player>& player
    ) const;
};