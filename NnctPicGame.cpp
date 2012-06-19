#include "StdAfx.h"
#include "PicBaseSystem.h"

// 0x	GGRRBB--
// 0x00	00000000	黒
// 0xc0	11000000	緑
// 0x30	00110000	赤
// 0x0c	00001100	青
//

//==============================================================
// プロトタイプ宣言
//==============================================================
void fillBox(int x1, int y1, int x2, int y2, int color);

//==============================================================
// 定数宣言
//==============================================================
#define GOAL_HP_MAX	6		// 虫がゴールに辿り着いてよい回数
#define GOAL_X	4
#define GOAL_Y	45
#define GOAL_W	4
#define GOAL_H	10
#define GOAL_COL	0x00
#define BUG_MAX 7							// 虫の最大数
#define BUG_W	5
#define BUG_H	3
#define BUG_Y	(GOAL_Y+GOAL_H-BUG_H)		// 虫のY座標
#define BUG_COL	0xc0
#define CTLINE_Y	6	// キャッチャーの誘導線のY座標
#define CTLINE_COL	0x3c
#define CT_COL	0x0c
#define CTBAR_H	3
#define CT_W	9
#define CT_H	7
#define CT_STATE_NORMAL	0
#define CT_STATE_STOP	1
#define CT_STATE_DOWN	3
#define CT_STATE_UP		4


//==============================================================
// グローバル変数の宣言
//==============================================================
unsigned int goal_hp = GOAL_HP_MAX;
unsigned int bug_x[BUG_MAX] = {0, 0, 0, 0, 0, 0, 0};	//7はBUG_MAX
unsigned int bug_y[BUG_MAX];	//7はBUG_MAX
int ct_x = 0;
unsigned int ct_y = CTLINE_Y + CTBAR_H;	// 吊り下げ棒の長さ分足したもの
unsigned int bug_n = 1;		// 現在の虫の最大出現数

unsigned int picked = 0;		// キャッチした虫の数

unsigned int count_b = 0;		//虫の動作用カウンタ
unsigned int count_b_lim = 2000;
unsigned int count_ct= 0;		//キャッチャー用カウンタ
unsigned int count_ct_lim = 1200;
int ct_state = 0;				// キャッチャーの状態


// DEBUG:
char s[10];


// 初期化処理
void game_init()
{
	int i=0, j=0;
	for (i=0; i<BUG_MAX; i++) {
		bug_y[i] = BUG_Y;
		bug_x[i] = 40+ i*(BUG_W+2);
	}
}

//==============================================================
// PICゲームのメイン関数
//==============================================================
void game_main()
{
	// 初期化処理
	int i, j;

	// 入力
	if (BTN1 && ct_state ==  CT_STATE_NORMAL) {
		printfDx("BTN1 && CT_STATE_NORMAL");
		ct_state = CT_STATE_STOP;
	}

	// 主処理
	fillBox(0, 0, H_SIZE, V_SIZE, 0xff);	// 背景塗りつぶし


	// アニメーションが絡む処理
	if (count_b>count_b_lim) {
		count_b = 0;
		for (i=0; i<bug_n; i++) {
			// 移動中で上昇していない虫がゴールへ向かう
			if (bug_x[i] > GOAL_X && bug_y[i] == BUG_Y) {
				bug_x[i]--;
			}
		}
	}

	// キャッチャー
	if (count_ct>count_ct_lim) {
		count_ct = 0;
		switch (ct_state) {
		case CT_STATE_NORMAL:
			ct_x++;
			if (ct_x >= H_SIZE) { ct_x = -CT_W; }
			break;
		case CT_STATE_STOP:
			ct_state++;	// 停止秒数をカウント(CT_STATE_DOWNまで)
			break;
		case CT_STATE_DOWN:
			ct_y++;
			if (BUG_Y <= ct_y) { ct_state = CT_STATE_UP; }
			break;
		case CT_STATE_UP:
			ct_y--;
			if (ct_y <= CTLINE_Y + CTBAR_H) { ct_state = CT_STATE_NORMAL; }
			break;
		default:	//どれでもない値(=CT_STATE_STOP< st_state <ST_STATE_DOWN)
			ct_state++;
			break;
		}
	}



	// 描画
	// ゴール
	fillBox(GOAL_X, GOAL_Y, GOAL_X+GOAL_W, GOAL_Y+GOAL_H, GOAL_COL);
	// 虫
	for (i=0; i<bug_n; i++) {
		if (bug_x[i] > GOAL_X) {
			fillBox(bug_x[i], bug_y[i], bug_x[i]+BUG_W, bug_y[i]+BUG_H, BUG_COL);
		}
	}

	// キャッチャー
	fillBox(0, CTLINE_Y, H_SIZE, CTLINE_Y+2, CTLINE_COL);
	fillBox(ct_x+CT_W/2, CTLINE_Y+2, ct_x+CT_W/2+1, ct_y, CT_COL);
	fillBox(ct_x, ct_y, ct_x+CT_W, ct_y+CT_H, CT_COL);


	// 終了処理
	count_b++;
	count_ct++;
	return;
}

// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	//初期化処理
	ChangeWindowMode(1);
	SetGraphMode(WINDOW_W, WINDOW_H, 16);
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}

	game_init();

	//メインループ
	while ( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0)
	{
		disp_frame();
		game_main();
	}


	// 終了処理
	DrawString(WINDOW_W - 80, WINDOW_H - 20, "END.", 0xaaff0000);
	WaitKey();				// キー入力待ち
	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}


// (x1,y1)から(x2 - 1,y2 - 1)までの長方形を塗りつぶす。
// 注意：(y1<y2) && (x1<x2)であること！
void fillBox(int x1, int y1, int x2, int y2, int color)
{
	int i, j;

	//全部はみ出ていれば、なにもしない。
	if (H_SIZE <= x1 || V_SIZE <= y1 || x2 < 0 || y2 < 0) { return; }

	// 横と縦では未出ていれば、収まるようにサイズ調整
	if (x1 < 0) { x1 = 0; }
	if (y1 < 0) { x1 = 0; }
	if (H_SIZE <= x2) { x2 = H_SIZE-1; }
	if (V_SIZE <= y2) { y2 = V_SIZE-1; }

	for (i=y1; i<y2; i++) {
		for (j=x1; j<x2; j++) {
			pat[i][j] = color;
		}
	}

	return;
}
