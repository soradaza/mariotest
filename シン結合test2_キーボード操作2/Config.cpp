#include "Config.h"
#include "SceneMgr.h"
#include "DxLib.h"

//更新
void Config_Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) {//Escキーが押されていたら
        SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
    }
}

//描画
void Config_Draw() {
    DrawString(0, 0, "設定画面です。", GetColor(255, 255, 255));
    DrawString(0, 20, "Escキーを押すとメニュー画面に戻ります。", GetColor(255, 255, 255));
}