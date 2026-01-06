#include "AnimationSprite.h"
#include <DxLib.h>
#include <sstream>
#include <iomanip>

AnimationSprite::~AnimationSprite()
{
    Clear();
}

//ゲーム全体で使うアニメをここで全部読み込む(予定)
void AnimationSprite::InitializeForBoardGame()
{
    // 試し読み込み
    LoadSequence(
        "Player_PutHand",
        "AI/AI/Material/Animation/Put hand",
        "PutHand_",
        0, 3,
        ".png",
        0
    );

    // 必要になったら追加
}

// 内部専用：連番画像の読み込み
bool AnimationSprite::LoadSequence(
    const std::string& key,
    const std::string& folderPath,
    const std::string& baseName,
    int startIndex,
    int endIndex,
    const std::string& ext,
    int zeroPadding)
{
    // 既存キーは解放して作り直す
    if (HasSequence(key))
    {
        auto it = sequences.find(key);
        if (it != sequences.end())
        {
            for (int h : it->second.handles)
            {
                if (h != -1) DeleteGraph(h);
            }
            sequences.erase(it);
        }
    }

    if (startIndex > endIndex)
        std::swap(startIndex, endIndex);

    Sequence seq;
    seq.handles.reserve(endIndex - startIndex + 1);

    for (int i = startIndex; i <= endIndex; i++)
    {
        std::stringstream ss;

        // フォルダパス
        ss << folderPath;
        if (!folderPath.empty() &&
            folderPath.back() != '/' &&
            folderPath.back() != '\\')
        {
            ss << '/';
        }

        // baseName
        ss << baseName;

        // 0埋め
        if (zeroPadding > 0)
        {
            ss << std::setw(zeroPadding) << std::setfill('0') << i;
        }
        else
        {
            ss << i;
        }

        ss << ext;

        std::string filePath = ss.str();
        int handle = LoadGraph(filePath.c_str());

        if (handle == -1)
        {
            // 失敗 → 今まで読んだ分を全部解放
            for (int h : seq.handles)
            {
                if (h != -1) DeleteGraph(h);
            }
            return false;
        }

        seq.handles.push_back(handle);
    }

    sequences[key] = std::move(seq);
    return true;
}

bool AnimationSprite::HasSequence(const std::string& key) const
{
    return sequences.find(key) != sequences.end();
}

const AnimationSprite::Sequence*
AnimationSprite::GetSequence(const std::string& key) const
{
    auto it = sequences.find(key);
    if (it == sequences.end()) return nullptr;
    return &it->second;
}

int AnimationSprite::GetFrame(const std::string& key, int frameIndex) const
{
    auto it = sequences.find(key);
    if (it == sequences.end()) return -1;

    const Sequence& seq = it->second;

    if (seq.handles.empty()) return -1;
    if (frameIndex < 0 || frameIndex >= (int)seq.handles.size()) return -1;

    return seq.handles[frameIndex];
}

// -------------------------------------------------------------------
void AnimationSprite::Clear()
{
    for (auto& [key, seq] : sequences)
    {
        for (int h : seq.handles)
        {
            if (h != -1) DeleteGraph(h);
        }
    }
    sequences.clear();
}
