#pragma once
#include <memory>
#include <string>

#include "InitialValue.h"

class Player;
class AIPlayer;
class InputBase;
class Blackboard;

// AI の難易度
enum class AIDifficulty
{
    Easy,
    Normal,
    Hard
};

class CharacterFactory
{
public:
    // プレイヤー生成
    //  ・HP / Pos / 手札初期化
    //  ・入力デバイス(InputBase 派生)をセット
    static std::shared_ptr<Player> CreatePlayer(
        std::shared_ptr<InputBase> inputDevice
    );

    // AI 生成
    //  ・HP / Pos / 手札初期化
    //  ・難易度（今はまだ未使用。将来パラメータに使える）
    //  ・ブラックボード（将来 AI 強化用）
    static std::shared_ptr<AIPlayer> CreateAI(
        AIDifficulty difficulty,
        std::shared_ptr<Blackboard> blackboard = nullptr
    );
};
