#include "Blackboard.h"

// コンストラクタ
Blackboard::Blackboard()
    : playerPos(0)
    , aiPos(0)
    , roundNumber(1)
    , turnInRound(0)
    , playerHasItem(false)
    , aiHasItem(false)
    , playerHP(0)
    , playerMaxHP(1)
    , aiHP(0)
    , aiMaxHP(1)
{
}

// 位置情報
void Blackboard::SetPositions(int playerPos_, int aiPos_)
{
    playerPos = playerPos_;
    aiPos = aiPos_;
}

int Blackboard::GetPlayerPos() const
{
    return playerPos;
}

int Blackboard::GetAIPos() const
{
    return aiPos;
}
// ラウンド / ターン情報
void Blackboard::SetRoundInfo(int round, int turn)
{
    roundNumber = round;
    turnInRound = turn;
}

int Blackboard::GetRoundNumber() const
{
    return roundNumber;
}

int Blackboard::GetTurnInRound() const
{
    return turnInRound;
}

// プレイヤー残り札
void Blackboard::SetPlayerHand(const std::vector<int>& hand)
{
    playerHand = hand;
}

const std::vector<int>& Blackboard::GetPlayerHand() const
{
    return playerHand;
}

// アイテム保持フラグ
void Blackboard::SetItemFlags(bool playerHas, bool aiHas)
{
    playerHasItem = playerHas;
    aiHasItem = aiHas;
}

bool Blackboard::PlayerHasItem() const
{
    return playerHasItem;
}

bool Blackboard::AIHasItem() const
{
    return aiHasItem;
}

// 5マス先のタイル情報
void Blackboard::SetPlayerForwardTiles(const std::array<TileInfoForAI, 5>& tiles)
{
    playerForward = tiles;
}

void Blackboard::SetAIForwardTiles(const std::array<TileInfoForAI, 5>& tiles)
{
    aiForward = tiles;
}

const std::array<TileInfoForAI, 5>& Blackboard::GetPlayerForwardTiles() const
{
    return playerForward;
}

const std::array<TileInfoForAI, 5>& Blackboard::GetAIForwardTiles() const
{
    return aiForward;
}

// HP情報
void Blackboard::SetHP(int playerHP_, int playerMaxHP_,
    int aiHP_, int aiMaxHP_)
{
    playerHP = playerHP_;
    playerMaxHP = playerMaxHP_;

    aiHP = aiHP_;
    aiMaxHP = aiMaxHP_;
}
int Blackboard::GetPlayerHP() const
{
    return playerHP;
}

int Blackboard::GetPlayerMaxHP() const
{
    return playerMaxHP;
}

int Blackboard::GetAIHP() const
{
    return aiHP;
}

int Blackboard::GetAIMaxHP() const
{
    return aiMaxHP;
}

bool Blackboard::IsAIDangerHP(float ratio) const
{
    if (aiMaxHP <= 0) return false;
    const float r = static_cast<float>(aiHP) / static_cast<float>(aiMaxHP);
    return r <= ratio;
}

bool Blackboard::IsAICriticalHP(float ratio) const
{
    if (aiMaxHP <= 0) return false;
    const float r = static_cast<float>(aiHP) / static_cast<float>(aiMaxHP);
    return r <= ratio;
}
// AI残り札
void Blackboard::SetAIHand(const std::vector<int>& hand)
{
    aiHand = hand;
}

const std::vector<int>& Blackboard::GetAIHand() const
{
    return aiHand;
}

// 直前ターン情報
void Blackboard::SetLastTurn(int playerCard, int aiCard, int result)
{
    lastPlayerCard = playerCard;
    lastAICard = aiCard;
    lastResult = result;
}

int Blackboard::GetLastPlayerCard() const
{
    return lastPlayerCard;
}

int Blackboard::GetLastAICard() const
{
    return lastAICard;
}

int Blackboard::GetLastResult() const
{
    return lastResult;
}

void Blackboard::SetReverseActive(bool active)
{
    reverseActive = active;
}

bool Blackboard::IsReverseActive() const
{
    return reverseActive;
}

// ターン内通知フラグ
void Blackboard::ClearTurnFlags()
{
    playerUsedBoostThisTurn = false;
    aiUsedBoostThisTurn = false;
    playerUsedReverseThisTurn = false;
    aiUsedReverseThisTurn = false;
}

void Blackboard::NotifyPlayerUsedBoost()
{
    playerUsedBoostThisTurn = true;
}

void Blackboard::NotifyAIUsedBoost()
{
    aiUsedBoostThisTurn = true;
}

void Blackboard::NotifyPlayerUsedReverse()
{
    playerUsedReverseThisTurn = true;
}

void Blackboard::NotifyAIUsedReverse()
{
    aiUsedReverseThisTurn = true;
}

bool Blackboard::PlayerUsedBoostThisTurn() const
{
    return playerUsedBoostThisTurn;
}

bool Blackboard::AIUsedBoostThisTurn() const
{
    return aiUsedBoostThisTurn;
}

bool Blackboard::PlayerUsedReverseThisTurn() const
{
    return playerUsedReverseThisTurn;
}

bool Blackboard::AIUsedReverseThisTurn() const
{
    return aiUsedReverseThisTurn;
}
void Blackboard::SetAIDecision(AIIntent intent, int choseCard, bool yolo)
{
    aiIntent = intent;
    aiChoseCard = choseCard;
    aiYolo = yolo;
}

AIIntent Blackboard::GetAIIntent() const
{
    return aiIntent;
}

int Blackboard::GetAIChoseCard() const
{
    return aiChoseCard;
}

bool Blackboard::IsAIYolo() const
{
    return aiYolo;
}