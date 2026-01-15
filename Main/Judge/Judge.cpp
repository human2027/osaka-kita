#include "Judge.h"

int Judge::JudgeWinner(int PlayerCard, int AICard, bool Reverse)
{
    if (Reverse)
    {
        //”š‚ª¬‚³‚¢•û‚ªŸ‚¿
        if (PlayerCard < AICard) return 1;   // Player ‚ÌŸ‚¿
        if (PlayerCard > AICard) return -1;  // AI ‚ÌŸ‚¿
    }
    else
    {
        //”š‚ª‘å‚«‚¢•û‚ªŸ‚¿
        if (PlayerCard > AICard) return 1;   // Player ‚ÌŸ‚¿
        if (PlayerCard < AICard) return -1;  // AI ‚ÌŸ‚¿
    }

    return 0; // ˆø‚«•ª‚¯
}
