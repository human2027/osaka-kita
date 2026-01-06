#include "CharacterFactory.h"

#include "Player.h"
#include "AIPlayer.h"
#include "InputBase.h"
#include "InitialValue.h"
#include "Blackboard.h" 

// プレイヤー生成
std::shared_ptr<Player> CharacterFactory::CreatePlayer(
    std::shared_ptr<InputBase> inputDevice
)
{
    auto p = std::make_shared<Player>();

    p->SetHP(MAX_Player_HP);
    p->SetPos(0);
    p->ResetRound();

    p->SetInput(inputDevice);
    return p;
}

// AI 生成
std::shared_ptr<AIPlayer> CharacterFactory::CreateAI(
    AIDifficulty difficulty,
    std::shared_ptr<Blackboard> blackboard
)
{
    //Blackboard をコンストラクタに渡す
    auto ai = std::make_shared<AIPlayer>(blackboard);

    ai->SetHP(MAX_Player_HP);
    ai->SetPos(0);
    ai->ResetRound();

    // 難易度に応じてパラメータをいじるならここ
    switch (difficulty)
    {
    case AIDifficulty::Easy:
        // ai->SetAggressiveness(0.3f);
        break;
    case AIDifficulty::Normal:
        // ai->SetAggressiveness(0.6f);
        break;
    case AIDifficulty::Hard:
        // ai->SetAggressiveness(0.9f);
        break;
    }

    return ai;
}
