#pragma once
#include "BehaviorNode.h"
#include "Blackboard.h"
#include "AIPlayer.h"
#include <limits>
#include <algorithm>

class SurviveHP : public BTNode
{
public:
    BTStatus Tick(AIPlayer& ai, const Blackboard& bb) override
    {
        const auto& myHand = ai.GetHand();
        if (myHand.empty())
            return BTStatus::Failure;

        int bestCard = -1;
        int bestScore = std::numeric_limits<int>::min();

        const bool critical = bb.IsAICriticalHP(0.15f);
        const bool danger = bb.IsAIDangerHP(0.30f);

        for (int card : myHand)
        {
            const int score = Evaluate(ai, bb, card, critical, danger);
            if (score > bestScore)
            {
                bestScore = score;
                bestCard = card;
            }
        }

        if (bestCard < 0)
            return BTStatus::Failure;

        ai.HasCard(bestCard);
        return BTStatus::Success;
    }

private:
    int Evaluate(AIPlayer& ai, const Blackboard& bb, int card, bool critical, bool danger)
    {
        // 1) 次の位置（あなたのルール：前に card 進む）
        const int aiPos = bb.GetAIPos();
        const int nextPos = aiPos + card;

        // 2) 次のマス（5マス先情報から引く）
        const TileInfoForAI* tile = FindTile(bb.GetAIForwardTiles(), nextPos);

        // 5マス以内のはずだが、万一見つからないなら控えめに減点
        if (!tile)
            return -500;

        int score = 0;

        // 3) タイルによる“生存スコア”
        //   HPが低いほどダメージの重みを増やす
        const int hpNow = bb.GetAIHP();

        switch (tile->type)
        {
        case TileType::Heal:
            // 回復は最優先。valueが大きいほど加点
            score += 5000 + tile->value * 30;
            break;

        case TileType::Damage:
        {
            // ダメージは致命域ならほぼ禁止級
            const int base = critical ? 9000 : (danger ? 5000 : 2500);
            score -= base + tile->value * (critical ? 80 : (danger ? 50 : 30));

            // これ踏んだら死に得るなら超減点（即死回避）
            if (hpNow <= tile->value)
                score -= 200000;
            break;
        }

        case TileType::Item:
            // 生存モードでは少し嬉しい（回復アイテムの可能性）
            score += 600;
            break;

        case TileType::Normal:
        default:
            score += 200;
            break;
        }

        //勝敗の“生存スコア”
        //   相手残り札(Blackboard)から、勝/負/引き分け確率を計算
        const auto& enemyHand = bb.GetPlayerHand();
        if (!enemyHand.empty())
        {
            int win = 0, lose = 0, draw = 0;
            for (int e : enemyHand)
            {
                if (card > e) ++win;
                else if (card < e) ++lose;
                else ++draw;
            }

            // HPが低いほど「負け」を重く扱う
            // （負けの意味：被ダメ・押し負け・主導権喪失 等、あなたのルールに合わせて後で調整）
            const int loseWeight = critical ? 1200 : (danger ? 700 : 350);
            const int winWeight = critical ? 180 : (danger ? 140 : 100);
            const int drawWeight = 0;

            score += win * winWeight;
            score -= lose * loseWeight;
            score += draw * drawWeight;

            // 追加：致命域なら「負けが確定しやすいカード」をさらに嫌う
            if (critical && lose > win)
                score -= 1500;
        }

        // 
        //   低HP時は「勝てるなら最小カードで勝つ」方が温存につながる場合が多い
        //   ただしあなたのゲームで“強カード温存”が重要なら有効。不要なら消してOK。
        if (danger)
            score -= card * 10; // 大きいカードほど少し減点（温存寄り）

        return score;
    }

    const TileInfoForAI* FindTile(const std::array<TileInfoForAI, 5>& fwd, int index)
    {
        for (const auto& t : fwd)
        {
            if (t.index == index)
                return &t;
        }
        return nullptr;
    }
};
