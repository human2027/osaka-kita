#include "UIMessageManager.h"

#include "DxLib.h"
#include "InitialValue.h"

#include <algorithm>

std::string UIMessageManager::GetItemName(ItemType item) const
{
    switch (item)
    {
    case ItemType::Item_heal:
        return "回復";

    case ItemType::Item_boost:
        return "ブースト";

    case ItemType::Item_reverse:
        return "反転";

    case ItemType::Item_none:
    default:
        return "なし";
    }
}

void UIMessageManager::AddMessage(const std::string& text, int duration)
{
    UIMessage msg;
    msg.text = text;
    msg.timer = duration;
    messages.push_back(msg);
}

void UIMessageManager::AddMainMessage(const std::string& text, int duration)
{
    mainMessage.text = text;
    mainMessage.timer = duration;
    hasMainMessage = true;
}

void UIMessageManager::DrawMainMessage() const
{
    if (!hasMainMessage) return;
    if (mainMessage.text.empty()) return;
    if (mainMessage.timer <= 0) return;

    // 大きいメインメッセージ用フォント
    // static にして、毎フレーム CreateFont しないようにする
    static int mainFont = -1;

    if (mainFont == -1)
    {
        mainFont = CreateFontToHandle(
            "ＭＳ ゴシック",
            MainMessage_FontSize,
            MainMessage_FontThickness,
            DX_FONTTYPE_ANTIALIASING_EDGE
        );
    }

    const int screenW = static_cast<int>(Window_screen_W);

    const int textColor = GetColor(255, 230, 80);
    const int shadowColor = GetColor(0, 0, 0);
    const int lineColor = GetColor(255, 255, 255);

    const int textW = GetDrawStringWidthToHandle(
        mainMessage.text.c_str(),
        static_cast<int>(mainMessage.text.size()),
        mainFont
    );

    const int x = screenW / 2 - textW / 2;
    const int y = MainMessage_Y;

    // 残り時間が少ない時だけフェード
    int alpha = 255;
    constexpr int fadeFrame = 30;

    if (mainMessage.timer < fadeFrame)
    {
        alpha = 255 * mainMessage.timer / fadeFrame;
        if (alpha < 0)
        {
            alpha = 0;
        }
    }

    SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

    // 背景帯
    DrawBox(
        0,
        y - 18,
        screenW,
        y + MainMessage_FontSize + 24,
        GetColor(0, 0, 0),
        TRUE
    );

    // 影
    DrawStringToHandle(
        x + Message_Shadow_Offset + 1,
        y + Message_Shadow_Offset + 1,
        mainMessage.text.c_str(),
        shadowColor,
        mainFont
    );

    DrawStringToHandle(
        x + Message_Shadow_Offset,
        y + Message_Shadow_Offset,
        mainMessage.text.c_str(),
        shadowColor,
        mainFont
    );

    // 本体
    DrawStringToHandle(
        x,
        y,
        mainMessage.text.c_str(),
        textColor,
        mainFont
    );

    // 下線
    DrawLine(
        x - 50,
        y + MainMessage_FontSize + 12,
        x + textW + 50,
        y + MainMessage_FontSize + 12,
        lineColor
    );

    SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void UIMessageManager::Draw(int x, int y, int lineHeight)
{
    // 表示位置
    (void)x;
    (void)y;
    (void)lineHeight;

    // 先に上中央のメインメッセージを描画
    DrawMainMessage();

    // 通常メッセージ用フォント
    static int subFont = -1;
    if (!messages.empty())
    {
        SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);

        DrawBox(
            SubMessage_Box_X,
            SubMessage_Box_Y,
            SubMessage_Box_X + SubMessage_Box_W,
            SubMessage_Box_Y + SubMessage_Box_H,
            GetColor(0, 0, 0),
            TRUE
        );

        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

        DrawBox(
            SubMessage_Box_X,
            SubMessage_Box_Y,
            SubMessage_Box_X + SubMessage_Box_W,
            SubMessage_Box_Y + SubMessage_Box_H,
            GetColor(255, 255, 255),
            FALSE
        );
    }
    if (subFont == -1)
    {
        subFont = CreateFontToHandle(
            "ＭＳ ゴシック",
            SubMessage_FontSize,
            SubMessage_FontThickness,
            DX_FONTTYPE_ANTIALIASING_EDGE
        );
    }

    const int yellow = GetColor(255, 255, 0);
    const int shadowColor = GetColor(0, 0, 0);

    int drawY = SubMessage_Y;

    for (auto& msg : messages)
    {
        // 影
        DrawStringToHandle(
            SubMessage_X + Message_Shadow_Offset,
            drawY + Message_Shadow_Offset,
            msg.text.c_str(),
            shadowColor,
            subFont
        );

        // 本体
        DrawStringToHandle(
            SubMessage_X,
            drawY,
            msg.text.c_str(),
            yellow,
            subFont
        );

        drawY += SubMessage_LineHeight;

        if (msg.timer > 0)
        {
            --msg.timer;
        }
    }

    messages.erase(
        std::remove_if(
            messages.begin(),
            messages.end(),
            [](const UIMessage& msg)
            {
                return msg.timer <= 0;
            }
        ),
        messages.end()
    );

    // メインメッセージの時間更新
    if (hasMainMessage)
    {
        if (mainMessage.timer > 0)
        {
            --mainMessage.timer;
        }

        if (mainMessage.timer <= 0)
        {
            hasMainMessage = false;
            mainMessage.text.clear();
        }
    }
}

