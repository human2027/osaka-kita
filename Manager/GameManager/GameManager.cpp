#include "GameManager.h"

#include "InitialValue.h"
#include "KeyboardInput.h"
#include "NormalMap.h"
#include "Judge.h"
#include "AIPlayer.h"
#include "InputBase.h"
#include "InRoundState.h"
#include "RoundResultState.h"
#include "GameEndState.h"
#include "MapLoader.h"
#include "DxLib.h"
#include <cstdlib>
#include <ctime>
#include "MouseInput.h"
GameManager::GameManager()
{
}

// 初期化
void GameManager::Initialize()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // Blackboard
    blackboard = std::make_shared<Blackboard>();

    // アニメ初期化
    animationManager.Initialize();
    animationManager.SetAIDrawPosition(AI_Draw_X, AI_Draw_Y);

    // プレイヤー / AI 生成
    player = std::make_shared<Player>();
    aiPlayer = std::make_shared<AIPlayer>(blackboard);

    // 入力デバイス（キーボード）設定
    inputDevice = std::make_shared<KeyboardInput>();
    player->SetInput(inputDevice);
    selectedPlayerCard = 0;
    // マップ生成 & 設定
    auto fixedMap = std::make_shared<NormalMap>();
    mapManager.SetMap(fixedMap);
    mapManager.Initialize();

    roundNumber = 1;
    playsThisRound = 0;
    lastPlayerCard = 0;
    lastAiCard = 0;

    pendingPlayerCard = -1;
    pendingAICard = -1;
    turnPhase = TurnPhase::StartTurn;

    player->ResetRound();
    aiPlayer->ResetRound();

    // 時間初期化
    prevMs = GetNowCount();
    deltaTime = 0.0f;

    // TurnManager → UI へのイベントコールバック
    turnManager.SetUIEventCallback(
        [this](const UIEvent& e)
        {
            uiManager.PushEvent(e);
        }
    );

    StartRound();
}

// ラウンド開始処理
void GameManager::StartRound()
{
    playsThisRound = 0;

    pendingPlayerCard = -1;
    pendingAICard = -1;
    selectedPlayerCard = 0;
    turnPhase = TurnPhase::StartTurn;

    turnManager.StartNewRound(*player, *aiPlayer);

    blackboard->SetPositions(player->GetPos(), aiPlayer->GetPos());
    blackboard->SetRoundInfo(roundNumber, playsThisRound);
    blackboard->SetPlayerHand(player->GetHand());
    blackboard->SetAIHand(aiPlayer->GetHand());
    blackboard->SetHP(player->GetHP(), MAX_Player_HP, aiPlayer->GetHP(), MAX_Player_HP);

    uiManager.ShowRoundStart(roundNumber);

    ChangeState(std::make_shared<InRoundState>());
}
// 勝利条件チェック
bool GameManager::CheckEndCondition()
{
    // 位置によるゴール
    if (mapManager.IsGoal(player->GetPos())) {
        uiManager.ShowPlayerGoalWin();
        ChangeState(std::make_shared<GameEndState>());
        return true;
    }

    if (mapManager.IsGoal(aiPlayer->GetPos())) {
        uiManager.ShowAIGoalWin();
        ChangeState(std::make_shared<GameEndState>());
        return true;
    }

    // HPによる決着
    if (player->GetHP() <= 0) {
        uiManager.ShowPlayerHPZeroLose();
        ChangeState(std::make_shared<GameEndState>());
        return true;
    }

    if (aiPlayer->GetHP() <= 0) {
        uiManager.ShowAIHPZeroLose();
        ChangeState(std::make_shared<GameEndState>());
        return true;
    }

    return false;
}

// 状態切り替え
void GameManager::ChangeState(std::shared_ptr<StateBase> newState)
{
    if (currentState) {
        currentState->Exit(*this);
    }
    currentState = newState;
    if (currentState) {
        currentState->Enter(*this);
    }
}

