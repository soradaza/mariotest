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

    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0 ) {//��ʍX�V & ���b�Z�[�W���� & ��ʏ���

        Keyboard_Update();    //�L�[�{�[�h�̍X�V
        SceneMgr_Update();  //�X�V
        SceneMgr_Draw();    //�`��
		Game_Cal();

    }

    DxLib_End(); // DX���C�u�����I������
    return 0;
}

//int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
//    ChangeWindowMode(TRUE), DxLib_Init(), SetDrawScreen(DX_SCREEN_BACK); //�E�B���h�E���[�h�ύX�Ə������Ɨ���ʐݒ�
//    SetMainWindowText("Table Tennis Training");
//
//    while (ScreenFlip() == 0 && ProcessMessage() == 0 && ClearDrawScreen() == 0) {//��ʍX�V & ���b�Z�[�W���� & ��ʏ���
//
//        Keyboard_Update();    //�L�[�{�[�h�̍X�V
//        SceneMgr_Update();  //�X�V
//        SceneMgr_Draw();    //�`��
//
//    }
//
//    DxLib_End(); // DX���C�u�����I������
//    return 0;
//}