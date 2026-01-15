#pragma once
#include <vector>
#include "InitialValue.h"

class CharacterBase  {
public:

    CharacterBase();

    virtual ~CharacterBase() = default;

    //基本HP
    int GetHP() const { return hp; }

    //現在のマス位置
    int GetPos() const { return pos; }

    //所持アイテム
    ItemType GetHeldItem() const { return heldItem; }

    //HP
    void SetHP(int v) { hp = v; }

    //場所
    void SetPos(int v) { pos = v; }

    //所持アイテム
    void SetHeldItem(ItemType it) { heldItem = it; }

    //HP回復の上限値
    void AddHP(int v);

    //数字は整数で進む
    void MovePos(int v);

    //アイテムフラグ
    bool IsBoostActive() const { return boostActive; }

    //反転
    bool IsReverseActive() const { return reverseActive; }
    //ブーストアイテム

    void ActivateBoost() { boostActive = true; }
    //反転アイテム

    void ActivateReverse() { reverseActive = true; }

    void ResetRound();  // 毎ラウンドの初期化

    // --- 手札 ---
    void InitHand();

    //そのカードを所持しているか
    bool HasCard(int c) const;

    //使ったカードは使えない
    void RemoveCard(int c);

    //手札参照を返す
    const std::vector<int>& GetHand() const { return hand; }

    //AIが学習する用の履歴
    void AddRoundHistory(int card);
    const std::vector<int>& GetRoundHistory() const { return roundHistory; }

    //Player / AI 専用動作
    virtual int ChooseCard() = 0;  // AIとPlayerで実装が違う

    //プレイヤーのアニメーション
    virtual void PlayAnimation(int animType) {}

    //AIの表情
    virtual void AIAnimation(int animType) {}

protected:
    int hp = 10;
    int pos = 0;

    ItemType heldItem = ItemType::Item_none;

    bool boostActive = false;
    bool reverseActive = false;

    std::vector<int> hand;
    std::vector<int> roundHistory;
};
