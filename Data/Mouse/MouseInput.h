#pragma once

class MouseInput
{
public:
    void Update();

    int GetX() const { return mouseX; }
    int GetY() const { return mouseY; }

    bool IsLeftPressed() const { return leftPressed; }
    bool IsLeftTriggered() const { return leftTriggered; }

private:
    int mouseX = 0;
    int mouseY = 0;

    bool leftPressed = false;
    bool prevLeftPressed = false;
    bool leftTriggered = false;
};