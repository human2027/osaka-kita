#include "AnimationManager.h"
#include "InitialValue.h"
#include "AnimDB.h"

AnimationManager::AnimationManager()
    : aiController(&aiBank)
    , playerController(&playerBank)
    , enemyMapAnim(&aiBank)
    , playerMapAnim(&playerBank)
{
}

void AnimationManager::Initialize()
{
    aiBank.InitializeForBoardGame();
    playerBank.InitializeForBoardGame();

    PlayEnemyMapAnimation();

}

void AnimationManager::Update(float dt)
{
    aiController.Update(dt);
    playerController.Update(dt);

    enemyMapAnim.Update(dt);
    playerMapAnim.Update(dt);
}

void AnimationManager::Draw() const
{
    aiController.Draw(aiDrawX, aiDrawY);
    playerController.Draw(playerDrawX, playerDrawY);
}

void AnimationManager::OnAIChooseCard(int cardValue, AnimMood mood)
{
    aiController.OnChooseCard(cardValue, mood);
}

void AnimationManager::SetAIDrawPosition(int x, int y)
{
    aiDrawX = x;
    aiDrawY = y;
}

bool AnimationManager::IsAIShowingCard() const
{
    return aiController.IsShowingCard();
}

void AnimationManager::OnPlayerTurnWin()
{
    playerController.OnTurnWin();
}

void AnimationManager::SetPlayerDrawPosition(int x, int y)
{
    playerDrawX = x;
    playerDrawY = y;
}

bool AnimationManager::IsPlayerTurnWinPlaying() const
{
    return playerController.IsPlayingTurnWin();
}

void AnimationManager::StopPlayerTurnWin()
{
    playerController.StopTurnWin();
}

void AnimationManager::PlayEnemyMapAnimation()
{
    {
        auto spec = AnimDB::FindWithFallback(
            AnimGroup::Enemy,
            AnimAction::Map,
            AnimMood::Normal
        );

        if (spec.has_value())
        {
            enemyMapAnim.Play(
                spec->key,
                spec->loop,
                spec->fps
            );
        }
    }

    {
        auto spec = AnimDB::FindWithFallback(
            AnimGroup::Player,
            AnimAction::Map,
            AnimMood::Normal
        );

        if (spec.has_value())
        {
            playerMapAnim.Play(
                spec->key,
                spec->loop,
                spec->fps
            );
        }
    }
}

void AnimationManager::DrawPlayerMapChar(int centerX, int centerY) const
{
    playerMapAnim.Draw(
        centerX - MapChar_drawOffsetX,
        centerY - MapChar_drawOffsetY
    );
}

void AnimationManager::DrawEnemyMapChar(int centerX, int centerY) const
{
    enemyMapAnim.Draw(
        centerX - MapChar_drawOffsetX,
        centerY - MapChar_drawOffsetY
    );
}