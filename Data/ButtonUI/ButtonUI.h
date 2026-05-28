#pragma once

#include <memory>

#include "Player.h"
#include "UIImageBank.h"

class ButtonUI
{
public:
    void DrawConfirmButton(const UIImageBank& imageBank) const;
    bool HitTestConfirmButton(int mouseX, int mouseY) const;

    void DrawUseItemButton(
        const std::shared_ptr<Player>& player,
        const UIImageBank& imageBank
    ) const;

    bool HitTestUseItemButton(int mouseX, int mouseY) const;
};