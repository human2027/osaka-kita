#pragma once
#include <string>

class UIBase {
public:
    virtual ~UIBase() = default;

    virtual void Initialize() {}
    virtual void Update() {}
    virtual void Draw() = 0;
    virtual void Finalize() {}
};
