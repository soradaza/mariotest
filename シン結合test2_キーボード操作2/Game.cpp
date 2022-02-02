#include "Game.h"
#include "SceneMgr.h"
#include "DxLib.h"
#include "Keyboard.h"

#define WINDOW_WIDTH (1600)
#define WINDOW_HEIGHT (900)

enum e_But
{
    But_Dow,
    But_Up,
    But_Rig,
    But_Total,
};

int Pad_Inp;

int Keyboard[e_But::But_Total];

bool Pad_Read()
{
    char tmpKey[256];
    Pad_Inp =GetHitKeyStateAll(tmpKey);

    for (int b = 0; b < e_But::But_Total; b = b + 1)
    {
        if (Pad_Inp & (1 << b))
        {
            Keyboard[b] = Keyboard[b] + 1;
        }
        else Keyboard[b] = 0;
    }

    return FALSE;
}

int Camera_x;

enum e_Type
{
    e_��,
    e_�n��,
    e_�G,
    e_�S�[��,
    e_��,
};

struct Pic
{
    int p;
    int w;
    int t;
};

enum e_Ani
{
    e_Normal,
    e_Walk1,
    e_Walk2,
    e_Jump,
    e_End,
    e_Ani_Total,

};

struct Ract
{
    int x;
    int y;
    int w;
    int t;
    int k;
    double r;
    Pic p[e_Ani::e_Ani_Total];
    int a;
    bool d;
    int j;
    bool c;
};

struct Text
{
    int k;
    int x;
    int y;
};

Text** Stage;

int STAGE_TOTAL;
int STAGE_WIDTH;
int STAGE_HEIGHT;

struct Color
{
    int r;
    int g;
    int b;
};

Color Col;

Ract* Block;

Ract* Boll;

//int  Handle = LoadGraph("aktama.png"); // �摜��`�悷��

//�W�����v���̗񋓑̒�`
enum e_Jump
{
    ���n,
    �W�����v,
    ����,
};

int Jump;

int Jump_Ymem;

//�u���b�N������
void Block_Ini(Ract* b)
{
    b->x = 0;
    b->y = 0;
    b->w = 0;
    b->t = e_Type::e_��;
    b->k = 0;
    b->r = 0.;

    for (int i = 0; i < e_Ani::e_Ani_Total; i = i + 1)
    {
        b->p[i].p = 0;
        b->p[i].w = 0;
        b->p[i].t = 0;
    }
    b->a = e_Ani::e_Normal;
    b->d = FALSE;
    b->j = 0;
    b->c = FALSE;
}

//�Q�[���J�n���̏���
void Game_Ini()
{
    char name[] = { "1-2.txt" };
    FILE* fp = NULL;
    errno_t error = fopen_s(&fp, name, "r");

    if (error == NULL &&
        fp != NULL)
    {
        fscanf_s(fp, "%d\n", &Col.r);
        fscanf_s(fp, "%d\n", &Col.g);
        fscanf_s(fp, "%d\n", &Col.b);
        SetBackgroundColor(Col.r, Col.g, Col.b);

        fscanf_s(fp, "%d\n", &STAGE_TOTAL);
        fscanf_s(fp, "%d\n", &STAGE_WIDTH);
        fscanf_s(fp, "%d\n", &STAGE_HEIGHT);

        Stage = new Text * [STAGE_WIDTH];
        for (int i = 0; i < STAGE_WIDTH; i = i + 1)
        {
            Stage[i] = new Text[STAGE_HEIGHT];
        }
        for (int y = 0; y < STAGE_HEIGHT; y = y + 1)
        {
            for (int x = 0; x < STAGE_WIDTH; x = x + 1)
            {
                fscanf_s(fp, "%d", &Stage[x][y].k);
                Stage[x][y].x = x;
                Stage[x][y].y = y;
            }
        }
        //�t�@�C������
        fclose(fp);
    }
    //�������m��
    Block = new Ract[STAGE_TOTAL];

    //Text�^����Ract�^�ɕϊ����Atxt�̏���Block[]
    unsigned int i = 0;
    for (int y = 0; y < STAGE_HEIGHT; y = y + 1)
    {
        for (int x = 0; x < STAGE_WIDTH; x = x + 1)
        {
            if (Stage[x][y].k == e_Type::e_��)//�u���b�N�Ȃ��̏ꍇ
            {
                //�u���b�N������
                Block_Ini(&Block[i]);
            }
            else//�u���b�N����̏ꍇ
            {
                //�u���b�N�̕��ƌ��݂��u���b�N�̎�ނ��画�f
                switch (Stage[x][y].k)
                {
                case e_Type::e_�n��:
                    Block[i].p[0].p =
                        LoadGraph("block2.jpg");

                    Block[i].d = FALSE;

                    Block[i].c = FALSE;

                    break;

                case e_Type::e_�G:
                    Block[i].p[0].p =
                        LoadGraph("enemy.png");

                    Block[i].d = FALSE;

                    Block[i].c = FALSE;

                    break;

                case e_Type::e_��:
                    Block[i].p[e_Ani::e_Normal].p =
                        LoadGraph("block2.jpg");
                    Block[i].p[e_Ani::e_Walk1].p =
                        LoadGraph("block2.jpg");
                    Block[i].p[e_Ani::e_Walk2].p =
                        LoadGraph("block2.jpg");
                    Block[i].p[e_Ani::e_Jump].p =
                        LoadGraph("block2.jpg");
                    Block[i].p[e_Ani::e_End].p =
                        LoadGraph("block2.jpg");

                    Block[i].d = FALSE;

                    Block[i].c = TRUE;

                    break;
                }

                //�u���b�N�ʐ^������݂ƕ��擾
                GetGraphSize(
                    Block[i].p[0].p,
                    &Block[i].p[0].w,
                    &Block[i].p[0].t);

                //�u���b�N�̕��ƌ��݂���
                Block[i].w = Block[i].p[0].w;
                Block[i].t = Block[i].p[0].t;

                //�u���b�N�̍��W�E��ށE��]�p�x�E�W�����v�J�E���^�[
                Block[i].x = Stage[x][y].x * 50;
                Block[i].y = Stage[x][y].y * 50;
                Block[i].k = Stage[x][y].k;
                Block[i].r = 0;
                Block[i].j = 0;
                Block[i].a = e_Ani::e_Normal;
            }

            //���[�v�J�E���^
            i = i + 1;
        }
    }
    //�������̉��(Text Stage)
    for (int i = 0; i < STAGE_WIDTH; i = i + 1)
    {
        delete[] Stage[i];
    }
    delete[] Stage;

    //������
    Camera_x = 0;
}





//�X�V
void Game_Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { //Esc�L�[��������Ă�����
        SceneMgr_ChangeScene(eScene_Menu);//�V�[�������j���[�ɕύX
    }
       
    
}

