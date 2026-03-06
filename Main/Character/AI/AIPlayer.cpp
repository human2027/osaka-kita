#include "AIPlayer.h"
#include "AIPlayerLogic.h"
#include "Blackboard.h"
#include "MapManager.h"
#include "InitialValue.h"
#include <array>
#include <vector>

AIPlayer::AIPlayer(std::shared_ptr<Blackboard> bb)
    : blackboard(std::move(bb))
{
}

void AIPlayer::UpdateBlackboard(Blackboard& bb, const MapManager& map, int /*roundNumber*/, int /*playsThisRound*/)
{
    cachedMap = &map;

    // AI 前方5マス
    std::array<TileInfoForAI, 5> aiForward{};
    const int aiPos = GetPos();
    const int mapSize = map.GetSize();

    for (int steps = AI_Card_Min; steps <= AI_Card_Max; ++steps)
    {
        TileInfoForAI info{};
        const int tileIndex = aiPos + steps;

        if (tileIndex < 0 || tileIndex >= mapSize)
        {
            info.index = -1;
            info.type = TileType::Normal;
            info.value = 0;
        }
        else
        {
            const Tile& t = map.GetTile(tileIndex);
            info.index = tileIndex;
            info.type = t.type;
            info.value = t.value;
        }
        aiForward[steps - 1] = info;
    }
    bb.SetAIForwardTiles(aiForward);

    // Player 前方5マス
    std::array<TileInfoForAI, 5> plForward{};
    const int plPos = bb.GetPlayerPos();

    for (int steps = AI_Card_Min; steps <= AI_Card_Max; ++steps)
    {
        TileInfoForAI info{};
        const int tileIndex = plPos + steps;

        if (tileIndex < 0 || tileIndex >= mapSize)
        {
            info.index = -1;
            info.type = TileType::Normal;
            info.value = 0;
        }
        else
        {
            const Tile& t = map.GetTile(tileIndex);
            info.index = tileIndex;
            info.type = t.type;
            info.value = t.value;
        }
        plForward[steps - 1] = info;
    }
    bb.SetPlayerForwardTiles(plForward);
}


// タイル評価（可視化用途）
int AIPlayer::EvaluateTile(const TileInfoForAI& info) const
{
    if (info.index < 0)
        return Nothing_Score;

    int score = 0;
    switch (info.type)
    {
    case TileType::Damage:
        score -= AI_Neutral_W_Damage_Avoid;
        score -= info.value;
        break;
    case TileType::Heal:
        score += AI_Neutral_W_Heal;
        score += info.value;
        break;
    case TileType::Item:
        score += AI_Neutral_W_Iteam;
        break;
    case TileType::Normal:
    default:
        break;
    }
    return score;
}

std::vector<AIPlayer::AITileEval> AIPlayer::GetForwardEvaluation() const
{
    std::vector<AITileEval> result;
    if (!blackboard) return result;

    const auto& forward = blackboard->GetAIForwardTiles();

    result.reserve(AI_Card_Max - AI_Card_Min + 1);

    for (int steps = AI_Card_Min; steps <= AI_Card_Max; ++steps)
    {
        const TileInfoForAI& info = forward[steps - 1];

        AITileEval e{};
        e.steps = steps;
        e.type = info.type;
        e.score = EvaluateTile(info);

        result.push_back(e);
    }
    return result;
}

int AIPlayer::ChooseCard()
{
    if (!blackboard) return -1;

    const auto& myHand = GetHand();
    if (myHand.empty()) return -1;

    const bool hasYolo = HasCard(AI_Yolo_Card);

    auto dec = AIPlayerLogic::ChooseCard(*blackboard, cachedMap, myHand, hasYolo);

    blackboard->SetAIDecision(dec.intent, dec.chosenCard, dec.yolo);

    return dec.chosenCard;
}