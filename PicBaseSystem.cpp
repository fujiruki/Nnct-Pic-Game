#include "StdAfx.h"
#include "PicBaseSystem.h"

//===============================================
// グローバル変数の宣言
//===============================================
int pat[V_SIZE][H_SIZE];	// VRAM
int U_BTN, R_BTN, D_BTN, L_BTN;
int BTN1, BTN2;

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
	static unsigned int e = GetNowCount();
	int y, cell_x, cell_y;
	int r,g,b;
	char key[256];	// 入力状態を格納する
	

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


	//1/60秒間隔でループ。
	//while (GetNowCount() - e < (1.0/60*1000)) { Sleep(10); }
	//e = GetNowCount();

	// 入力信号を取得
	GetHitKeyStateAll(key);
	U_BTN = key[KEY_INPUT_UP];
	D_BTN = key[KEY_INPUT_DOWN];
	R_BTN = key[KEY_INPUT_RIGHT];
	L_BTN = key[KEY_INPUT_LEFT];
	BTN1  = key[KEY_INPUT_Z];
	BTN2  = key[KEY_INPUT_X];
}
