#include "UIManager.h"

#include "DxLib.h"
#include <algorithm>

//----------------------------------------
// 描画
//----------------------------------------
void UIManager::Draw(const std::shared_ptr<Player>& player,
    const std::shared_ptr<AIPlayer>& ai,
    int round, int plays,
    int lastPlayerCard, int lastAICard)
{
    const int white = GetColor(255, 255, 255);
    const int yellow = GetColor(255, 255, 0);
    const int cyan = GetColor(0, 255, 255);

    int x = kBaseX;
    int y = kBaseY;

    //基本情報表示
    DrawFormatString(x, y, yellow, "Round %d  Turn %d", round, plays);
    y += kLineHeight;

    if (player)
    {
        DrawFormatString(x, y, white, "Player HP: %d", player->GetHP());
        y += kLineHeight;
    }

    if (ai)
    {
        DrawFormatString(x, y, white, "AI HP: %d", ai->GetHP());
        y += kLineHeight;
    }

    DrawFormatString(
        x, y, white,
        "Player Last: %d  AI Last: %d",
        lastPlayerCard, lastAICard
    );
    y += kLineHeight;

    //AI の前方評価の可視化
    if (ai)
    {
        auto evals = ai->GetForwardEvaluation();

        int evalY = kForwardEvalOffsetY;
        DrawString(x, evalY, "AI Forward Eval (steps / type / score):", cyan);
        evalY += kLineHeight;

        for (const auto& e : evals)
        {
            DrawFormatString(
                x, evalY, white,
                " %d: type=%d score=%d",
                e.steps,
                static_cast<int>(e.type),
                e.score
            );
            evalY += kLineHeight;
        }
    }

    //システムメッセージ一覧を描画 
    int yMsg = kMessageOffsetY;
    for (auto& msg : m_messages)
    {
        DrawString(x, yMsg, msg.text.c_str(), yellow);
        yMsg += kLineHeight;

        if (msg.timer > 0)
        {
            --msg.timer;
        }
    }

    // 時間が切れたメッセージを削除
    m_messages.erase(
        std::remove_if(
            m_messages.begin(),
            m_messages.end(),
            [](const UIMessage& msg) { return msg.timer <= 0; }),
        m_messages.end()
    );
}

// 内部共通：メッセージ追加
void UIManager::AddMessage(const std::string& text, int duration)
{
    UIMessage msg;
    msg.text = text;
    msg.timer = duration;
    m_messages.push_back(msg);
}

// 汎用メッセージ追加（生テキスト）
void UIManager::PushSystemMessage(const std::string& text, int duration)
{
    AddMessage(text, duration);
}

// TurnManager などからのイベントを処理
void UIManager::PushEvent(const UIEvent& e)
{
    int dur = (e.duration > 0) ? e.duration : 0;

    switch (e.type)
    {
    case UIMessageType::InvalidCard:
        if (dur == 0) dur = kDurShort;
        AddMessage("そのカードはもう使えません。", dur);
        break;

    case UIMessageType::PlayerBoost:
        if (dur == 0) dur = kDurMiddle;
        AddMessage("プレイヤーのブースト発動！ +2マス移動！", dur);
        break;

    case UIMessageType::AIBoost:
        if (dur == 0) dur = kDurMiddle;
        AddMessage("AIのブースト発動！ +2マス移動！", dur);
        break;

    case UIMessageType::PlayerWinMove:
        if (dur == 0) dur = kDurShort;
        AddMessage(
            "プレイヤーの勝利！ " + std::to_string(e.value1) + "マス進んだ！",
            dur
        );
        break;

    case UIMessageType::AIWinMove:
        if (dur == 0) dur = kDurShort;
        AddMessage(
            "プレイヤーの敗北… AIが " + std::to_string(e.value1) + "マス進んだ！",
            dur
        );
        break;

    case UIMessageType::Draw:
        if (dur == 0) dur = kDurShort;
        AddMessage("引き分け。", dur);
        break;

    case UIMessageType::PlayerReachGoal:
        if (dur == 0) dur = kDurMiddle;
        AddMessage("プレイヤーがゴールに到達！", dur);
        break;

    case UIMessageType::AIReachGoal:
        if (dur == 0) dur = kDurMiddle;
        AddMessage("AIがゴールに到達！", dur);
        break;

    case UIMessageType::RoundExtended:
        if (dur == 0) dur = kDurMiddle;
        AddMessage(
            "決着つかず… 引き分け扱いで延長！ 次は" +
            std::to_string(e.value1) + "回目まで続行！",
            dur
        );
        break;

    case UIMessageType::RoundEndQuick:
        if (dur == 0) dur = kDurMiddle;
        AddMessage(
            std::to_string(e.value1) +
            "回の勝負で決着がついた！ ラウンド終了。",
            dur
        );
        break;

    case UIMessageType::RoundEndMaxTurn:
        if (dur == 0) dur = kDurMiddle;
        AddMessage(
            "このラウンドのターン上限（最大" +
            std::to_string(e.value1) + "回）に達しました。",
            dur
        );
        break;

    case UIMessageType::RoundHpPenalty:
        if (dur == 0) dur = kDurMiddle;
        AddMessage(
            "ラウンド終了！ 時間経過で双方に " +
            std::to_string(e.value1) + " ダメージ！",
            dur
        );
        break;

    case UIMessageType::BothGoalDraw:
        if (dur == 0) dur = kDurLong;
        AddMessage("両者ゴール！ 引き分け！", dur);
        break;

    case UIMessageType::PlayerGoalWin:
        if (dur == 0) dur = kDurLong;
        AddMessage("プレイヤーのゴール勝利！", dur);
        break;

    case UIMessageType::AIGoalWin:
        if (dur == 0) dur = kDurLong;
        AddMessage("AIのゴール勝利！", dur);
        break;

    case UIMessageType::BothDeadDraw:
        if (dur == 0) dur = kDurLong;
        AddMessage("両者倒れた！ 引き分け！", dur);
        break;

    case UIMessageType::PlayerDeadLose:
        if (dur == 0) dur = kDurLong;
        AddMessage("プレイヤーは倒れた… AIの勝利！", dur);
        break;

    case UIMessageType::AIDeadLose:
        if (dur == 0) dur = kDurLong;
        AddMessage("AIは倒れた！ プレイヤーの勝利！", dur);
        break;
    }
}

// ===== GameManager 向けの「意味付き」メッセージ API 群 =====

void UIManager::ShowRoundStart(int round)
{
    std::string text = "ラウンド " + std::to_string(round) + " 開始！";
    AddMessage(text, kDurShort);
}

void UIManager::ShowPlayerGoalWin()
{
    AddMessage("あなたがゴール！勝利！", kDurMiddle);
}

void UIManager::ShowAIGoalWin()
{
    AddMessage("AIがゴール！あなたの負け…", kDurMiddle);
}

void UIManager::ShowPlayerHPZeroLose()
{
    AddMessage("HPが0！AIの勝利！", kDurMiddle);
}

void UIManager::ShowAIHPZeroLose()
{
    AddMessage("AIのHPが0！あなたの勝利！", kDurMiddle);
}
