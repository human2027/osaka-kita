#pragma once
#include <memory>
#include <string>

#include "Player.h"
#include "AIPlayer.h"
#include "MapManager.h"
#include "UIManager.h"
#include "StateBase.h"
#include "TurnManager.h"
#include "Judge.h"
#include "Blackboard.h"
#include "InputBase.h"
#include "AnimationManager.h"

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

    // 状態遷移
    void ChangeState(std::shared_ptr<StateBase> newState);

    // 状態ごとの内部処理（State から呼ばれる）
    void UpdateInRound();
    void DrawGame();

    // ゲームの進行
    void StartRound();
    bool CheckEndCondition();

    void NextRound();
    void RestartGame();

public:
    std::shared_ptr<Player>     player;
    std::shared_ptr<AIPlayer>   aiPlayer;
    std::shared_ptr<Blackboard> blackboard;

    MapManager mapManager;
    UIManager  uiManager;
    AnimationManager animationManager;

    TurnManager turnManager{ Player_per_round };
    Judge judge;

    std::shared_ptr<StateBase> currentState;
    std::shared_ptr<InputBase> inputDevice;

    int roundNumber = 1;
    int playsThisRound = 0;

    int lastPlayerCard = 0;
    int lastAiCard = 0;

    std::string mainMessage;
    int messageTimer = 0;

    // アニメ更新用
    int prevMs = 0;
    float deltaTime = 0.0f;
};