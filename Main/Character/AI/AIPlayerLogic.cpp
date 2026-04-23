#include "AIPlayerLogic.h"

#include "Blackboard.h"
#include "MapManager.h"
#include "InitialValue.h"
#include "Judge.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <limits>
#include <vector>
#include <AITypes.h>

namespace
{
    enum class Intent
    {
        GetItem,
        HealSelf,
        BlockHeal,
        Neutral
    };

    struct Weights
    {
        // アイテム種別ごとの重み
        int wItemHeal = 0;
        int wItemBoost = 0;
        int wItemReverse = 0;

        // 既存マス用
        int wHeal = AI_Neutral_W_Heal;
        int wDamageAvoid = AI_Neutral_W_Damage_Avoid;
        int wDamageEnemy = AI_Neutral_W_Damage_Enemy;
        int wBlockHeal = AI_Neutral_W_Block_Heal;
    };

    static float Rand01()
    {
        return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
    }

    static AIIntent ToAIIntent(Intent intent)
    {
        switch (intent)
        {
        case Intent::GetItem:   return AIIntent::GetItem;
        case Intent::HealSelf:  return AIIntent::HealSelf;
        case Intent::BlockHeal: return AIIntent::BlockHeal;
        case Intent::Neutral:
        default:                return AIIntent::Neutral;
        }
    }

    static Intent DecideIntent(const Blackboard& bb)
    {
        const int aiHP = bb.GetAIHP();
        const int aiMax = bb.GetAIMaxHP();
        const int plHP = bb.GetPlayerHP();
        const int plMax = bb.GetPlayerMaxHP();

        // HP満タンならアイテム狙い（未所持なら特に）
        if (aiMax > 0 && aiHP >= aiMax && !bb.AIHasItem())
            return Intent::GetItem;

        // 低HPなら回復優先
        if (bb.IsAIDangerHP(AI_Danger_HP_Ratio))
            return Intent::HealSelf;

        // 相手が瀕死で、相手前方に回復があるなら阻止
        const float pr = (plMax > 0) ? (float)plHP / (float)plMax : 1.0f;
        if (pr <= AI_Block_Enemy_HP_Ratio)
        {
            const auto& pf = bb.GetPlayerForwardTiles();
            for (int i = 0; i < 5; ++i)
            {
                if (pf[i].index >= 0 && pf[i].type == TileType::Heal)
                    return Intent::BlockHeal;
            }
        }

        return Intent::Neutral;
    }

    static Weights WeightsFor(Intent intent)
    {
        Weights w{};

        switch (intent)
        {
        case Intent::GetItem:
            // アイテム狙い：Boost と Reverse をやや高く
            w.wItemHeal = 20;
            w.wItemBoost = 35;
            w.wItemReverse = 30;

            w.wHeal = AI_GetItem_W_Item;
            w.wDamageAvoid = AI_GetItem_W_Damage_Avoid;
            w.wDamageEnemy = AI_GetItem_W_Damage_Enemy;
            w.wBlockHeal = AI_GetItem_W_Block_Heal;
            break;

        case Intent::HealSelf:
            // 低HP：回復アイテム最優先
            w.wItemHeal = 40;
            w.wItemBoost = 15;
            w.wItemReverse = 10;

            w.wHeal = AI_HealSelf_W_Heal;
            w.wDamageAvoid = AI_HealSelf_W_Damage_Avoid;
            w.wDamageEnemy = AI_HealSelf_W_Damage_Enemy;
            w.wBlockHeal = AI_HealSelf_W_Heal;
            break;

        case Intent::BlockHeal:
            // 相手回復阻止：反転や攻勢系を少し高め
            w.wItemHeal = 10;
            w.wItemBoost = 20;
            w.wItemReverse = 35;

            w.wHeal = AI_HealSelf_W_Heal;
            w.wDamageAvoid = AI_HealSelf_W_Damage_Avoid;
            w.wDamageEnemy = AI_HealSelf_W_Damage_Enemy;
            w.wBlockHeal = AI_HealSelf_W_Block_Heal;
            break;

        case Intent::Neutral:
        default:
            w.wItemHeal = 20;
            w.wItemBoost = 25;
            w.wItemReverse = 25;

            w.wHeal = AI_Neutral_W_Heal;
            w.wDamageAvoid = AI_Neutral_W_Damage_Avoid;
            w.wDamageEnemy = AI_Neutral_W_Damage_Enemy;
            w.wBlockHeal = AI_Neutral_W_Block_Heal;
            break;
        }

        return w;
    }

