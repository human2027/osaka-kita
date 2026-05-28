#include "UIImageBank.h"
#include "UIAssets.h"
#include "DxLib.h"

UIImageBank::~UIImageBank()
{
    Clear();
}

bool UIImageBank::Load()
{
    Clear();

    cardHandles[1] = LoadGraph(UIAssets::Card1);
    cardHandles[2] = LoadGraph(UIAssets::Card2);
    cardHandles[3] = LoadGraph(UIAssets::Card3);
    cardHandles[4] = LoadGraph(UIAssets::Card4);
    cardHandles[5] = LoadGraph(UIAssets::Card5);

    confirmButtonHandle = LoadGraph(UIAssets::ConfirmButton);
    useItemButtonHandle = LoadGraph(UIAssets::UseItemButton);
    useItemButtonDisabledHandle = LoadGraph(UIAssets::UseItemButtonDisabled);

    statusPanelHandle = LoadGraph(UIAssets::StatusPanel);
    handPanelHandle = LoadGraph(UIAssets::HandPanel);

    // ƒ^ƒCƒgƒ‹‰æ–Ê
    titleHandle = LoadGraph(UIAssets::Title);
    rightHandle = LoadGraph(UIAssets::Right);
    return true;
}

void UIImageBank::Clear()
{
    for (int i = 1; i <= 5; ++i)
    {
        if (cardHandles[i] != -1)
        {
            DeleteGraph(cardHandles[i]);
            cardHandles[i] = -1;
        }
    }

    if (confirmButtonHandle != -1)
    {
        DeleteGraph(confirmButtonHandle);
        confirmButtonHandle = -1;
    }

    if (useItemButtonHandle != -1)
    {
        DeleteGraph(useItemButtonHandle);
        useItemButtonHandle = -1;
    }

    if (useItemButtonDisabledHandle != -1)
    {
        DeleteGraph(useItemButtonDisabledHandle);
        useItemButtonDisabledHandle = -1;
    }

    if (statusPanelHandle != -1)
    {
        DeleteGraph(statusPanelHandle);
        statusPanelHandle = -1;
    }

    if (handPanelHandle != -1)
    {
        DeleteGraph(handPanelHandle);
        handPanelHandle = -1;
    }

    if (titleHandle != -1)
    {
        DeleteGraph(titleHandle);
        titleHandle = -1;
    }
    if (rightHandle != -1)
    {
        DeleteGraph(rightHandle);
        rightHandle = -1;
    }
}

int UIImageBank::GetCardHandle(int card) const
{
    if (card < 1 || card > 5)
    {
        return -1;
    }

    return cardHandles[card];
}

int UIImageBank::GetUseItemButtonHandle(bool enabled) const
{
    return enabled ? useItemButtonHandle : useItemButtonDisabledHandle;
}