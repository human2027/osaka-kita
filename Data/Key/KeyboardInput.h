#pragma once
#include "InputBase.h"
#include "DxLib.h"

class KeyboardInput : public InputBase
{
public:
    KeyboardInput() = default;

    void Update() override
    {
        for (int i = 0; i < 256; ++i)
        {
            oldKeys[i] = nowKeys[i];
        }

        char keyState[256];
        GetHitKeyStateAll(keyState);

        for (int i = 0; i < 256; ++i)
        {
            nowKeys[i] = static_cast<unsigned char>(keyState[i]);
        }

        confirmTriggered = false;

        if (IsTriggered(KEY_INPUT_1)) selectedCard = 1;
        if (IsTriggered(KEY_INPUT_2)) selectedCard = 2;
        if (IsTriggered(KEY_INPUT_3)) selectedCard = 3;
        if (IsTriggered(KEY_INPUT_4)) selectedCard = 4;
        if (IsTriggered(KEY_INPUT_5)) selectedCard = 5;

        if (IsTriggered(KEY_INPUT_RETURN) || IsTriggered(KEY_INPUT_SPACE) || IsTriggered(KEY_INPUT_Z))
        {
            confirmTriggered = true;
        }
    }

    int GetSelectedCard() override
    {
        return selectedCard;
    }

    bool IsConfirmTriggered() override
    {
        return confirmTriggered;
    }

private:
    bool IsTriggered(int key) const
    {
        return nowKeys[key] != 0 && oldKeys[key] == 0;
    }

private:
    int selectedCard = 1;
    bool confirmTriggered = false;

    int nowKeys[256]{};
    int oldKeys[256]{};
};