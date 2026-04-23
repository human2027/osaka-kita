#pragma once

// Ÿ”s”»’èƒ‚[ƒh
enum class JudgeMode
{
    Normal,   // ‘å‚«‚¢”š‚ªŸ‚¿
    Reverse   // ¬‚³‚¢”š‚ªŸ‚¿
};

// ”»’èŒ‹‰Ê
enum class JudgeResult
{
    PlayerWin,
    AIWin,
    Draw
};

class Judge
{
public:
    JudgeResult JudgeWinner(int playerCard, int aiCard, JudgeMode mode = JudgeMode::Normal) const;
};