/* PicBaseSystem.h
 * �o���搶�������PIC���Q�[���@�̕`�揈�����Č�
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

void disp_frame();									// �c1���`��B�Ăяo�����Ƃɕ`���͂����B
void pat2rgb(int pat, int *r, int *g, int *b);		// pat�z��ւ̓���͐F�w����ARGB�\���֕ϊ�����B


