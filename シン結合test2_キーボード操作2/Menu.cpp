#include "Menu.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Keyboard.h"

const static int GAME_Y = 240;    //「ゲーム」文字のy位置
const static int CONFIG_Y = 270;    //「設定」文字のy位置

typedef enum {
    eMenu_Game,        //ゲーム
    eMenu_Config,    //設定

    eMenu_Num,        //本項目の数
} eMenu;

static int NowSelect = eMenu_Game;    //現在の選択状態(初期はゲーム選択中)

//更新
void Menu_Update() {
    if (Keyboard_Get(KEY_INPUT_DOWN) == 1) {//下キーが押されていたら
        NowSelect = (NowSelect + 1) % eMenu_Num;//選択状態を一つ下げる
    }
    if (Keyboard_Get(KEY_INPUT_UP) == 1) {//上キーが押されていたら
        NowSelect = (NowSelect + (eMenu_Num - 1)) % eMenu_Num;//選択状態を一つ上げる
    }
    if (Keyboard_Get(KEY_INPUT_RETURN) == 1) {//エンターキーが押されたら
        switch (NowSelect) {//現在選択中の状態によって処理を分岐
        case eMenu_Game://ゲーム選択中なら
            SceneMgr_ChangeScene(eScene_Game);//シーンをゲーム画面に変更
            break;
        case eMenu_Config://設定選択中なら
            SceneMgr_ChangeScene(eScene_Config);//シーンを設定画面に変更
            break;
        }
    }
}

//描画
void Menu_Draw() {
    DrawString(200, 150, "メニュー画面です。", GetColor(230, 0, 51));
    DrawString(200, 170, "上下キーを押し、エンターを押して下さい。", GetColor(230, 0, 51));
    DrawString(280, GAME_Y, "ゲーム", GetColor(230, 0, 51));
    DrawString(280, CONFIG_Y, "設定", GetColor(230, 0, 51));
    int y = 0;
    switch (NowSelect) {//現在の選択状態に従って処理を分岐
    case eMenu_Game://ゲーム選択中なら
        y = GAME_Y;    //ゲームの座標を格納
        break;
    case eMenu_Config://設定選択中なら
        y = CONFIG_Y;    //設定の座標を格納
        break;
    }
    DrawString(250, y, "■", GetColor(230, 0, 51));
}