#include "AIPlayerLogic.h"

#include "Blackboard.h"
#include "MapManager.h"
#include "InitialValue.h"

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
        int wItem = AI_NEUTRAL_W_ITEM;
        int wHeal = AI_NEUTRAL_W_HEAL;
        int wDamageAvoid = AI_NEUTRAL_W_DAMAGE_AVOID;
        int wDamageEnemy = AI_NEUTRAL_W_DAMAGE_ENEMY;
        int wBlockHeal = AI_NEUTRAL_W_BLOCK_HEAL;
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

        // (1) HPñûÉ^ÉìÇ»ÇÁÉAÉCÉeÉÄë_Ç¢Åiñ¢èäéùÇ»ÇÁì¡Ç…Åj
        if (aiMax > 0 && aiHP >= aiMax && !bb.AIHasItem())
            return Intent::GetItem;

        // (2) í·HPÇ»ÇÁâÒïúóDêÊ
        if (bb.IsAIDangerHP(AI_DANGER_HP_RATIO))
            return Intent::HealSelf;

        // (3) ëäéËÇ™ïméÄÇ≈ÅAëäéËëOï˚Ç…âÒïúÇ™Ç†ÇÈÇ»ÇÁëjé~
        const int plHP = bb.GetPlayerHP();
        const int plMax = bb.GetPlayerMaxHP();
        const float pr = (plMax > 0) ? (float)plHP / (float)plMax : 1.0f;

        if (pr <= AI_BLOCK_ENEMY_HP_RATIO)
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
        Weights w;
        switch (intent)
        {
        case Intent::GetItem:
            w.wItem = AI_GETITEM_W_ITEM;
            w.wHeal = AI_GETITEM_W_HEAL;
            w.wDamageAvoid = AI_GETITEM_W_DAMAGE_AVOID;
            w.wDamageEnemy = AI_GETITEM_W_DAMAGE_ENEMY;
            w.wBlockHeal = AI_GETITEM_W_BLOCK_HEAL;
            break;

        case Intent::HealSelf:
            w.wItem = AI_HEALSELF_W_ITEM;
            w.wHeal = AI_HEALSELF_W_HEAL;
            w.wDamageAvoid = AI_HEALSELF_W_DAMAGE_AVOID;
            w.wDamageEnemy = AI_HEALSELF_W_DAMAGE_ENEMY;
            w.wBlockHeal = AI_HEALSELF_W_BLOCK_HEAL;
            break;

        case Intent::BlockHeal:
            w.wItem = AI_BLOCKHEAL_W_ITEM;
            w.wHeal = AI_BLOCKHEAL_W_HEAL;
            w.wDamageAvoid = AI_BLOCKHEAL_W_DAMAGE_AVOID;
            w.wDamageEnemy = AI_BLOCKHEAL_W_DAMAGE_ENEMY;
            w.wBlockHeal = AI_BLOCKHEAL_W_BLOCK_HEAL;
            break;

        case Intent::Neutral:
        default:
            w.wItem = AI_NEUTRAL_W_ITEM;
            w.wHeal = AI_NEUTRAL_W_HEAL;
            w.wDamageAvoid = AI_NEUTRAL_W_DAMAGE_AVOID;
            w.wDamageEnemy = AI_NEUTRAL_W_DAMAGE_ENEMY;
            w.wBlockHeal = AI_NEUTRAL_W_BLOCK_HEAL;
            break;
        }
        return w;
    }

    // result: +1=playerèüÇø, -1=AIèüÇø, 0=à¯Ç´ï™ÇØ
    static int JudgeWinnerMini(int playerCard, int aiCard, bool reverse)
    {
        if (!reverse)
        {
            if (playerCard > aiCard) return +1;
            if (playerCard < aiCard) return -1;
            return 0;
        }
        else
        {
            if (playerCard < aiCard) return +1;
            if (playerCard > aiCard) return -1;
            return 0;
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
            return info;
        }

        const Tile& t = map.GetTile(index);
        info.index = index;
        info.type = t.type;
        info.value = t.value;
        return info;
    }

    static int ScoreLandingAI(const TileInfoForAI& t, const Weights& w)
    {
        if (t.index < 0) return nothing_score;

        switch (t.type)
        {
        case TileType::Item:   return +w.wItem;
        case TileType::Heal:   return +w.wHeal;
        case TileType::Damage: return -w.wDamageAvoid;
        case TileType::Normal:
        default:               return 0;
        }
    }

    static int ScoreLandingPlayer(const TileInfoForAI& t, const Weights& w, Intent intent)
    {
        if (t.index < 0) return 0;

        int s = 0;
        switch (t.type)
        {
        case TileType::Item:
            s -= w.wItem;
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

        // (A) HPí·Ç¢ Å® è¨Ç≥Ç¢éDäÒÇË
        const int aiHP = bb.GetAIHP();
        const int aiMax = bb.GetAIMaxHP();
        const float ar = (aiMax > 0) ? (float)aiHP / (float)aiMax : 1.0f;

        if (ar <= AI_LOWHP_SMALL_RATIO)
        {
            bias += (-center) * AI_BIAS_LOWHP_SMALL;
        }

        // (B) à íuÇ≈ïâÇØÇƒÇÈ Å® ëÂÇ´Ç¢éDäÒÇË
        const int diff = bb.GetPlayerPos() - bb.GetAIPos(); // +Ç»ÇÁAIÇ™å„ÇÎ
        if (diff > 0)
        {
            int d = diff;
            if (d > AI_BEHIND_DIFF_CAP) d = AI_BEHIND_DIFF_CAP;

            const int scaled = (AI_BIAS_BEHIND_BIG * d) / AI_BEHIND_DIFF_CAP;
            bias += (center)*scaled;
        }

        return bias;
    }

    static bool ShouldYoloBehind(const Blackboard& bb)
    {
        const int diff = bb.GetPlayerPos() - bb.GetAIPos();
        if (diff < AI_BEHIND_YOLO_MIN_DIFF) return false;
        return Rand01() < AI_BEHIND_YOLO_PROB;
    }

    struct Cand
    {
        int card = 0;
        int score = 0;
    };

    static int SoftmaxPick(const std::vector<Cand>& cands, float temperature)
    {
        if (cands.empty()) return AI_CARD_MIN;

        int maxScore = cands[0].score;
        for (const auto& c : cands)
            maxScore = (std::max)(maxScore, c.score);

        const float T = (temperature <= 0.0001f) ? 0.0001f : temperature;

        double sumW = 0.0;
        std::vector<double> ws;
        ws.reserve(cands.size());

        for (const auto& c : cands)
        {
            const int noise = (rand() % (AI_NOISE_RANGE * 2 + 1)) - AI_NOISE_RANGE; // [-R..+R]
            const double x = (double)(c.score + noise - maxScore) / (double)T;
            const double xc = (x < -50.0) ? -50.0 : (x > 50.0 ? 50.0 : x);

            const double w = std::exp(xc);
            ws.push_back(w);
            sumW += w;
        }

        if (sumW <= 0.0)
            return cands.back().card;

        double r = (double)Rand01();
        double acc = 0.0;

        for (size_t i = 0; i < cands.size(); ++i)
        {
            acc += ws[i] / sumW;
            if (r <= acc)
                return cands[i].card;
        }
        return cands.back().card;
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

        // map Ç™ñ≥Ç¢Ç»ÇÁï€åØÅFêÊì™
        if (!map)
        {
            out.chosenCard = (int)myHand.front();
            out.yolo = false;
            return out;
        }

        // YOLOÅiã}Ç…ç≈ëÂéDÅj
        if (hasYoloCard && ShouldYoloBehind(bb))
        {
            out.chosenCard = AI_YOLO_CARD; // í èÌ5
            out.yolo = true;
            return out;
        }

        // ëäéËécÇËéDÅiBBÇ…ñ≥Ç¢Ç»ÇÁ1..5âºíËÅj
        std::vector<int> oppCards;
        const auto& ph = bb.GetPlayerHand();
        if (ph.empty())
        {
            for (int c = AI_CARD_MIN; c <= AI_CARD_MAX; ++c)
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

        // ïâÇØÇƒÇÈÇŸÇ«â∑ìxUP
        const int diff = bb.GetPlayerPos() - bb.GetAIPos();
        float behind01 = 0.0f;
        if (diff > 0)
        {
            int d = diff;
            if (d > AI_BEHIND_DIFF_CAP) d = AI_BEHIND_DIFF_CAP;
            behind01 = (float)d / (float)AI_BEHIND_DIFF_CAP;
        }
        const float temperature = AI_SOFTMAX_TEMP_BASE + behind01 * AI_SOFTMAX_TEMP_BEHIND_ADD;

        std::vector<Cand> cands;
        cands.reserve(myHand.size());

        for (int aiCard : myHand)
        {
            if (aiCard < AI_CARD_MIN || aiCard > AI_CARD_MAX) continue;

            long long sum = 0;
            int cnt = 0;

            for (int plCard : oppCards)
            {
                if (plCard < AI_CARD_MIN || plCard > AI_CARD_MAX) continue;

                const int result = JudgeWinnerMini(plCard, aiCard, reverse);

                int sc = 0;
                if (result < 0)
                {
                    // AIèüÇø Å® AIÇ™ì•Çﬁ
                    const TileInfoForAI aiLanding = GetTileInfoAt(*map, aiPos + aiCard);
                    sc = ScoreLandingAI(aiLanding, w);
                }
                else if (result > 0)
                {
                    // PlayerèüÇø Å® PlayerÇ™ì•ÇﬁÅiAIéãì_Åj
                    const TileInfoForAI plLanding = GetTileInfoAt(*map, plPos + plCard);
                    sc = ScoreLandingPlayer(plLanding, w, intent);
                }
                else
                {
                    sc = -AI_DRAW_PENALTY;
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