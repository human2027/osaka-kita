#pragma once

#include <memory>

#include "Player.h"
#include "AIPlayer.h"
#include "ItemType.h"
#include "UIFontBank.h"

class StatusPanelUI
{
public:
    void Draw(
        const std::shared_ptr<Player>& player,
        const std::shared_ptr<AIPlayer>& ai,
        int round,
        int plays,
        int lastPlayerCard,
        int lastAICard,
        const UIFontBank& fontBank
    ) const;

private:
    const char* GetItemName(ItemType item) const;
};