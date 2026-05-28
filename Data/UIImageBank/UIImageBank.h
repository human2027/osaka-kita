#pragma once

class UIImageBank
{
public:
    UIImageBank() = default;
    ~UIImageBank();

    bool Load();
    void Clear();

    int GetCardHandle(int card) const;

    int GetConfirmButtonHandle() const { return confirmButtonHandle; }
    int GetUseItemButtonHandle(bool enabled) const;

    int GetStatusPanelHandle() const { return statusPanelHandle; }
    int GetHandPanelHandle() const { return handPanelHandle; }

    // タイトル画面
    int GetTitleHandle() const { return titleHandle; }

    int GetRightHandle() const { return rightHandle; }
private:
    int cardHandles[6] = { -1, -1, -1, -1, -1, -1 };

    int confirmButtonHandle = -1;
    int useItemButtonHandle = -1;
    int useItemButtonDisabledHandle = -1;

    int statusPanelHandle = -1;
    int handPanelHandle = -1;

    // タイトル画面
    int titleHandle = -1;

    int rightHandle = -1;
};