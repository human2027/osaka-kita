#pragma once
#include <memory>
#include "CharacterBase.h"
#include "InputBase.h"
 
class Player : public CharacterBase
{
public:
    Player();
    virtual ~Player() = default;

    // 入力デバイスをセット（キーボード用・マウス用など）
    void SetInput(const std::shared_ptr<InputBase>& inputDevice);

    // 1ターンで出すカードを決める（プレイヤーは入力から取得）
    int ChooseCard() override;

    // プレイヤー固有アニメーション（今は空でOK）
    void PlayAnimation(int animType) override;

private:
    std::shared_ptr<InputBase> input;   // 入力用
};
