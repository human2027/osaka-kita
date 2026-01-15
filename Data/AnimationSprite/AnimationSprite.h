#pragma once
#include <string>
#include <vector>
#include <unordered_map>

class AnimationSprite
{
public:
    struct Sequence
    {
        std::vector<int> handles;

        int GetFrameCount() const
        {
            return static_cast<int>(handles.size());
        }
    };

public:
    AnimationSprite() = default;
    ~AnimationSprite();       // 全ハンドル解放

    //全アニメを登録する
    void InitializeForBoardGame();

    // すでに読み込んでいるか？
    bool HasSequence(const std::string& key) const;

    // アニメ全体の取得（存在しない場合 nullptr）
    const Sequence* GetSequence(const std::string& key) const;

    // 指定フレームハンドル取得（範囲外・未登録なら -1）
    int GetFrame(const std::string& key, int frameIndex) const;

    // 全登録アニメを解放
    void Clear();

private:
    std::unordered_map<std::string, Sequence> sequences;

    //連番画像の読み込み
    bool LoadSequence(const std::string& key,
        const std::string& folderPath,
        const std::string& baseName,
        int startIndex,
        int endIndex,
        const std::string& ext = ".png",
        int zeroPadding = 0);
};
