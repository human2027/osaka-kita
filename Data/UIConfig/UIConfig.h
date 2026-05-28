#pragma once

namespace UIConfig
{
    // =========================
    // プレイヤー手札UI
    // 左下に寄せる
    // =========================
    constexpr int HandStartX = 55;
    constexpr int HandY = 705;
    constexpr int CardW = 68;
    constexpr int CardH = 92;
    constexpr int CardGap = 10;

    // =========================
    // AI手札UI
    // 左中段
    // =========================
    constexpr int AIHandStartX = 150;
    constexpr int AIHandY = 350;
    constexpr int AICardW = 52;
    constexpr int AICardH = 70;
    constexpr int AICardGap = 10;

    // =========================
    // 決定ボタンUI
    // プレイヤー手札の下・右側
    // =========================
    constexpr int ConfirmX = 270;
    constexpr int ConfirmY = 840;
    constexpr int ConfirmW = 150;
    constexpr int ConfirmH = 60;

    // =========================
    // アイテムボタンUI
    // プレイヤー手札の下・左側
    // =========================
    constexpr int UseItemButtonX = 55;
    constexpr int UseItemButtonY = 840;
    constexpr int UseItemButtonW = 180;
    constexpr int UseItemButtonH = 60;

    // =========================
    // ステータスパネルUI
    // =========================
    constexpr int StatusPanelX = 12;
    constexpr int StatusPanelY = 12;
    constexpr int StatusPanelW = 320;
    constexpr int StatusPanelH = 170;

    // =========================
    // 共通
    // =========================
    constexpr int LineHeight = 20;
}