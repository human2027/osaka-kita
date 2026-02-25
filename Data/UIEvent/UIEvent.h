#pragma once

// どんな種類のシステムメッセージか
enum class UIMessageType
{
    InvalidCard,        // そのカードはもう使えません
    PlayerBoost,        // プレイヤーブースト
    AIBoost,            // AIブースト

    PlayerWinMove,      // プレイヤーが勝利して N マス進む（value1 = N）
    AIWinMove,          // AI が勝利して N マス進む（value1 = N）
    Draw,               // 引き分け

    PlayerReachGoal,    // プレイヤーがゴール
    AIReachGoal,        // AI がゴール

    RoundExtended,      // ラウンド延長（value1 = 次のターン上限など）
    RoundEndQuick,      // 引き分けなしで早期決着（value1 = 使ったターン数）
    RoundEndMaxTurn,    // 最大ターンに達した（value1 = 最大ターン数）

    RoundHpPenalty,     // ラウンド終了時のHPペナルティ（value1 = 減少量）

    BothGoalDraw,       // 両者ゴールで引き分け
    PlayerGoalWin,      // プレイヤーのゴール勝利
    AIGoalWin,          // AIのゴール勝利

    BothDeadDraw,       // 両者HP0
    PlayerDeadLose,     // プレイヤーHP0で敗北
    AIDeadLose,         // AI HP0で敗北
    AI_PresentStart,    // value1 = (int)AIIntent, duration = 思考/溜め時間
    AI_ShowCard,        // value1 = aiCard(1..5), duration = 表示時間
};

// UI に渡す一件分のイベント
struct UIEvent
{
    UIMessageType type{};
    int value1 = 0; // 汎用パラメータ（移動マス数など）
    int value2 = 0;//演出、サブ情報用
    int duration = 0; // 表示フレーム（0 のときは UI 側でデフォルトを決める）
};
