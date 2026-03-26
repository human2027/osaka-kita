#pragma once

class InputBase {
public:
    virtual ~InputBase() = default;

    virtual void Update() = 0;
    virtual int GetSelectedCard() = 0;
    virtual bool IsConfirmTriggered() = 0;
};