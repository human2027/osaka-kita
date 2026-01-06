#include "DxLib.h"
#include "GameManager.h"

//------------------------------------
// WinMain（DxLib の標準）
//------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // DxLib 初期化
    SetOutApplicationLogValidFlag(FALSE);   // ログを出さない
    ChangeWindowMode(TRUE);                 // ウィンドウモード
    SetBackgroundColor(0, 0, 0);            // 背景色
    SetGraphMode(860, 720, 32);             // 画面サイズ
    if (DxLib_Init() == -1) return -1;

    // 画面を真っ黒にしないモード
    SetDrawScreen(DX_SCREEN_BACK);

    //------------------------------------
    // GameManager を用意
    //------------------------------------
    GameManager game;
    game.Initialize();

    //------------------------------------
    // メインループ
    //------------------------------------
    while (ProcessMessage() == 0)
    {
        ClearDrawScreen();   // 画面クリア

        game.Update();       // ロジック更新
        game.Draw();         // 描画

        ScreenFlip();        // 表示
    }

    //------------------------------------
    // 終了処理
    //------------------------------------
    DxLib_End();
    return 0;
}
