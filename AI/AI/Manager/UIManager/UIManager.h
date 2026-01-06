#pragma once

#include <memory>
#include <deque>
#include <string>

#include "Player.h"
#include "AIPlayer.h"
#include "UIEvent.h"

// UI に表示する 1 件分のメッセージ
struct UIMessage
{
    std::string text;  // 表示テキスト
    int         timer; // 残り表示フレーム数
};

class UIManager
{
public:
    // メインの描画処理
    void Draw(const std::shared_ptr<Player>& player,
        const std::shared_ptr<AIPlayer>& ai,
        int round, int plays,
        int lastPlayerCard, int lastAICard);

    // TurnManager / GameManager からの生テキスト（デバッグ用など）
    void PushSystemMessage(const std::string& text, int duration);

    //イベントベース
    void PushEvent(const UIEvent& e);

    // GameManager 向けの「意味付き」メッセージ API 群
    void ShowRoundStart(int round);
    void ShowPlayerGoalWin();
    void ShowAIGoalWin();
    void ShowPlayerHPZeroLose();
    void ShowAIHPZeroLose();

private:
    // 実際にメッセージキューへ積む共通処理
    void AddMessage(const std::string& text, int duration);

private:
    std::deque<UIMessage> m_messages;

    // 画面レイアウト用の定数
    static constexpr int kBaseX = 20;   // 左マージン
    static constexpr int kBaseY = 20;   // 一番上の行
    static constexpr int kLineHeight = 20;   // 行間
    static constexpr int kForwardEvalOffsetY = 180;  // AI 評価表示の Y 開始位置
    static constexpr int kMessageOffsetY = 140;  // システムメッセージの Y 開始位置

    // メッセージ表示時間（フレーム）
    static constexpr int kDurShort = 120;  // 短め（2 秒相当）
    static constexpr int kDurMiddle = 180;  // やや長め（3 秒相当）
    static constexpr int kDurLong = 240;  // 長め
};
