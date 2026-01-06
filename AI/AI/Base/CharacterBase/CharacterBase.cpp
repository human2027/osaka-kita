#include "CharacterBase.h"
#include <algorithm>
#include "InitialValue.h"

CharacterBase::CharacterBase()
{
    hp = MAX_Player_HP;
    pos = 0;

    heldItem = ItemType::Item_none;
    boostActive = false;
    reverseActive = false;

    InitHand();     
    roundHistory.clear();
}
//回復処理
void CharacterBase::AddHP(int v) {
    hp += v;
    if (hp > MAX_Player_HP) hp = MAX_Player_HP;
}
//マスを進める
void CharacterBase::MovePos(int v) {
    pos += v;
}
//ラウンド開始の初期化
void CharacterBase::ResetRound() {
    boostActive = false;
    reverseActive = false;
    InitHand();
    roundHistory.clear();
}
//手札の初期化
void CharacterBase::InitHand() {
    hand = { 1,2,3,4,5 };
}

//カードを持っているのか
bool CharacterBase::HasCard(int c) const {
    return std::find(hand.begin(), hand.end(), c) != hand.end();
}

//手札からカードを削除
void CharacterBase::RemoveCard(int c) {
    auto it = std::find(hand.begin(), hand.end(), c);
    if (it != hand.end()) hand.erase(it);
}

//このラウンドで出したカードを記録
void CharacterBase::AddRoundHistory(int card) {
    roundHistory.push_back(card);
}
