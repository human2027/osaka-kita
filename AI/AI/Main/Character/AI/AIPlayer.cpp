#include "AIPlayer.h"
#include <limits>

AIPlayer::AIPlayer(std::shared_ptr<Blackboard> bb)
    : blackboard(std::move(bb))
{
   
}

void AIPlayer::UpdateBlackboard(Blackboard& bb, const MapManager& map, int roundNumber, int playsThisRound)
{
    // 位置・ラウンド情報は GameManager 側で設定済みだが、
    // 必要ならここで AI 独自の情報を追加で書き込んでもよい

    // AI から見た前方 5 マス情報を Blackboard に書き込む
    std::array<TileInfoForAI, 5> aiForward{};
    int aiPos = GetPos();          // CharacterBase から現在位置を取得
    int mapSize = map.GetSize();

    for (int steps = 1; steps <= 5; ++steps)
    {
        TileInfoForAI info{};
        int tileIndex = aiPos + steps;

        if (tileIndex < 0 || tileIndex >= mapSize)
        {
            // 範囲外：index < 0 にしておけば EvaluateTile が -10000 にしてくれる
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
}
// タイル評価
int AIPlayer::EvaluateTile(const TileInfoForAI& info) const
{
    if (info.index < 0)
    {
        return  nothing_score;
    }

    int score = 0;

    switch (info.type)
    {
    case TileType::Damage:
        score -= Damage_score;
        score -= info.value;
        break;
    case TileType::Heal:
        score += Heal_score;
        score += info.value;
        break;
    case TileType::Item:
        score += Item_score;
        break;
    case TileType::Normal:
    default:
        break;
    }

    return score;
}

// 手札から 1 枚カードを選ぶ
int AIPlayer::ChooseCard()
{
    if (!blackboard)
    {
        return -1;
    }

    const auto& handRef = GetHand();
    if (handRef.empty())
    {
        return -1;
    }

    const auto& forward = blackboard->GetAIForwardTiles();
    if (forward.empty())
    {
        // 安全策：情報が無いときは適当に先頭
        return handRef.front();
    }

    int bestScore = (std::numeric_limits<int>::min)();
    std::vector<int> candidates;  // 同点で一番良い候補たち

    for (int cardValue : handRef)
    {
        int steps = cardValue;

        // 歩数の範囲チェック
        if (steps <= 0 || steps > 5)
        {
            continue;
        }

        // forward の範囲チェック（万一短いときの保険）
        if (static_cast<size_t>(steps - 1) >= forward.size())
        {
            continue;
        }

        const TileInfoForAI& tileInfo = forward[steps - 1];
        int score = EvaluateTile(tileInfo);

        if (score > bestScore)
        {
            bestScore = score;
            candidates.clear();
            candidates.push_back(cardValue);
        }
        else if (score == bestScore)
        {
            candidates.push_back(cardValue);
        }
    }

    if (candidates.empty())
    {
        // 評価対象がなかったときはとりあえず先頭カードを返す
        return handRef.front();
    }

    //同点の中からランダムに 1 枚選ぶ
    int idx = rand() % static_cast<int>(candidates.size());
    int bestCard = candidates[idx];

    return bestCard;
}

std::vector<AIPlayer::AITileEval> AIPlayer::GetForwardEvaluation() const
{
    std::vector<AITileEval> result;

    if (!blackboard)
    {
        return result;
    }

    const auto& forward = blackboard->GetAIForwardTiles();

    for (int steps = 1; steps <= 5; ++steps)
    {
        const TileInfoForAI& info = forward[steps - 1];

        AITileEval e;
        e.steps = steps;
        e.type = info.type;
        e.score = EvaluateTile(info); 

        result.push_back(e);
    }

    return result;
}
