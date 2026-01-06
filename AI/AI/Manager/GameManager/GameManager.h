#pragma once
#include <memory>
#include <string>

#include "Player.h"
#include "MapManager.h"
#include "UIManager.h"
#include "StateBase.h"
#include "TurnManager.h"
#include "Judge.h"
#include "Blackboard.h"

class GameManager
{
public:
    GameManager();
    void Initialize();

    void Update();     
    void Draw();

    bool IsRoundFinished() const;

    bool HasPlayerReachedGoal() const;

    bool HasAIReachedGoal() const;

    //状態遷移
    void ChangeState(std::shared_ptr<StateBase> newState);

    //状態ごとの内部処理（Stateから呼ばれる
    void UpdateInRound();
    void DrawGame();

    //ゲームの進行
    void StartRound();
    bool CheckEndCondition();

    void NextRound();
    void RestartGame();

public:
   
    std::shared_ptr<Player>    player;
    std::shared_ptr<AIPlayer>  aiPlayer;
    std::shared_ptr<Blackboard> blackboard;

    MapManager mapManager;
    UIManager  uiManager;
    
    TurnManager turnManager{ Player_per_round }; // 初期化
    Judge       judge;
    std::shared_ptr<StateBase> currentState;  
    std::shared_ptr<InputBase> inputDevice;

    int roundNumber = 1;
    int playsThisRound = 0;

    int lastPlayerCard = 0;
    int lastAiCard = 0;

    std::string mainMessage;
    int messageTimer = 0;
};
