#include "ButtonUI.h"

#include "DxLib.h"
#include "UIConfig.h"

void ButtonUI::DrawConfirmButton(const UIImageBank& imageBank) const
{
    const int handle = imageBank.GetConfirmButtonHandle();

    if (handle != -1)
    {
        DrawExtendGraph(
            UIConfig::ConfirmX,
            UIConfig::ConfirmY,
            UIConfig::ConfirmX + UIConfig::ConfirmW,
            UIConfig::ConfirmY + UIConfig::ConfirmH,
            handle,
            TRUE
        );
        return;
    }

    const int white = GetColor(255, 255, 255);
    const int cyan = GetColor(0, 255, 255);

    DrawBox(
        UIConfig::ConfirmX,
        UIConfig::ConfirmY,
        UIConfig::ConfirmX + UIConfig::ConfirmW,
        UIConfig::ConfirmY + UIConfig::ConfirmH,
        cyan,
        TRUE
    );

    DrawBox(
        UIConfig::ConfirmX,
        UIConfig::ConfirmY,
        UIConfig::ConfirmX + UIConfig::ConfirmW,
        UIConfig::ConfirmY + UIConfig::ConfirmH,
        white,
        FALSE
    );

    DrawString(
        UIConfig::ConfirmX + 20,
        UIConfig::ConfirmY + 20,
        "ƒ^[ƒ“I—¹",
        white
    );
}

bool ButtonUI::HitTestConfirmButton(int mouseX, int mouseY) const
{
    return
        mouseX >= UIConfig::ConfirmX &&
        mouseX <= UIConfig::ConfirmX + UIConfig::ConfirmW &&
        mouseY >= UIConfig::ConfirmY &&
        mouseY <= UIConfig::ConfirmY + UIConfig::ConfirmH;
}

void ButtonUI::DrawUseItemButton(
    const std::shared_ptr<Player>& player,
    const UIImageBank& imageBank
) const
{
    const bool hasItem = player && player->HasItem();
    const int handle = imageBank.GetUseItemButtonHandle(hasItem);

    const int x1 = UIConfig::UseItemButtonX;
    const int y1 = UIConfig::UseItemButtonY;
    const int x2 = UIConfig::UseItemButtonX + UIConfig::UseItemButtonW;
    const int y2 = UIConfig::UseItemButtonY + UIConfig::UseItemButtonH;

    if (handle != -1)
    {
        DrawExtendGraph(x1, y1, x2, y2, handle, TRUE);
        return;
    }

    const unsigned int fillColor = hasItem
        ? GetColor(60, 60, 60)
        : GetColor(30, 30, 30);

    const unsigned int textColor = hasItem
        ? GetColor(255, 255, 255)
        : GetColor(120, 120, 120);

    DrawBox(x1, y1, x2, y2, fillColor, TRUE);
    DrawBox(x1, y1, x2, y2, GetColor(255, 255, 255), FALSE);

    DrawString(
        UIConfig::UseItemButtonX + 30,
        UIConfig::UseItemButtonY + 20,
        "USE ITEM",
        textColor
    );
}

bool ButtonUI::HitTestUseItemButton(int mouseX, int mouseY) const
{
    return
        mouseX >= UIConfig::UseItemButtonX &&
        mouseX <= UIConfig::UseItemButtonX + UIConfig::UseItemButtonW &&
        mouseY >= UIConfig::UseItemButtonY &&
        mouseY <= UIConfig::UseItemButtonY + UIConfig::UseItemButtonH;
}