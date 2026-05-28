#pragma once

#include <string>
#include <vector>

#include "ItemType.h"
#include "UIEvent.h"

struct UIMessage
{
    std::string text;
    int timer = 0;
};

class UIMessageManager
{
public:
    void AddMessage(const std::string& text, int duration);
    void Draw(int x, int y, int lineHeight);
    void ClearMessages();
    void PushSystemMessage(const std::string& text, int duration);
    void PushEvent(const UIEvent& e);

    void ShowRoundStart(int round);
    void ShowPlayerGoalWin();
    void ShowAIGoalWin();
    void ShowPlayerHPZeroLose();
    void ShowAIHPZeroLose();
    void ShowItemPickup(bool isPlayer, ItemType item);
    void ShowItemUse(bool isPlayer, ItemType item);

private:
    std::string GetItemName(ItemType item) const;

    void AddMainMessage(const std::string& text, int duration);
    void DrawMainMessage() const;

private:
    std::vector<UIMessage> messages;

    UIMessage mainMessage;
    bool hasMainMessage = false;
};