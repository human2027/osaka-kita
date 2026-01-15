#pragma once
#include "InputBase.h"
#include "DxLib.h"
//Œ¤‹†
class Pad : public InputBase
{
public:
	int GetSelectedCard() override {
        if (GetJoypadInputState(PAD_INPUT_1)) return 1;
        if (GetJoypadInputState(PAD_INPUT_2)) return 2;
        if (GetJoypadInputState(PAD_INPUT_4)) return 3;
        if (GetJoypadInputState(PAD_INPUT_LEFT)) return 4;
        if (GetJoypadInputState(PAD_INPUT_DOWN)) return 5;
        return 0;
	}



};

