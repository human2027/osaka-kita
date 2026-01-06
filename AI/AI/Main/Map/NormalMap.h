#pragma once
#include <string>
#include "MapBase.h"

// -------------------------
//   通常マップ（CSV読み込み）
// -------------------------
class NormalMap : public MapBase
{
public:
    // CSVを指定しない場合はデフォルトのパスを使用
    NormalMap();

    // CSVパスを指定する版
    NormalMap(const std::string& csvPath);

    virtual ~NormalMap() = default;

    // マップ初期化（CSV読み込み）
    virtual void Initialize() override;

private:
    std::string path;   // CSVのパス
};
