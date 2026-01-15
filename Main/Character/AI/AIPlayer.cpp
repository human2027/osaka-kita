#include "AIPlayer.h"
#include "InitialValue.h"

#include <limits>
#include <vector>
#include <cstdlib> 
#include <array>
#include <cmath> 
#include <algorithm>


namespace
{
    enum class Intent
    {
        GetItem,     // HP満タン&未所持：アイテム優先
        HealSelf,    // 低HP：回復優先
        BlockHeal,   // 相手回復阻止
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

    static Intent DecideIntent(const Blackboard& bb)
    {
        const int aiHP = bb.GetAIHP();
        const int aiMax = bb.GetAIMaxHP();

        // (1) HP満タンならアイテム狙い（未所持なら特に）
        if (aiMax > 0 && aiHP >= aiMax && !bb.AIHasItem())
            return Intent::GetItem;

        // (2) 低HPなら回復優先
        if (bb.IsAIDangerHP(AI_DANGER_HP_RATIO))
            return Intent::HealSelf;

        // (4) 相手が瀕死で、前方に回復があるなら阻止
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

    // 逆転ルール付き勝敗
    // result: +1=player勝ち, -1=AI勝ち, 0=引き分け（引き分けは進まない前提）
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
            // 小さい方が勝ち
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
        // 相手が得するほどAIにはマイナス、相手が損するほどAIにはプラス
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

    // 札の傾向バイアス
    // - HPが低いほど小さい札を出しやすい
    // - 位置的に負けてる（AIが後ろ）ほど大きい札を出しやすい
    static int CardPreferenceBias(const Blackboard& bb, int aiCard)
    {
        const int center = aiCard - 3; // 1:-2,2:-1,3:0,4:+1,5:+2
        int bias = 0;

        // (A) HPが低い → 小さい札寄り
        const int aiHP = bb.GetAIHP();
        const int aiMax = bb.GetAIMaxHP();
        const float ar = (aiMax > 0) ? (float)aiHP / (float)aiMax : 1.0f;

        if (ar <= AI_LOWHP_SMALL_RATIO)
        {
            // 小さいほどプラス： -center
            bias += (-center) * AI_BIAS_LOWHP_SMALL;
        }

        // (B) 位置で負けてる → 大きい札寄り
        const int diff = bb.GetPlayerPos() - bb.GetAIPos(); // +ならAIが後ろ
        if (diff > 0)
        {
            int d = diff;
            if (d > AI_BEHIND_DIFF_CAP) d = AI_BEHIND_DIFF_CAP;

            const int scaled = (AI_BIAS_BEHIND_BIG * d) / AI_BEHIND_DIFF_CAP;
            bias += (center)*scaled;
        }

        return bias;
    }

    // 負けてる時に「急に最大札」を切る（読みやすさ対策の思い切り）
    static bool ShouldYoloBehind(const Blackboard& bb)
    {
        const int diff = bb.GetPlayerPos() - bb.GetAIPos(); // +ならAIが後ろ
        if (diff < AI_BEHIND_YOLO_MIN_DIFF) return false;
        return Rand01() < AI_BEHIND_YOLO_PROB;
    }

    struct Cand
    {
        int card = 0;   // 1..5
        int score = 0;  // expected + bias + など
    };

    // Softmaxで確率選択（温度付き + 小ノイズ）
    static int SoftmaxPick(const std::vector<Cand>& cands, float temperature)
    {
        if (cands.empty()) return AI_CARD_MIN;

        // 数値安定化：maxを引く
        int maxScore = cands[0].score;
        for(const auto& c : cands)
            maxScore = (std::max)(maxScore, c.score);

        double sumW = 0.0;
        std::vector<double> ws;
        ws.reserve(cands.size());

        const float T = (temperature <= 0.0001f) ? 0.0001f : temperature;

        for (const auto& c : cands)
        {
            const int noise = (rand() % (AI_NOISE_RANGE * 2 + 1)) - AI_NOISE_RANGE; // [-R..+R]
            const double x = (double)(c.score + noise - maxScore) / (double)T;

            // expの暴走防止
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

} // namespace


AIPlayer::AIPlayer(std::shared_ptr<Blackboard> bb)
    : blackboard(std::move(bb))
{
}

void AIPlayer::UpdateBlackboard(Blackboard& bb, const MapManager& map, int roundNumber, int playsThisRound)
{
    // ChooseCard() が map を参照できるように保持
    cachedMap = &map;

    // AI から見た前方 5 マス情報
    std::array<TileInfoForAI, 5> aiForward{};
    const int aiPos = GetPos();
    const int mapSize = map.GetSize();

    for (int steps = AI_CARD_MIN; steps <= AI_CARD_MAX; ++steps)
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

    // Player 前方5マス情報（回復阻止などの意図判定に必要）
    std::array<TileInfoForAI, 5> plForward{};
    const int plPos = bb.GetPlayerPos();

    for (int steps = AI_CARD_MIN; steps <= AI_CARD_MAX; ++steps)
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
        return nothing_score;

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

// 手札から 1 枚カードを選ぶ（期待値探索 + バイアス + Softmax + YOLO）
int AIPlayer::ChooseCard()
{
    if (!blackboard) return -1;

    const auto& myHand = GetHand();
    if (myHand.empty()) return -1;

    // UpdateBlackboardがまだ等の保険
    if (!cachedMap) return myHand.front();

    // ★負けてる時の思い切り（急に5）
    // ただし手札に5がある時だけ
    if (ShouldYoloBehind(*blackboard) && HasCard(AI_YOLO_CARD))
        return AI_YOLO_CARD;

    // 相手残り札（BBに入ってないなら 1..5 仮定）
    std::vector<int> oppCards;
    const auto& ph = blackboard->GetPlayerHand();
    if (ph.empty())
    {
        for (int c = AI_CARD_MIN; c <= AI_CARD_MAX; ++c)
            oppCards.push_back(c);
    }
    else
    {
        oppCards = ph;
    }

    const int aiPos = blackboard->GetAIPos();
    const int plPos = blackboard->GetPlayerPos();

    const Intent intent = DecideIntent(*blackboard);
    const Weights w = WeightsFor(intent);

    const bool reverse = blackboard->IsReverseActive();

    // 「負けてるほど温度UP（ブレやすくする）」
    const int diff = blackboard->GetPlayerPos() - blackboard->GetAIPos(); // +ならAIが後ろ
    float behind01 = 0.0f;
    if (diff > 0)
    {
        int d = diff;
        if (d > AI_BEHIND_DIFF_CAP) d = AI_BEHIND_DIFF_CAP;
        behind01 = (float)d / (float)AI_BEHIND_DIFF_CAP;
    }
    const float temperature = AI_SOFTMAX_TEMP_BASE + behind01 * AI_SOFTMAX_TEMP_BEHIND_ADD;

    // 各カードのスコアを作ってSoftmax抽選
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
                // AI勝ち → AIが踏むマス
                const TileInfoForAI aiLanding = GetTileInfoAt(*cachedMap, aiPos + aiCard);
                sc = ScoreLandingAI(aiLanding, w);
            }
            else if (result > 0)
            {
                // Player勝ち → Playerが踏むマス（AI視点で）
                const TileInfoForAI plLanding = GetTileInfoAt(*cachedMap, plPos + plCard);
                sc = ScoreLandingPlayer(plLanding, w, intent);
            }
            else
            {
                // 引き分けは進まない：好みでペナルティ
                sc = -AI_DRAW_PENALTY;
            }

            sum += sc;
            ++cnt;
        }

        if (cnt == 0) continue;

        int expected = (int)(sum / cnt);

        // 札の傾向（低HP→小さめ、負け→大きめ）
        expected += CardPreferenceBias(*blackboard, aiCard);

        cands.push_back({ aiCard, expected });
    }

    if (cands.empty()) return myHand.front();
    return SoftmaxPick(cands, temperature);
}

std::vector<AIPlayer::AITileEval> AIPlayer::GetForwardEvaluation() const
{
    std::vector<AITileEval> result;
    if (!blackboard) return result;

    const auto& forward = blackboard->GetAIForwardTiles();

    for (int steps = AI_CARD_MIN; steps <= AI_CARD_MAX; ++steps)
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