void UIMessageManager::ClearMessages()
{
    messages.clear();

    mainMessage.text.clear();
    mainMessage.timer = 0;
    hasMainMessage = false;
}

void UIMessageManager::PushSystemMessage(const std::string& text, int duration)
{
    AddMessage(text, duration);
}

void UIMessageManager::PushEvent(const UIEvent& e)
{
    int dur = (e.duration > 0) ? e.duration : 0;

    switch (e.type)
    {
    case UIMessageType::InvalidCard:
        if (dur == 0) dur = Message_Dur_Short;
        AddMessage("そのカードはもう使えません。", dur);
        break;

    case UIMessageType::PlayerBoost:
        if (dur == 0) dur = Message_Dur_Middle;
        AddMessage("プレイヤーのブースト発動！ +1マス移動！", dur);
        break;

    case UIMessageType::AIBoost:
        if (dur == 0) dur = Message_Dur_Middle;
        AddMessage("AIのブースト発動！ +1マス移動！", dur);
        break;

    case UIMessageType::PlayerWinMove:
        if (dur == 0) dur = Message_Dur_Short;
        AddMessage(
            "プレイヤーの勝利！ " + std::to_string(e.value1) + "マス進んだ！",
            dur
        );
        break;

    case UIMessageType::AIWinMove:
        if (dur == 0) dur = Message_Dur_Short;
        AddMessage(
            "プレイヤーの敗北… AIが " + std::to_string(e.value1) + "マス進んだ！",
            dur
        );
        break;

    case UIMessageType::Draw:
        if (dur == 0) dur = Message_Dur_Short;
        AddMessage("引き分け。", dur);
        break;

    case UIMessageType::PlayerReachGoal:
        if (dur == 0) dur = Message_Dur_Middle;
        AddMessage("プレイヤーがゴールに到達！", dur);
        break;

    case UIMessageType::AIReachGoal:
        if (dur == 0) dur = Message_Dur_Middle;
        AddMessage("AIがゴールに到達！", dur);
        break;

    case UIMessageType::RoundExtended:
        if (dur == 0) dur = Message_Dur_Middle;
        AddMessage(
            "決着つかず… 引き分け扱いで延長！ 次は" +
            std::to_string(e.value1) + "回目まで続行！",
            dur
        );
        break;

    case UIMessageType::RoundEndQuick:
        if (dur == 0) dur = Message_Dur_Middle;
        AddMessage(
            std::to_string(e.value1) +
            "回の勝負で決着がついた！ ラウンド終了。",
            dur
        );
        break;

    case UIMessageType::RoundEndMaxTurn:
        if (dur == 0) dur = Message_Dur_Middle;
        AddMessage(
            "このラウンドのターン上限（最大" +
            std::to_string(e.value1) + "回）に達しました。",
            dur
        );
        break;

    case UIMessageType::RoundHpPenalty:
        if (dur == 0) dur = Message_Dur_Middle;
        AddMessage(
            "ラウンド終了！ 時間経過で双方に " +
            std::to_string(e.value1) + " ダメージ！",
            dur
        );
        break;

    case UIMessageType::BothGoalDraw:
        if (dur == 0) dur = Message_Dur_Long;
        AddMainMessage("両者ゴール！ 引き分け！", MainMessage_Dur_Long);
        AddMessage("両者ゴール！ 引き分け！", dur);
        break;

    case UIMessageType::PlayerGoalWin:
        if (dur == 0) dur = Message_Dur_Long;
        AddMainMessage("プレイヤーのゴール勝利！", MainMessage_Dur_Long);
        AddMessage("プレイヤーのゴール勝利！", dur);
        break;

    case UIMessageType::AIGoalWin:
        if (dur == 0) dur = Message_Dur_Long;
        AddMainMessage("AIのゴール勝利！", MainMessage_Dur_Long);
        AddMessage("AIのゴール勝利！", dur);
        break;

    case UIMessageType::BothDeadDraw:
        if (dur == 0) dur = Message_Dur_Long;
        AddMainMessage("両者倒れた！ 引き分け！", MainMessage_Dur_Long);
        AddMessage("両者倒れた！ 引き分け！", dur);
        break;

    case UIMessageType::PlayerDeadLose:
        if (dur == 0) dur = Message_Dur_Long;
        AddMainMessage("プレイヤーは倒れた… AIの勝利！", MainMessage_Dur_Long);
        AddMessage("プレイヤーは倒れた… AIの勝利！", dur);
        break;

    case UIMessageType::AIDeadLose:
        if (dur == 0) dur = Message_Dur_Long;
        AddMainMessage("AIは倒れた！ プレイヤーの勝利！", MainMessage_Dur_Long);
        AddMessage("AIは倒れた！ プレイヤーの勝利！", dur);
        break;
    }
}

