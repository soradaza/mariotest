#include "DxLib.h"
#include "SceneMgr.h"
#include "Keyboard.h"
#include "Game.h"

#define WINDOW_WIDTH (1600)
#define WINDOW_HEIGHT (900)

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstnce,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd)

{
	ChangeWindowMode(TRUE);
	DxLib_Init();


	SetWindowInitPosition(0, 0);
	SetGraphMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32);
	SetDrawScreen(DX_SCREEN_BACK);

	Game_Ini();

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 ) {//画面更新 & メッセージ処理 & 画面消去

        Keyboard_Update();    //キーボードの更新
        SceneMgr_Update();  //更新
        SceneMgr_Draw();    //描画
		Game_Cal();

    }

    DxLib_End(); // DXライブラリ終了処理
    return 0;
}

//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//    ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //ウィンドウモード変更と初期化と裏画面設定
//    SetMainWindowText("Table Tennis Training");
//
//    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {//画面更新 & メッセージ処理 & 画面消去
//
//        Keyboard_Update();    //キーボードの更新
//        SceneMgr_Update();  //更新
//        SceneMgr_Draw();    //描画
//
//    }
//
//    DxLib_End(); // DXライブラリ終了処理
//    return 0;
//}