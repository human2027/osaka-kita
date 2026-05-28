#pragma once

////画面サイズ
constexpr float Window_screen_W = 1980.0f;
constexpr float Window_screen_H = 1080.0f;

////ゲーム
// 最大ラウンド数
constexpr int MaxRound = 15;   

// 毎ラウンドごとのHP減少
constexpr int HP_decrease = 10;

// 回復アイテムで回復する値
constexpr int Item_Recovery = 20;

// アイテムで進む数がプラスされる
constexpr int Item_plus = 1;

//基本のラウンド数
constexpr int Player_per_round = 3;

// ダメージマスで減るHP
constexpr int Tile_Damage_Value = 10;

// 回復マスで増えるHP
constexpr int Tile_Heal_Value = 10;      

 // ゴール地点
constexpr int Goal_pos = 50;             

////UI-------------------------------------------------
constexpr int Count_Position_x = 100;
constexpr int Count_Position_y = 40;

// UIメッセージ：メイン表示
constexpr int MainMessage_Y = 430;
constexpr int MainMessage_FontSize = 48;
constexpr int MainMessage_FontThickness = 5;

// UIメッセージ：通常表示
constexpr int SubMessage_X = 550;
constexpr int SubMessage_Y = 932;
constexpr int SubMessage_FontSize = 28;
constexpr int SubMessage_FontThickness = 3;
constexpr int SubMessage_LineHeight = 38;

// UIメッセージ：通常表示の背景
constexpr int SubMessage_Box_X = 520;
constexpr int SubMessage_Box_Y = 910;
constexpr int SubMessage_Box_W = 900;
constexpr int SubMessage_Box_H = 80;

// UIメッセージ：影のずらし量
constexpr int Message_Shadow_Offset = 3;
// UIメッセージ：表示時間
constexpr int Message_Dur_Short = 210;
constexpr int Message_Dur_Middle = 220;
constexpr int Message_Dur_Long = 330;

// UIメッセージ：メイン表示時間
constexpr int MainMessage_Dur_Short = 150;
constexpr int MainMessage_Dur_Middle = 200;
constexpr int MainMessage_Dur_Long = 320;
//-----------------------------------------
//プレイヤーとAI
constexpr int MAX_Player_HP = 100;

//射程外の場合
constexpr int Nothing_Score = -100000;

//一ラウンドの最大
constexpr int Max_Turn_Limit = 5;

//Intent判定用のHP比率
constexpr float AI_Danger_HP_Ratio = 0.30f;      // 危険域
constexpr float AI_Critical_HP_Ratio = 0.15f;    // 致命域

//相手が瀕死なら回復阻止を狙う比率
constexpr float AI_Block_Enemy_HP_Ratio = 0.30f;

//引き分けに対する好み（0なら無視）
constexpr int AI_Draw_Penalty = 0;

//期待値探索の“相手札情報が無い時”の仮定
constexpr int AI_Card_Min = 1;
constexpr int AI_Card_Max = 5;

//Weights: Neutral（通常）
constexpr int AI_Neutral_W_Item = 0;
constexpr int AI_Neutral_W_Item_Heal = 0;
constexpr int AI_Neutral_W_Item_Boost = 30;
constexpr int AI_Neutral_W_Item_Reverse = 25;

constexpr int AI_Neutral_W_Heal = 90;
constexpr int AI_Neutral_W_Damage_Avoid = 110;
constexpr int AI_Neutral_W_Damage_Enemy = 70;
constexpr int AI_Neutral_W_Block_Heal = 180;

//Weights: GetItem（HP満タン&未所持でアイテム優先）
constexpr int AI_GetItem_W_Item = 140;
constexpr int AI_GetItem_W_Heal = 10;
constexpr int AI_GetItem_W_Damage_Avoid = 110;
constexpr int AI_GetItem_W_Damage_Enemy = 50;
constexpr int AI_GetItem_W_Block_Heal = 120;

//Weights: HealSelf（低HPで回復優先）
constexpr int AI_HealSelf_W_Item = 40;
constexpr int AI_HealSelf_W_Heal = 170;
constexpr int AI_HealSelf_W_Damage_Avoid = 170;
constexpr int AI_HealSelf_W_Damage_Enemy = 30;
constexpr int AI_HealSelf_W_Block_Heal = 120;

//Weights: BlockHeal（相手回復阻止）
constexpr int AI_BlockHeal_W_Item = 70;
constexpr int AI_BlockHeal_W_Heal = 50;
constexpr int AI_BlockHeal_W_Damage_Avoid = 70;
constexpr int AI_BlockHeal_W_Damage_Enemy = 120;
constexpr int AI_BlockHeal_W_Block_Heal = 280;

//HPがこの比率以下なら「小さめ札」を出しやすくする
constexpr float AI_Low_HP_Small_Ratio = 0.35f;

//HP低い時に小さい札へ寄せる強さ
constexpr int AI_Bias_Low_HP_Small = 18;

//位置で負けている時に大きい札へ寄せる強さ
constexpr int AI_Bias_Behind_Big = 14;

//何マス差から負けてると見なして強めるか
constexpr int AI_Behind_Diff_Cap = 10;

//期待値に加える小ノイズ
constexpr int AI_Noise_Range = 3;

//Softmaxの温度
constexpr float AI_Softmax_Temp_Base = 0.80f;

//負けてるほど温度を上げてブレやすくする加算
constexpr float AI_Softmax_Temp_Behind_Add = 0.70f;

//負けてるときに思い切りを出す確率
constexpr float AI_Behind_Yolo_Prob = 0.08f;

//YOLOを発動する最低差分
constexpr int AI_Behind_Yolo_Min_Diff = 5;

//YOLO時に切る札
constexpr int AI_Yolo_Card = AI_Card_Max;

////描画位置
constexpr int AI_Draw_X = 420;
constexpr int AI_Draw_Y = 150;
constexpr int Player_Draw_X = 200;
constexpr int Player_Draw_Y = 150;

//マップキャラの位置
constexpr int MapChar_drawOffsetX = 28;
constexpr int MapChar_drawOffsetY = 28;

/// ターン結果メッセージを見せる待機時間
constexpr int Turn_Wait_After_Result = 120;