#include "DxLib.h"
#include "Config.h"
#include "Game.h"
#include "Menu.h"
#include "SceneMgr.h"

static eScene Scene = eScene_Menu;    //シーン管理変数

//更新
void SceneMgr_Update() {
    switch (Scene) {       //シーンによって処理を分岐
    case eScene_Menu:    //現在の画面がメニューなら
        Menu_Update();   //メニュー画面の更新処理をする
        break;//以下略
    case eScene_Game:
        Game_Update();
        break;
    case eScene_Config:
        Config_Update();
        break;
    }
}

//描画
void SceneMgr_Draw() {
    switch (Scene) {      //シーンによって処理を分岐
    case eScene_Menu:   //現在の画面がメニュー画面なら
        Menu_Draw();    //メニュー画面の描画処理をする
        break;//以下略
    case eScene_Game:
        Game_Draw();
        Game_Cal();
        break;
    case eScene_Config:
        Config_Draw();
        break;
    }
}

// 引数 nextScene にシーンを変更する
void SceneMgr_ChangeScene(eScene NextScene) {
    Scene = NextScene;
}