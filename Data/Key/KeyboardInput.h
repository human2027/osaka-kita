#pragma once
#include "InputBase.h"
#include "DxLib.h"
//Œ¤‹†
class KeyboardInput : public InputBase {
public:
    int GetSelectedCard() override {
        if (CheckHitKey(KEY_INPUT_1)) return 1;
        if (CheckHitKey(KEY_INPUT_2)) return 2;
        if (CheckHitKey(KEY_INPUT_3)) return 3;
        if (CheckHitKey(KEY_INPUT_4)) return 4;
        if (CheckHitKey(KEY_INPUT_5)) return 5;
        return 0;
    }
};