void UIMessageManager::ShowRoundStart(int round)
{
    AddMainMessage(
        "ラウンド " + std::to_string(round) + " 開始！",
        MainMessage_Dur_Middle
    );
}

void UIMessageManager::ShowPlayerGoalWin()
{
    AddMainMessage("あなたがゴール！勝利！", MainMessage_Dur_Long);
    AddMessage("あなたがゴール！勝利！", Message_Dur_Middle);
}

void UIMessageManager::ShowAIGoalWin()
{
    AddMainMessage("AIがゴール！あなたの負け…", MainMessage_Dur_Long);
    AddMessage("AIがゴール！あなたの負け…", Message_Dur_Middle);
}

void UIMessageManager::ShowPlayerHPZeroLose()
{
    AddMainMessage("HPが0！AIの勝利！", MainMessage_Dur_Long);
    AddMessage("HPが0！AIの勝利！", Message_Dur_Middle);
}

void UIMessageManager::ShowAIHPZeroLose()
{
    AddMainMessage("AIのHPが0！あなたの勝利！", MainMessage_Dur_Long);
    AddMessage("AIのHPが0！あなたの勝利！", Message_Dur_Middle);
}

void UIMessageManager::ShowItemPickup(bool isPlayer, ItemType item)
{
    const std::string who = isPlayer ? "プレイヤー" : "AI";
    AddMessage(who + "は「" + GetItemName(item) + "」を拾った！", Message_Dur_Middle);
}

void UIMessageManager::ShowItemUse(bool isPlayer, ItemType item)
{
    const std::string who = isPlayer ? "プレイヤー" : "AI";
    AddMessage(who + "の「" + GetItemName(item) + "」発動！", Message_Dur_Middle);
}