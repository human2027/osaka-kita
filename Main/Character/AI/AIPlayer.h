#pragma once
#include <vector>
#include <memory>

#include "CharacterBase.h"   
#include "BlackBoard.h"
#include "MapManager.h"
#include "MapBase.h"
#include "AITypes.h"
#include "AIAnimationController.h"

class AIPlayer : public CharacterBase
{
public:
    explicit AIPlayer(std::shared_ptr<Blackboard> bb);

    void SetBlackboard(std::shared_ptr<Blackboard> bb) { blackboard = std::move(bb); }

    int ChooseCard() override;

    struct AITileEval
    {
        int      steps;
        TileType type;
        int      score;
    };

    std::vector<AITileEval> GetForwardEvaluation() const;



    void UpdateBlackboard(Blackboard& bb, const MapManager& map, int roundNumber, int playsThisRound);

private:
    const MapManager* cachedMap = nullptr;
    std::shared_ptr<Blackboard> blackboard;

    int EvaluateTile(const TileInfoForAI& info) const;
};