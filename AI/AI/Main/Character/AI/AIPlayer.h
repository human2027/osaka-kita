#pragma once
#include <memory>
#include "CharacterBase.h"
#include "Blackboard.h"
#include "MapManager.h"

class AIPlayer : public CharacterBase
{
public:

    explicit AIPlayer(std::shared_ptr<Blackboard> bb = nullptr);

    // 後から差し替えたいとき用
    void SetBlackboard(std::shared_ptr<Blackboard> bb) { blackboard = std::move(bb); }

    // CharacterBase の純粋仮想関数
    int ChooseCard() override;

    // AI から見た前方タイル評価を可視化用に返す
    struct AITileEval
    {
        int      steps;  // 何マス先か（1〜5）
        TileType type;   // タイル種別
        int      score;  // EvaluateTile の評価値
    };

    // 前方5マス分の評価を steps=1〜5 の順で返す
    std::vector<AITileEval> GetForwardEvaluation() const;
    void UpdateBlackboard(Blackboard& bb, const MapManager& map, int roundNumber, int playsThisRound);
private:
    std::shared_ptr<Blackboard> blackboard;

    int EvaluateTile(const TileInfoForAI& info) const;
};
