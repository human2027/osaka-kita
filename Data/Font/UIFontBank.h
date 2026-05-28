#pragma once

class UIFontBank
{
public:
    UIFontBank() = default;
    ~UIFontBank();

    bool Load();
    void Clear();

    int GetStatusTitleFont() const { return statusTitleFont; }
    int GetStatusTextFont() const { return statusTextFont; }

private:
    int statusTitleFont = -1;
    int statusTextFont = -1;
};