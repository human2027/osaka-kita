#pragma once

//画面サイズ
constexpr float Window_screen_W = 860.0f;
constexpr float Window_screen_H = 720.0f;

//ゲーム

constexpr int MaxRound = 15;//最大ラウンド数
constexpr int HP_decrease = 10;//毎ラウンドごとのHP減少
constexpr int Item_Recovery = 20;//回復アイテムで回復する値
constexpr int Item_plus = 1;//アイテムで進む数がプラスされる奴
constexpr int Player_per_round = 3;

constexpr int TILE_DAMAGE_VALUE = 10;  // ダメージマスで減るHP
constexpr int TILE_HEAL_VALUE = 10;  // 回復マスで増えるHP
//マップ
constexpr int Goal_pos = 50;//ゴール地点

//プレイヤー
constexpr int MAX_Player_HP = 100;
enum class ItemType
{
	Item_none = 0,
	Item_heal = 1,
	Item_Reverse = 2,
	Item_boost = 3,
};

constexpr int nothing_score = -100000;
constexpr int Damage_score = 50;
constexpr int Heal_score = 50;
constexpr int Item_score = 40;
constexpr int MAX_TURN_LIMIT = 5;