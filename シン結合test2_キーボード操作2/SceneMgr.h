#pragma once
typedef enum {
    eScene_Menu,    //���j���[���
    eScene_Game,    //�Q�[�����
    eScene_Config,  //�ݒ���
} eScene;

//�X�V
void SceneMgr_Update();

//�`��
void SceneMgr_Draw();

// ���� nextScene �ɃV�[����ύX����
void SceneMgr_ChangeScene(eScene nextScene);