/* PicBaseSystem.h
 * 出口先生が作ったPIC製ゲーム機の描画処理を再現
 */

#define WINDOW_W (W_CELL * H_SIZE+30)
#define WINDOW_H (H_CELL * V_SIZE+30)
#define W_CELL 10
#define H_CELL W_CELL
#define H_SIZE 60
#define V_SIZE 60

extern int pat[V_SIZE][H_SIZE];	// VRAM
extern int U_BTN, R_BTN, D_BTN, L_BTN;
extern int BTN1, BTN2;

void disp_frame();									// 縦1列を描画。呼び出しごとに描画列はずれる。
void pat2rgb(int pat, int *r, int *g, int *b);		// pat配列への特殊は色指定を、RGB表現へ変換する。