    static JudgeResult JudgeWinnerMini(int playerCard, int aiCard, bool reverse)
    {
        if (!reverse)
        {
            if (playerCard > aiCard) return JudgeResult::PlayerWin;
            if (playerCard < aiCard) return JudgeResult::AIWin;
            return JudgeResult::Draw;
        }
        else
        {
            if (playerCard < aiCard) return JudgeResult::PlayerWin;
            if (playerCard > aiCard) return JudgeResult::AIWin;
            return JudgeResult::Draw;
        }
    }

    static TileInfoForAI GetTileInfoAt(const MapManager& map, int index)
    {
        TileInfoForAI info{};
        const int mapSize = map.GetSize();

        if (index < 0 || index >= mapSize)
        {
            info.index = -1;
            info.type = TileType::Normal;
            info.value = 0;
            info.itemId = static_cast<int>(ItemType::Item_none);
            return info;
        }

        const Tile& t = map.GetTile(index);
        info.index = index;
        info.type = t.type;
        info.value = t.value;
        info.itemId = static_cast<int>(t.itemType);
        return info;
    }

    static int ScoreItemType(int itemId, const Weights& w)
    {
        const ItemType item = static_cast<ItemType>(itemId);

        switch (item)
        {
        case ItemType::Item_heal:
            return w.wItemHeal;

        case ItemType::Item_boost:
            return w.wItemBoost;

        case ItemType::Item_reverse:
            return w.wItemReverse;

        case ItemType::Item_none:
        default:
            return 0;
        }
    }

    static int ScoreLandingAI(const TileInfoForAI& t, const Weights& w)
    {
        if (t.index < 0) return Nothing_Score;

        switch (t.type)
        {
        case TileType::Item:
            return ScoreItemType(t.itemId, w);

        case TileType::Heal:
            return +w.wHeal;

        case TileType::Damage:
            return -w.wDamageAvoid;

        case TileType::Normal:
        default:
            return 0;
        }
    }

    static int ScoreLandingPlayer(const TileInfoForAI& t, const Weights& w, Intent intent)
    {
        if (t.index < 0) return 0;

        int s = 0;
        switch (t.type)
        {
        case TileType::Item:
            s -= ScoreItemType(t.itemId, w);
            break;

        case TileType::Heal:
            s -= w.wHeal;
            if (intent == Intent::BlockHeal) s -= w.wBlockHeal;
            break;

        case TileType::Damage:
            s += w.wDamageEnemy;
            break;

        case TileType::Normal:
        default:
            break;
        }
        return s;
    }

    static int CardPreferenceBias(const Blackboard& bb, int aiCard)
    {
        const int center = aiCard - 3;
        int bias = 0;

        // (A) HP低い → 小さい札寄り
        const int aiHP = bb.GetAIHP();
        const int aiMax = bb.GetAIMaxHP();
        const float ar = (aiMax > 0) ? (float)aiHP / (float)aiMax : 1.0f;

        if (ar <= AI_Low_HP_Small_Ratio)
        {
            bias += (-center) * AI_Bias_Low_HP_Small;
        }

        // (B) 位置で負けてる → 大きい札寄り
        const int diff = bb.GetPlayerPos() - bb.GetAIPos(); // +ならAIが後ろ
        if (diff > 0)
        {
            int d = diff;
            if (d > AI_Behind_Diff_Cap) d = AI_Behind_Diff_Cap;

            const int scaled = (AI_Bias_Behind_Big * d) / AI_Behind_Diff_Cap;
            bias += (center)*scaled;
        }

        return bias;
    }

    static bool ShouldYoloBehind(const Blackboard& bb)
    {
        const int diff = bb.GetPlayerPos() - bb.GetAIPos();
        if (diff < AI_Behind_Yolo_Min_Diff) return false;
        return Rand01() < AI_Behind_Yolo_Prob;
    }

    struct Cand
    {
        int card = 0;
        int score = 0;
    };

