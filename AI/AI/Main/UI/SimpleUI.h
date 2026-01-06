#pragma once
#include <string>
#include "CharacterBase.h"

class SimpleUI
{
public:
    void Draw(
        const CharacterBase& player,
        const CharacterBase& ai,
        int roundNumber,
        int lastPlayerCard,
        int lastAiCard
    ) const;
};
