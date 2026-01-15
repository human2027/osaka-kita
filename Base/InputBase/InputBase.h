#pragma once

class InputBase {
public:
    virtual ~InputBase() = default;
    virtual int GetSelectedCard() = 0;
};
