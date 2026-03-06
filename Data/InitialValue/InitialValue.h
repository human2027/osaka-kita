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

constexpr int Tile_Damage_Value = 10;  // ダメージマスで減るHP
constexpr int Tile_Heal_Value = 10;  // 回復マスで増えるHP
//マップ
constexpr int Goal_pos = 50;//ゴール地点

//UI
constexpr int Count_Position_x = 40;
constexpr int Count_Position_y = 40;



//プレイヤー
constexpr int MAX_Player_HP = 100;
enum class ItemType
{
	Item_none = 0,
	Item_heal = 1,
	Item_Reverse = 2,
	Item_boost = 3,
};

//射程外の場合
constexpr int Nothing_Score = -100000;
//一ラウンドの最大
constexpr int Max_Turn_Limit = 5;

// Intent判定用のHP比率
constexpr float AI_Danger_HP_Ratio = 0.30f; // 危険域
constexpr float AI_CriticlL_HP_Raito = 0.15f; // 致命域（必要なら）

// 相手が瀕死なら回復阻止を狙う比率（同じでOKなら上と共用でも可）
constexpr float AI_Block_Enemy_HP_Raito = 0.30f;

// 引き分けに対する好み（0なら無視）
constexpr int AI_Draw_Penalty = 0;

// 期待値探索の“相手札情報が無い時”の仮定
constexpr int AI_Card_Min = 1;
constexpr int AI_Card_Max = 5;

// Weights: Neutral（通常）
constexpr int AI_Neutral_W_Iteam = 60;
constexpr int AI_Neutral_W_Heal = 90;
constexpr int AI_Neutral_W_Damage_Avoid = 110;
constexpr int AI_Neutral_W_Damage_Enemy = 70;
constexpr int AI_Neutral_W_Block_Heal = 180;

// Weights: GetItem（HP満タン&未所持でアイテム優先）
constexpr int AI_GETITEM_W_ITEM = 140;
constexpr int AI_GETITEM_W_HEAL = 10;
constexpr int AI_GETITEM_W_DAMAGE_AVOID = 110;
constexpr int AI_GETITEM_W_DAMAGE_ENEMY = 50;
constexpr int AI_GETITEM_W_BLOCK_HEAL = 120;

// Weights: HealSelf（低HPで回復優先）
constexpr int AI_Healself_W_Iteam = 40;
constexpr int AI_Healself_W_Heal = 170;
constexpr int AI_Healself_W_Damage_Avoid = 170;
constexpr int AI_Healself_W_Damage_Enemy = 30;
constexpr int AI_Healself_W_Block_Heal = 120;

// Weights: BlockHeal（相手回復阻止）-
constexpr int AI_Blockheal_W_Iteam = 70;
constexpr int AI_Blockheal_W_Heal = 50;
constexpr int AI_Blockheal_W_Damage_Avoid = 70;
constexpr int AI_Blockheal_W_Damage_Enemy = 120;
constexpr int AI_Blockheal_W_Block_Heal = 280;

// HPがこの比率以下なら「小さめ札」を出しやすくする
constexpr float AI_LowHP_Small_Raito = 0.35f; // 例：35%

// HP低い時に小さい札へ寄せる強さ（1～5の偏りに掛ける）
constexpr int AI_Bias_LowHP_Small = 18;

// 位置で負けている時に大きい札へ寄せる強さ
constexpr int AI_Bias_LowHP_Big = 14;

// 何マス差から「負けてる」と見なして強めるか（差をこの値で頭打ち）
constexpr int AI_Behind_Diff_Cap = 10;

// 期待値に加える小ノイズ（毎回同じになりにくくする）
constexpr int AI_Noise_Range = 3;

// Softmaxの温度（大きいほどランダム、小さいほど最善手固定）
// 基本温度（普段）
constexpr float AI_Softmax_Temp_Base = 0.80f;

// 「負けてる（後ろ）」ほど温度を上げてブレやすくする加算
constexpr float AI_Softmax_Temp_Behind_Add = 0.70f;

// 「負けてる」ときに“思い切り”を出す確率（例：8%）
// 例：AIが後ろのとき、一定確率で最大札(5)を切る
constexpr float AI_Behind_Yolo_Prob = 0.08f;

// YOLOを発動する最低差分（これ未満なら発動しない）
constexpr int AI_Behind_Yolo_Min_Diff = 5;

// YOLO時に切る札（最大札を想定）
constexpr int AI_Yolo_Card = AI_Card_Max;