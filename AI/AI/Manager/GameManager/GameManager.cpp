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

GameManager::GameManager()
{
}

// 初期化
void GameManager::Initialize()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // Blackboard
    blackboard = std::make_shared<Blackboard>();

    // プレイヤー / AI 生成
    player = std::make_shared<Player>();
    aiPlayer = std::make_shared<AIPlayer>(blackboard);

    // 入力デバイス（キーボード）設定
    inputDevice = std::make_shared<KeyboardInput>();
    player->SetInput(inputDevice);

    // マップ生成 & 設定
    auto fixedMap = std::make_shared<NormalMap>();
    mapManager.SetMap(fixedMap);
    mapManager.Initialize();

    roundNumber = 1;
    playsThisRound = 0;
    lastPlayerCard = 0;
    lastAiCard = 0;

    player->ResetRound();
    aiPlayer->ResetRound();

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

    turnManager.StartNewRound(*player, *aiPlayer);

    uiManager.ShowRoundStart(roundNumber);

    ChangeState(std::make_shared<InRoundState>());
}

// 勝利条件チェック
// true ならゲーム終了
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
    // 位置・ラウンドの基本情報を Blackboard に書き込む
    blackboard->SetPositions(player->GetPos(), aiPlayer->GetPos());
    blackboard->SetRoundInfo(roundNumber, playsThisRound);

    // AI 用の情報更新は AIPlayer 側に任せる
    aiPlayer->UpdateBlackboard(*blackboard, mapManager, roundNumber, playsThisRound);

    // 1ターン進行
    bool turned = turnManager.ExecuteTurn(
        *player,
        *aiPlayer,
        mapManager,
        judge
    );

    if (!turned) {
        // 何も起きなかった（カード無効など）
        return;
    }

    // TurnManager からラウンド内の情報を取り出して保持
    playsThisRound = turnManager.GetPlaysThisRound();
    lastPlayerCard = turnManager.GetLastPlayerCard();
    lastAiCard = turnManager.GetLastAICard();
}

// 共通の描画処理
void GameManager::DrawGame()
{
    // マップの描画（マスの色など）
    mapManager.Draw();
    uiManager.Draw(
        player, aiPlayer,
        roundNumber, playsThisRound,
        lastPlayerCard, lastAiCard
    );
}

// メイン更新
void GameManager::Update()
{
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

// ラウンド番号操作用の小さいヘルパ（RoundResultState から使う）
void GameManager::NextRound()
{
    ++roundNumber;
    StartRound();
}

// ゲーム再スタート用ヘルパ（GameEndState から使う）
void GameManager::RestartGame()
{
    Initialize();
}
