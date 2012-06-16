#include "StdAfx.h"

// 0x	GGRRBB--
// 0x00	00000000	黒
// 0xc0	11000000	緑
// 0x30	00110000	赤
// 0x0c	00001100	青
//


#include "DxLib.h"

#define W_CELL 8
#define H_CELL 8
#define H_SIZE 60
#define V_SIZE 60

int pat[V_SIZE][H_SIZE];

// pat[][]の要素をrgbに分解する。
void pat2rgb(int pat, int *r, int *g, int *b)
{
	int pat2 = pat >> 2;
	*b = pat2 & 3;
	pat2 >>= 2;
	*r = pat2 & 3;
	pat2 >>= 2;
	*g = pat2 & 3;

	*r = (*r) * 85;
	*g = (*g) * 85;
	*b = (*b) * 85;
}

void disp_frame()
{
	static int draw_x = 0;
	int y, cell_x, cell_y;
	int r,g,b;

	cell_x = W_CELL*draw_x;
	for (y=0; y<H_SIZE; y++)
	{
		pat2rgb(pat[y][draw_x], &r, &g, &b);

		cell_y = H_CELL*y;
		//DrawBox(cell_x, cell_y, cell_x+W_CELL, cell_y+H_CELL, GetColor(r,g,b), 1);	// 隙間なしバージョン
		DrawBox(cell_x, cell_y, cell_x+W_CELL-1, cell_y+H_CELL-1, GetColor(r,g,b), 1);	// 隙間ありバージョン
	}

	// 実行するたびに次の列を描画する
	draw_x = (draw_x+1) % H_SIZE;
}

void game_main()
{
	int i=0, j=0;

	for (i=0; i<V_SIZE; i++)
	{
		for (j=0; j<H_SIZE; j++)
		{
			pat[j][i] = 0xc0;	//赤にする
			if (10 < i && i < 30 && 40 < j && j < 50) {
				pat[j][i] = ~pat[j][i];
			}
		}
	}

	return;
}

// プログラムは WinMain から始まります
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{

	ChangeWindowMode(1);
	SetGraphMode(W_CELL * H_SIZE+30, H_CELL * V_SIZE+30, 16);
	//SetGraphM
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}

	//メインループ
	while ( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0)
	{
		disp_frame();

		game_main();
	}

	//WaitKey() ;				// キー入力待ち
	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了 
}