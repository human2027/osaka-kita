#include "UIFontBank.h"
#include "DxLib.h"

UIFontBank::~UIFontBank()
{
    Clear();
}

bool UIFontBank::Load()
{
    Clear();

    statusTitleFont = CreateFontToHandle(
        "Yu Gothic UI Semibold",
        18,
        3,
        DX_FONTTYPE_ANTIALIASING_EDGE
    );

    statusTextFont = CreateFontToHandle(
        "Yu Gothic UI",
        15,
        2,
        DX_FONTTYPE_ANTIALIASING_EDGE
    );

    return statusTitleFont != -1 && statusTextFont != -1;
}

void UIFontBank::Clear()
{
    if (statusTitleFont != -1)
    {
        DeleteFontToHandle(statusTitleFont);
        statusTitleFont = -1;
    }

    if (statusTextFont != -1)
    {
        DeleteFontToHandle(statusTextFont);
        statusTextFont = -1;
    }
}