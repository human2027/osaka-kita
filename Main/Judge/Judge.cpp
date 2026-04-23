#include "Judge.h"

JudgeResult Judge::JudgeWinner(int PlayerCard, int AICard, JudgeMode mode) const
{
    if (mode == JudgeMode::Reverse)
    {
        // ”š‚ª¬‚³‚¢•û‚ªŸ‚¿
        if (PlayerCard < AICard) return JudgeResult::PlayerWin;
        if (PlayerCard > AICard) return JudgeResult::AIWin;
    }
    else
    {
        // ”š‚ª‘å‚«‚¢•û‚ªŸ‚¿
        if (PlayerCard > AICard) return JudgeResult::PlayerWin;
        if (PlayerCard < AICard) return JudgeResult::AIWin;
    }

    return JudgeResult::Draw;
}