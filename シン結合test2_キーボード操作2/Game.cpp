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
    e_空,
    e_地面,
    e_敵,
    e_ゴール,
    e_玉,
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

//int  Handle = LoadGraph("aktama.png"); // 画像を描画する

//ジャンプ時の列挙体定義
enum e_Jump
{
    着地,
    ジャンプ,
    落下,
};

int Jump;

int Jump_Ymem;

//ブロック初期化
void Block_Ini(Ract* b)
{
    b->x = 0;
    b->y = 0;
    b->w = 0;
    b->t = e_Type::e_玉;
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

//ゲーム開始時の処理
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
        //ファイル閉じる
        fclose(fp);
    }
    //メモリ確保
    Block = new Ract[STAGE_TOTAL];

    //Text型からRact型に変換し、txtの情報をBlock[]
    unsigned int i = 0;
    for (int y = 0; y < STAGE_HEIGHT; y = y + 1)
    {
        for (int x = 0; x < STAGE_WIDTH; x = x + 1)
        {
            if (Stage[x][y].k == e_Type::e_空)//ブロックなしの場合
            {
                //ブロック初期化
                Block_Ini(&Block[i]);
            }
            else//ブロックありの場合
            {
                //ブロックの幅と厚みをブロックの種類から判断
                switch (Stage[x][y].k)
                {
                case e_Type::e_地面:
                    Block[i].p[0].p =
                        LoadGraph("block2.jpg");

                    Block[i].d = FALSE;

                    Block[i].c = FALSE;

                    break;

                case e_Type::e_敵:
                    Block[i].p[0].p =
                        LoadGraph("enemy.png");

                    Block[i].d = FALSE;

                    Block[i].c = FALSE;

                    break;

                case e_Type::e_玉:
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

                //ブロック写真から厚みと幅取得
                GetGraphSize(
                    Block[i].p[0].p,
                    &Block[i].p[0].w,
                    &Block[i].p[0].t);

                //ブロックの幅と厚みを代入
                Block[i].w = Block[i].p[0].w;
                Block[i].t = Block[i].p[0].t;

                //ブロックの座標・種類・回転角度・ジャンプカウンター
                Block[i].x = Stage[x][y].x * 50;
                Block[i].y = Stage[x][y].y * 50;
                Block[i].k = Stage[x][y].k;
                Block[i].r = 0;
                Block[i].j = 0;
                Block[i].a = e_Ani::e_Normal;
            }

            //ループカウンタ
            i = i + 1;
        }
    }
    //メモリの解放(Text Stage)
    for (int i = 0; i < STAGE_WIDTH; i = i + 1)
    {
        delete[] Stage[i];
    }
    delete[] Stage;

    //初期化
    Camera_x = 0;
}





//更新
void Game_Update() {
    if (CheckHitKey(KEY_INPUT_ESCAPE) != 0) { //Escキーが押されていたら
        SceneMgr_ChangeScene(eScene_Menu);//シーンをメニューに変更
    }
       
    
}

void Game_End(){}

#define Move_X (5)
#define Move_Max_Y (300)



//--------------------------------
//概　要:接触判定を行う関数
//因　数:玉、玉の因子、接触時に倒すか否か
//戻り値:接触判定結果
//--------------------------------
bool HitJudg(Ract c, int i_c, bool d)
{
    for (int i = 0; i < STAGE_TOTAL; i = i + 1)
    {
          if (i != i_c) //同じもの同士の比較を避けるため
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
                        //玉が障害物と接触した時
                        if ((c.k == e_Type::e_玉 ||
                            Block[i].k == e_Type::e_玉) &&
                            c.c == TRUE &&
                            Block[i].c == TRUE)
                            {
                            //接触時に倒すか
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

                        //ゴールに接触
                        else if (c.k == e_Type::e_玉 &&
                            Block[i].k == e_Type::e_ゴール)
                        {
                            Game_End();
                        }

                        return TRUE; //接触
                    }
                }
            }

    return FALSE;
        }