void Game_End(){}

#define Move_X (5)
#define Move_Max_Y (300)



//--------------------------------
//�T�@�v:�ڐG������s���֐�
//���@��:�ʁA�ʂ̈��q�A�ڐG���ɓ|�����ۂ�
//�߂�l:�ڐG���茋��
//--------------------------------
bool HitJudg(Ract c, int i_c, bool d)
{
    for (int i = 0; i < STAGE_TOTAL; i = i + 1)
    {
          if (i != i_c) //�������̓��m�̔�r������邽��
          {
                if (((Block[i].x <= c.x &&
                    c.x < Block[i].x + Block[i].w) ||

                    (Block[i].x <= c.x + c.w &&
                        c.x + c.w < Block[i].x + Block[i].w)) &&

                    ((Block[i].y <= c.y &&
                        c.y < Block[i].y + Block[i].t) ||

                           (Block[i].y <= c.y + c.t &&
                              c.y + c.t < Block[i].y + Block[i].t)))
                    {
                        //�ʂ���Q���ƐڐG������
                        if ((c.k == e_Type::e_�� ||
                            Block[i].k == e_Type::e_��) &&
                            c.c == TRUE &&
                            Block[i].c == TRUE)
                            {
                            //�ڐG���ɓ|����
                                switch(d)
                                {
                                case FALSE:

                                    Game_End();

                                    break;

                                case TRUE:

                                    Block_Ini(&Block[i]);

                                    break;

                                }
                            }

                        //�S�[���ɐڐG
                        else if (c.k == e_Type::e_�� &&
                            Block[i].k == e_Type::e_�S�[��)
                        {
                            Game_End();
                        }

                        return TRUE; //�ڐG
                    }
                }
            }

    return FALSE;
        }

//�Q�[���v�Z�������s���֐�
void Game_Cal()
{
    int y_add = 0;

    const int a = 25;

    //�ʂ̈���
    int c_i = 0;

    int m_x = 0;
    for (int i = 0; i < STAGE_TOTAL; i = i + 1)
    {
        if (Block[i].k == e_Type::e_��)
        {
            //�ʃR�s�[
            Boll = &Block[i];

            //�ʍ��W�R�s�[
            m_x = Block[i].x;

            //�ʂ̈���
            c_i = i;

            //���[�v����
            break;
        }
    }

    if (CheckHitKey(KEY_INPUT_RIGHT) != 0) { // �E�L�[��������Ă�����
    //if (Keyboard[e_But::But_Rig] != 0) { // �E�L�[��������Ă�����

        Boll->d = FALSE;
        
        for (int i = 0; i < Move_X; i = i + 1)
        {
            //��x���W�v�Z
            Boll->x = Boll->x + 1;

            if (Boll->x > WINDOW_WIDTH / 2 &&
                Boll->x < STAGE_WIDTH * 50 - WINDOW_WIDTH / 2)
            {
                Camera_x = Camera_x - 1;
            }

            //x�����̋ʂƏ�Q���̐ڐG����
            if (Boll->x > STAGE_WIDTH * 50 - Boll->w ||
                HitJudg(*Boll, c_i, FALSE) == TRUE)
            {
                //��x���W�v�Z
                Boll->x = Boll->x - 1;

                //�J����x���W�v�Z
                if (Boll->x > WINDOW_WIDTH / 2 &&
                    Boll->x < STAGE_WIDTH * 50 - WINDOW_WIDTH / 2)
                {
                    Camera_x = Camera_x + 1;
                }

                //���[�v����
                break;
            }
        }
    }

    if (CheckHitKey(KEY_INPUT_LEFT) != 0) { // ���L�[��������Ă�����

        Boll->d = FALSE;

        for (int i = 0; i < Move_X; i = i + 1)
        {
            Boll->x = Boll->x - 1;
            if (Boll->x > WINDOW_WIDTH / 2 &&
                Boll->x < STAGE_WIDTH * 50 - WINDOW_WIDTH / 2)
            {
                Camera_x = Camera_x + 1;
            }

            //x�����̋ʂƏ�Q���̐ڐG����
            if (Boll->x < 0 ||
                HitJudg(*Boll, c_i, FALSE) == TRUE)
            {
                //�ʍ��W�v�Z
                Boll->x = Boll->x + 1;

                //�J����x���W�v�Z
                if (Boll->x > WINDOW_WIDTH / 2 &&
                    Boll->x < STAGE_WIDTH * 50 - WINDOW_WIDTH / 2)
                {
                    Camera_x = Camera_x - 1;
                }

                //���[�v����
                break;
            }
        }
    }
    //�ʃW�����v
    if (CheckHitKey(KEY_INPUT_UP) != 0 &&
        Jump == e_Jump::���n)
    {
        Jump = e_Jump::�W�����v;

        Jump_Ymem = Boll->y;
    }

    if (Jump == e_Jump::�W�����v)
    {
        Boll->j = Boll->j + 1;
        //�ړ��ʌv�Z
        y_add = (Boll->j * Boll->j) / a;

        for (int i = 0; i < y_add; i = i + 1)
        {
            //�ʂ�y���W�v�Z
            Boll->y = Boll->y - 1;

            //y�����̋ʂƃu���b�N�̐ڐG����
            if (Boll->y < 0 ||
                HitJudg(*Boll, c_i, FALSE) == FALSE ||
                Move_Max_Y == Jump_Ymem - Boll->y)
            {
                //�W�����v�t���O�ύX
                Jump = e_Jump::����;

                //��y���W�v�Z
                Boll->y = Boll->y + 1;

                //�W�����v�J�E���^������
                Boll->j = 0;

                //���[�v����
                break;
            }

        }
    }

    //�ʎ��R����
    if (Jump == e_Jump::���� ||
        Jump == e_Jump::���n)
    {
        //�W�����v�J�E���^
        Boll->j = Boll->j + 1;

        //��y�����ړ��ʌv�Z
        y_add = (Boll->j * Boll->j) / a;

        for (int i = 0; i < y_add; i = i + 1)
        {
            //�{�[��y���W�v�Z
            Boll->y = Boll->y + 1;

            //y�����̋ʂƃu���b�N�̐ڐG����
            if (Boll->y < WINDOW_HEIGHT ||
                HitJudg(*Boll, c_i, TRUE) == TRUE)
            {
                //�W�����v�t���O�ύX
                Jump = e_Jump::���n;

                //��y���W�v�Z
                Boll->y = Boll->y - 1;

                //�W�����v�J�E���^������
                Boll->j = 0;

                //���[�v����
                break;
            }
        }
    }

}

//�`��
void Game_Draw() {

    //�u���b�N�`��
    for (int i = 0; i < STAGE_TOTAL; i = i + 1)
    {
        switch (Block[i].d)
        {
        case FALSE:
            DrawTurnGraph(
                Block[i].x + Camera_x,
                Block[i].y,
                Block[i].p[0].p,
                TRUE);

            break;

        case TRUE:
            DrawTurnGraph(
                Block[i].x + Camera_x,
                Block[i].y,
                Block[i].p[0].p,
                TRUE);

            break;
        }
    }

    //struct Cir
    //{
    //    int x;
    //    int y;
    //    int r;
    //};

    //struct Speed
    //{
    //    int x;
    //    int y;
    //};

    //Speed Ball_Speed;
    //Ball_Speed = { 0,-10 };

    //int x = 0;
    //int y = 0;

    ////PlaySound("02.mp3", DX_PLAYTYPE_BACK);

    //while (1) {//�J��Ԃ���

    //    if (ProcessMessage() != 0) { // ���b�Z�[�W����
    //        break;//�E�B���h�E�́~�{�^���������ꂽ�烋�[�v�𔲂���
    //    }
    //    // DrawRotaGraph(x, y, 1.0, 0.0,Handle, TRUE); // x,y �̈ʒu�ɃL������`��

    //     //�{�[�����V��ɐڐG����
    //    if (y < 10)
    //    {
    //        //�{�[���̑��x�v�Z
    //        Ball_Speed.y = Ball_Speed.y * (-1);
    //    }
    //    if (y > 400)
    //    {
    //        //�{�[���̑��x�v�Z
    //        Ball_Speed.y = Ball_Speed.y * (-1);
    //    }
    //    //if (CheckHitKey(KEY_INPUT_RIGHT) >= 1) { // �E�L�[��������Ă�����
    //    //    x = x + 2;  // �E�ֈړ�
    //    //}
    //    //if (CheckHitKey(KEY_INPUT_DOWN) == 1) { // ���L�[��������Ă�����
    //    //y = y + 2; // ���ֈړ�
    //    //}
    //    //if (CheckHitKey(KEY_INPUT_LEFT) == 1) { // ���L�[��������Ă�����
    //    //    x = x - 2;  // ���ֈړ�
    //    //}
    //    //if (CheckHitKey(KEY_INPUT_UP) == 1) { // ��L�[��������Ă�����
    //    //y = y - 2;    // ��ֈړ�
    //    //}
    //    DrawGraph(x, y, e_Type::e_��, TRUE); //�摜�̕`��
    //    //x = x + Ball_Speed.x; // x��2���₷
    //    y = y + Ball_Speed.y; // x��2���₷
    //    ScreenFlip(); // 7[ms]�ҋ@


            
        
       
       

      
    //}
 
}