// 「ラウンド中」の進行処理
void GameManager::UpdateInRound()
{
    // Blackboard 更新
    blackboard->SetPositions(player->GetPos(), aiPlayer->GetPos());
    blackboard->SetRoundInfo(roundNumber, playsThisRound);
    blackboard->SetPlayerHand(player->GetHand());
    blackboard->SetAIHand(aiPlayer->GetHand());
    blackboard->SetHP(player->GetHP(), MAX_Player_HP, aiPlayer->GetHP(), MAX_Player_HP);

    aiPlayer->UpdateBlackboard(*blackboard, mapManager, roundNumber, playsThisRound);

    switch (turnPhase)
    {
    case TurnPhase::StartTurn:
    {
        pendingPlayerCard = -1;
        pendingAICard = -1;
        turnPhase = TurnPhase::AIChooseCard;
        break;
    }

    case TurnPhase::AIChooseCard:
    {
        pendingAICard = aiPlayer->ChooseCard();

        if (pendingAICard <= 0 || !aiPlayer->HasCard(pendingAICard))
        {
            const auto& hand = aiPlayer->GetHand();
            pendingAICard = hand.empty() ? 0 : static_cast<int>(hand.front());
        }

        if (pendingAICard > 0)
        {
            AnimMood mood = AnimMood::Normal;
            animationManager.OnAIChooseCard(pendingAICard, mood);
            turnPhase = TurnPhase::AIShowAnimation;
        }
        break;
    }

    case TurnPhase::AIShowAnimation:
    {
        // AIの手出しアニメが終わるまで待つ
        if (!animationManager.IsAIShowingCard())
        {
            turnPhase = TurnPhase::PlayerChooseCard;
        }
        break;
    }

    case TurnPhase::PlayerChooseCard:
    {
        if (mouseInput.IsLeftTriggered())
        {
            const int mx = mouseInput.GetX();
            const int my = mouseInput.GetY();

            const int hitCard = uiManager.HitTestPlayerCard(mx, my, player);
            if (hitCard > 0 && player->HasCard(hitCard))
            {
                selectedPlayerCard = hitCard;
            }

            if (uiManager.HitTestConfirmButton(mx, my))
            {
                if (selectedPlayerCard > 0 && player->HasCard(selectedPlayerCard))
                {
                    pendingPlayerCard = selectedPlayerCard;
                    turnPhase = TurnPhase::ResolveTurn;
                }
            }
        }
        break;
    }
    case TurnPhase::ResolveTurn:
    {
        // ここで初めて勝敗判定・移動・効果適用
        // TurnManager 側にこの関数を用意する想定
        bool turned = turnManager.ResolveTurn(
            *player,
            *aiPlayer,
            pendingPlayerCard,
            pendingAICard,
            mapManager,
            judge,
            *blackboard
        );

        if (!turned)
        {
            return;
        }

        // 表示用に保持
        playsThisRound = turnManager.GetPlaysThisRound();
        lastPlayerCard = pendingPlayerCard;
        lastAiCard = pendingAICard;

        // 勝敗条件チェック
        if (CheckEndCondition())
        {
            return;
        }

        turnPhase = TurnPhase::TurnEnd;
        break;
    }

    case TurnPhase::TurnEnd:
    {
        if (turnManager.IsRoundFinished())
        {
            ChangeState(std::make_shared<RoundResultState>());
        }
        else
        {
            turnPhase = TurnPhase::StartTurn;
        }
        break;
    }
    }
}
void GameManager::DrawGame()
{
    //マップの描画
    mapManager.Draw();

    // AIアニメ描画
    animationManager.Draw();

    // UI描画
    uiManager.Draw(
        player, aiPlayer,
        roundNumber, playsThisRound,
        lastPlayerCard, lastAiCard
    );

    uiManager.DrawPlayerHand(player, selectedPlayerCard);
    uiManager.DrawConfirmButton();
}

// メイン更新
void GameManager::Update()
{
    const int now = GetNowCount();
    int dtMs = now - prevMs;

    if (dtMs < 0) dtMs = 0;
    if (dtMs > 100) dtMs = 100;

    prevMs = now;
    deltaTime = static_cast<float>(dtMs) / 1000.0f;

    // 入力更新
    if (inputDevice)
    {
        inputDevice->Update();
    }

    mouseInput.Update();

    // アニメ更新
    animationManager.Update(deltaTime);

    if (currentState) {
        currentState->Update(*this);
    }
}
// メイン描画
void GameManager::Draw()
{
    if (currentState) {
        currentState->Draw(*this);
    }
}

bool GameManager::IsRoundFinished() const
{
    return turnManager.IsRoundFinished();
}

bool GameManager::HasPlayerReachedGoal() const
{
    return turnManager.HasPlayerReachedGoal();
}

bool GameManager::HasAIReachedGoal() const
{
    return turnManager.HasAIReachedGoal();
}

// ラウンド番号操作
void GameManager::NextRound()
{
    ++roundNumber;
    StartRound();
}

// ゲーム再スタート
void GameManager::RestartGame()
{
    Initialize();
}