//ゲーム計算処理を行う関数
void Game_Cal()
{
    int y_add = 0;

    const int a = 25;

    //玉の因数
    int c_i = 0;

    int m_x = 0;
    for (int i = 0; i < STAGE_TOTAL; i = i + 1)
    {
        if (Block[i].k == e_Type::e_玉)
        {
            //玉コピー
            Boll = &Block[i];

            //玉座標コピー
            m_x = Block[i].x;

            //玉の因数
            c_i = i;

            //ループ解除
            break;
        }
    }

    if (CheckHitKey(KEY_INPUT_RIGHT) != 0) { // 右キーが押されていたら
    //if (Keyboard[e_But::But_Rig] != 0) { // 右キーが押されていたら

        Boll->d = FALSE;
        
        for (int i = 0; i < Move_X; i = i + 1)
        {
            //玉x座標計算
            Boll->x = Boll->x + 1;

            if (Boll->x > WINDOW_WIDTH / 2 &&
                Boll->x < STAGE_WIDTH * 50 - WINDOW_WIDTH / 2)
            {
                Camera_x = Camera_x - 1;
            }

            //x方向の玉と障害物の接触判定
            if (Boll->x > STAGE_WIDTH * 50 - Boll->w ||
                HitJudg(*Boll, c_i, FALSE) == TRUE)
            {
                //玉x座標計算
                Boll->x = Boll->x - 1;

                //カメラx座標計算
                if (Boll->x > WINDOW_WIDTH / 2 &&
                    Boll->x < STAGE_WIDTH * 50 - WINDOW_WIDTH / 2)
                {
                    Camera_x = Camera_x + 1;
                }

                //ループ解除
                break;
            }
        }
    }

    if (CheckHitKey(KEY_INPUT_LEFT) != 0) { // 左キーが押されていたら

        Boll->d = FALSE;

        for (int i = 0; i < Move_X; i = i + 1)
        {
            Boll->x = Boll->x - 1;
            if (Boll->x > WINDOW_WIDTH / 2 &&
                Boll->x < STAGE_WIDTH * 50 - WINDOW_WIDTH / 2)
            {
                Camera_x = Camera_x + 1;
            }

            //x方向の玉と障害物の接触判定
            if (Boll->x < 0 ||
                HitJudg(*Boll, c_i, FALSE) == TRUE)
            {
                //玉座標計算
                Boll->x = Boll->x + 1;

                //カメラx座標計算
                if (Boll->x > WINDOW_WIDTH / 2 &&
                    Boll->x < STAGE_WIDTH * 50 - WINDOW_WIDTH / 2)
                {
                    Camera_x = Camera_x - 1;
                }

                //ループ解除
                break;
            }
        }
    }
    //玉ジャンプ
    if (CheckHitKey(KEY_INPUT_UP) != 0 &&
        Jump == e_Jump::着地)
    {
        Jump = e_Jump::ジャンプ;

        Jump_Ymem = Boll->y;
    }

    if (Jump == e_Jump::ジャンプ)
    {
        Boll->j = Boll->j + 1;
        //移動量計算
        y_add = (Boll->j * Boll->j) / a;

        for (int i = 0; i < y_add; i = i + 1)
        {
            //玉のy座標計算
            Boll->y = Boll->y - 1;

            //y方向の玉とブロックの接触判定
            if (Boll->y < 0 ||
                HitJudg(*Boll, c_i, FALSE) == FALSE ||
                Move_Max_Y == Jump_Ymem - Boll->y)
            {
                //ジャンプフラグ変更
                Jump = e_Jump::落下;

                //玉y座標計算
                Boll->y = Boll->y + 1;

                //ジャンプカウンタ初期化
                Boll->j = 0;

                //ループ解除
                break;
            }

        }
    }

    //玉自由落下
    if (Jump == e_Jump::落下 ||
        Jump == e_Jump::着地)
    {
        //ジャンプカウンタ
        Boll->j = Boll->j + 1;

        //玉y方向移動量計算
        y_add = (Boll->j * Boll->j) / a;

        for (int i = 0; i < y_add; i = i + 1)
        {
            //ボールy座標計算
            Boll->y = Boll->y + 1;

            //y方向の玉とブロックの接触判定
            if (Boll->y < WINDOW_HEIGHT ||
                HitJudg(*Boll, c_i, TRUE) == TRUE)
            {
                //ジャンプフラグ変更
                Jump = e_Jump::着地;

                //玉y座標計算
                Boll->y = Boll->y - 1;

                //ジャンプカウンタ初期化
                Boll->j = 0;

                //ループ解除
                break;
            }
        }
    }

}

//描画
void Game_Draw() {

    //ブロック描画
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

    //while (1) {//繰り返し文

    //    if (ProcessMessage() != 0) { // メッセージ処理
    //        break;//ウィンドウの×ボタンが押されたらループを抜ける
    //    }
    //    // DrawRotaGraph(x, y, 1.0, 0.0,Handle, TRUE); // x,y の位置にキャラを描画

    //     //ボールが天井に接触した
    //    if (y < 10)
    //    {
    //        //ボールの速度計算
    //        Ball_Speed.y = Ball_Speed.y * (-1);
    //    }
    //    if (y > 400)
    //    {
    //        //ボールの速度計算
    //        Ball_Speed.y = Ball_Speed.y * (-1);
    //    }
    //    //if (CheckHitKey(KEY_INPUT_RIGHT) >= 1) { // 右キーが押されていたら
    //    //    x = x + 2;  // 右へ移動
    //    //}
    //    //if (CheckHitKey(KEY_INPUT_DOWN) == 1) { // 下キーが押されていたら
    //    //y = y + 2; // 下へ移動
    //    //}
    //    //if (CheckHitKey(KEY_INPUT_LEFT) == 1) { // 左キーが押されていたら
    //    //    x = x - 2;  // 左へ移動
    //    //}
    //    //if (CheckHitKey(KEY_INPUT_UP) == 1) { // 上キーが押されていたら
    //    //y = y - 2;    // 上へ移動
    //    //}
    //    DrawGraph(x, y, e_Type::e_玉, TRUE); //画像の描画
    //    //x = x + Ball_Speed.x; // xを2増やす
    //    y = y + Ball_Speed.y; // xを2増やす
    //    ScreenFlip(); // 7[ms]待機


            
        
       
       

      
    //}
 
}

