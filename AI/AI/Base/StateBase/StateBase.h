#pragma once

class GameManager;

class StateBase {
public:
    virtual ~StateBase() = default;

    virtual void Enter(GameManager& gm) {}    // ó‘Ô‚É“ü‚Á‚½uŠÔ
    virtual void Update(GameManager& gm) = 0; // –ˆƒtƒŒ[ƒ€
    virtual void Draw(GameManager& gm) = 0;   // •`‰æ
    virtual void Exit(GameManager& gm) {}     // Ø‚è‘Ö‚¦
};
