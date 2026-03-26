#include "MouseInput.h"
#include <DxLib.h>

void MouseInput::Update()
{
    GetMousePoint(&mouseX, &mouseY);

    prevLeftPressed = leftPressed;
    leftPressed = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

    leftTriggered = leftPressed && !prevLeftPressed;
}