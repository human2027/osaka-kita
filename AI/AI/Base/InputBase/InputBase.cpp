#pragma once

class InputBase {
public:
    virtual ~InputBase() {}
    virtual int GetSelectedCard() = 0;
};
