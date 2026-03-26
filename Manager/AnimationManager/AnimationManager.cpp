#include "AnimationManager.h"

AnimationManager::AnimationManager()
    : aiController(&aiBank)
{
}

void AnimationManager::Initialize()
{
    aiBank.InitializeForBoardGame();
}

void AnimationManager::Update(float dt)
{
    aiController.Update(dt);
}

void AnimationManager::Draw() const
{
    aiController.Draw(aiDrawX, aiDrawY);
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