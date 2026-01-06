#include "SimpleUI.h"
#include "DxLib.h"
#include <string>

void SimpleUI::Draw(
    const CharacterBase& player,
    const CharacterBase& ai,
    int roundNumber,
    int lastPlayerCard,
    int lastAiCard
) const
{
    int white = GetColor(255, 255, 255);
    int yellow = GetColor(255, 255, 0);

    // --- ラウンド番号 ---
    DrawFormatString(20, 20, yellow, "Round: %d", roundNumber);

    // --- プレイヤー情報 ---
    DrawFormatString(20, 60, white, "Player HP: %d", player.GetHP());
    DrawFormatString(20, 80, white, "Player Pos: %d", player.GetPos());

    DrawFormatString(20, 100, white, "Player Item: %d", (int)player.GetHeldItem());

    // --- 手札表示 ---
    const auto& hand = player.GetHand();
    std::string handStr = "";
    for (int c : hand) handStr += std::to_string(c) + " ";

    DrawFormatString(20, 140, white, "Hand: %s", handStr.c_str());

    // --- 出した手 ---
    DrawFormatString(20, 180, yellow, "Last Player Card: %d", lastPlayerCard);
    DrawFormatString(20, 200, yellow, "Last AI Card: %d", lastAiCard);

    // --- AI 情報（必要なら） ---
    DrawFormatString(20, 240, white, "AI HP: %d", ai.GetHP());
    DrawFormatString(20, 260, white, "AI Pos: %d", ai.GetPos());
    DrawFormatString(20, 280, white, "AI Item: %d", (int)ai.GetHeldItem());
}
