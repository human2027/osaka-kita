#pragma once

#include <functional>
#include "UIEvent.h"
#include "InitialValue.h"
#include "CharacterBase.h"
#include "MapManager.h"
#include "Judge.h"
#include "Blackboard.h"

class TurnManager
{
public:
    explicit TurnManager(int maxTurnsPerRound);

    void StartNewRound(CharacterBase& player, CharacterBase& ai);
    bool ResolveTurn(
        CharacterBase& player,
        CharacterBase& ai,
        int playerCard,
        int aiCard,
        MapManager& map,
        Judge& judge,
        Blackboard& bb
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

    //ƒ‰ƒbƒp
    void PushUIEvent(UIMessageType type);                         // type‚¾‚¯
    void PushUIEvent(UIMessageType type, int value1);             // type + value1
    void PushUIEvent(UIMessageType type, int value1, int duration); // type + value1 + duration

    void PushUIEvent(UIMessageType type, int value1, int value2, int duration);
};
