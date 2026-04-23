#include "DxLib.h"
#include "GameManager.h"
#include "InitialValue.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // DxLib 初期化
    if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
    {
        return -1;			// エラーが起きたら直ちに終了
    }
    ChangeWindowMode(TRUE);                 // ウィンドウモード
    SetBackgroundColor(0, 0, 0);            // 背景色
    SetGraphMode(Window_screen_W, Window_screen_H, 32);             // 画面サイズ
    if (DxLib_Init() == -1) return -1;

    // 画面を真っ黒にしないモード
    SetDrawScreen(DX_SCREEN_BACK);

    GameManager game;
    game.Initialize();

    // メインループ
    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();   // 画面クリア

        game.Update();       // ロジック更新
        game.Draw();         // 描画

        ScreenFlip();        // 表示
    }

    // 終了処理
    DxLib_End();
    return 0;
}
