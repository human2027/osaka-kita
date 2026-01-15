#include "Player.h"
#include "InputBase.h"

Player::Player()
    : CharacterBase()   // HP/pos/手札 初期化
{
    input = nullptr;
}

void Player::SetInput(const std::shared_ptr<InputBase>& inputDevice)
{
    input = inputDevice;
}

int Player::ChooseCard()
{
    if (!input) return 0;

    int card = input->GetSelectedCard();

    // そのカードをまだ持っているか
    if (!HasCard(card))
    {
        return 0;  
    }

    return card;
}

void Player::PlayAnimation(int animType)
{
    // 将来的手を出すモーション(別の.cpp .hを作るかも)
}
