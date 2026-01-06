#pragma once

#include <functional>
#include "UIEvent.h"

class CharacterBase;
class MapManager;
class Judge;

class TurnManager
{
public:
    explicit TurnManager(int maxTurnsPerRound);

    void StartNewRound(CharacterBase& player, CharacterBase& ai);

    bool ExecuteTurn(
        CharacterBase& player,
        CharacterBase& ai,
        MapManager& map,
        Judge& judge
    );

    using UIEventCallback = std::function<void(const UIEvent&)>;
    void SetUIEventCallback(const UIEventCallback& cb)
    {
        uiEventFunc = cb;
    }

    int  GetPlaysThisRound()     const { return playsThisRound; }
    int  GetLastPlayerCard()     const { return lastPlayerCard; }
    int  GetLastAICard()         const { return lastAICard; }
    bool IsRoundFinished()       const { return roundFinished; }
    bool HasPlayerReachedGoal()  const { return playerReachedGoal; }
    bool HasAIReachedGoal()      const { return aiReachedGoal; }

private:
    void PushUIEvent(UIMessageType type, int value1 = 0, int duration = 0);

private:
    int maxTurnsPerRound = 3;

    int playsThisRound = 0;
    int lastPlayerCard = 0;
    int lastAICard = 0;

    bool roundFinished = false;
    bool playerReachedGoal = false;
    bool aiReachedGoal = false;

    int  playerWinsThisRound = 0;
    int  aiWinsThisRound = 0;
    int  drawsThisRound = 0;
    bool sameNumberOccurred = false;

    int  segmentDraws = 0;
    bool segmentSameNumber = false;

    bool isExtended = false;
    int  currentTurnLimit = 0;

    UIEventCallback uiEventFunc;
};
