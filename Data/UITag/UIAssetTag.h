#pragma once

// UI画像の種類
enum class UIAssetType
{
    None,

    // パネル
    StatusPanel,
    HandPanel,

    // カード
    Card1,
    Card2,
    Card3,
    Card4,
    Card5,

    // ボタン
    ConfirmButton,
    UseItemButton,
};

// UIの状態
enum class UIState
{
    Normal,
    Hover,
    Pressed,
    Disabled,
    Selected,
};

struct UITag
{
    UIAssetType type = UIAssetType::None;
    UIState state = UIState::Normal;
};

inline bool operator==(const UITag& a, const UITag& b) noexcept
{
    return a.type == b.type && a.state == b.state;
}