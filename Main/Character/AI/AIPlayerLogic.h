#pragma once

#include <vector>
#include "BlackBoard.h"
#include "MapManager.h"

namespace AIPlayerLogic
{
    struct AIDecision
    {
        AIIntent intent = AIIntent::Neutral;
        int  chosenCard = 0;  // 1..5
        bool yolo = false;
    };

    AIDecision ChooseCard(
        const Blackboard& bb,
        const MapManager* map,
        const std::vector<int>& myHand,
        bool hasYoloCard
    );
}