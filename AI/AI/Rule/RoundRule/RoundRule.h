#pragma once
#include "CharacterBase.h"
#include "MapManager.h"

// ラウンドのルール判定をまとめたクラス
class RoundRule
{
public:
    // ブースト処理（移動量を増加させるだけ）
    static void ApplyBoosts(
        CharacterBase& player,
        CharacterBase& ai,
        int& playerMove,
        int& aiMove
    );

    // 勝敗結果からラウンド統計を更新
    static void UpdateStatsOnResult(
        int playerCard,
        int aiCard,
        int result,
        int& playerWinsThisRound,
        int& aiWinsThisRound,
        int& drawsThisRound,
        bool& sameNumberOccurred,
        int& segmentDraws,
        bool& segmentSameNumber
    );

    // 勝敗結果に応じたマス移動のみ（メッセージは外で）
    static void ApplyMove(
        CharacterBase& player,
        CharacterBase& ai,
        int result,
        int playerMove,
        int aiMove
    );

    // ゴール判定のみ（フラグ操作のみ）
    static void CheckGoal(
        MapManager& map,
        CharacterBase& player,
        CharacterBase& ai,
        bool& playerReachedGoal,
        bool& aiReachedGoal,
        bool& roundFinished,
        bool& roundEndedThisTurn
    );

    // ターン上限と延長ロジック（フラグ・上限値だけ触る）
    static void HandleTurnLimitAndExtension(
        int playsThisRound,
        int& currentTurnLimit,
        int maxTurnLimit,
        int segmentDraws,
        bool segmentSameNumber,
        bool& roundFinished,
        bool& roundEndedThisTurn,
        bool& isExtended
    );
};