    static int SoftmaxPick(const std::vector<Cand>& candidates, float temperature)
    {
        if (candidates.empty()) return AI_Card_Min;

        int maxScore = candidates[0].score;
        for (const auto& candidate : candidates)
            maxScore = (std::max)(maxScore, candidate.score);

        const float Temperature = (temperature <= 0.0001f) ? 0.0001f : temperature;

        double sumWeight = 0.0;
        std::vector<double> weights;
        weights.reserve(candidates.size());

        for (const auto& candidate : candidates)
        {
            const int noise = (rand() % (AI_Noise_Range * 2 + 1)) - AI_Noise_Range; // [-R..+R]
            const double x = (double)(candidate.score + noise - maxScore) / (double)Temperature;
            const double xc = (x < -50.0) ? -50.0 : (x > 50.0 ? 50.0 : x);

            const double w = std::exp(xc);
            weights.push_back(w);
            sumWeight += w;
        }

        if (sumWeight <= 0.0)
            return candidates.back().card;

        double randomValue = (double)Rand01();
        double cumulativeProb = 0.0;

        for (size_t i = 0; i < candidates.size(); ++i)
        {
            cumulativeProb += weights[i] / sumWeight;
            if (randomValue <= cumulativeProb)
                return candidates[i].card;
        }
        return candidates.back().card;
    }
}

namespace AIPlayerLogic
{
    AIDecision ChooseCard(
        const Blackboard& bb,
        const MapManager* map,
        const std::vector<int>& myHand,
        bool hasYoloCard
    )
    {
        AIDecision out{};
        if (myHand.empty())
        {
            out.intent = AIIntent::Neutral;
            out.chosenCard = 0;
            out.yolo = false;
            return out;
        }

        const Intent intent = DecideIntent(bb);
        out.intent = ToAIIntent(intent);

        // map が無いなら保険：先頭
        if (!map)
        {
            out.chosenCard = (int)myHand.front();
            out.yolo = false;
            return out;
        }

        // YOLO（急に最大札）
        if (hasYoloCard && ShouldYoloBehind(bb))
        {
            out.chosenCard = AI_Yolo_Card; // 通常5
            out.yolo = true;
            return out;
        }

        // 相手残り札（BBに無いなら1..5仮定）
        std::vector<int> oppCards;
        const auto& ph = bb.GetPlayerHand();
        if (ph.empty())
        {
            for (int c = AI_Card_Min; c <= AI_Card_Max; ++c)
                oppCards.push_back(c);
        }
        else
        {
            oppCards = ph;
        }

        const int aiPos = bb.GetAIPos();
        const int plPos = bb.GetPlayerPos();

        const Weights w = WeightsFor(intent);
        const bool reverse = bb.IsReverseActive();

        // 負けてるほど温度UP
        const int diff = bb.GetPlayerPos() - bb.GetAIPos();
        float behind01 = 0.0f;
        if (diff > 0)
        {
            int d = diff;
            if (d > AI_Behind_Diff_Cap) d = AI_Behind_Diff_Cap;
            behind01 = (float)d / (float)AI_Behind_Diff_Cap;
        }
        // 評価値が高いカードほど選ばれやすくするため、
        // 各候補の score を Softmax 用の重みに変換する。
        // また、劣勢時は temperature を上げて選択に揺らぎを持たせる
        const float temperature = 
            AI_Softmax_Temp_Base + behind01 * AI_Softmax_Temp_Behind_Add;

        std::vector<Cand> cands;
        cands.reserve(myHand.size());

        for (int aiCard : myHand)
        {
            if (aiCard < AI_Card_Min || aiCard > AI_Card_Max) continue;

            long long sum = 0;
            int cnt = 0;

            for (int plCard : oppCards)
            {
                if (plCard < AI_Card_Min || plCard > AI_Card_Max) continue;

                const JudgeResult result = JudgeWinnerMini(plCard, aiCard, reverse);

                int sc = 0;
                if (result == JudgeResult::AIWin)
                {
                    // AI勝ち → AIが踏む
                    const TileInfoForAI aiLanding = GetTileInfoAt(*map, aiPos + aiCard);
                    sc = ScoreLandingAI(aiLanding, w);
                }
                else if (result == JudgeResult::PlayerWin)
                {
                    // Player勝ち → Playerが踏む（AI視点）
                    const TileInfoForAI plLanding = GetTileInfoAt(*map, plPos + plCard);
                    sc = ScoreLandingPlayer(plLanding, w, intent);
                }
                else
                {
                    sc = -AI_Draw_Penalty;
                }

                sum += sc;
                ++cnt;
            }

            if (cnt == 0) continue;

            int expected = (int)(sum / cnt);
            expected += CardPreferenceBias(bb, aiCard);

            cands.push_back({ aiCard, expected });
        }

        if (cands.empty())
        {
            out.chosenCard = (int)myHand.front();
            out.yolo = false;
            return out;
        }

        out.chosenCard = SoftmaxPick(cands, temperature);
        out.yolo = false;
        return out;
    }
}