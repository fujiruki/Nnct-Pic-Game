#include "StdAfx.h"
#include "PicBaseSystem.h"

// 0x	GGRRBB--
// 0x00	00000000	��
// 0xc0	11000000	��
// 0x30	00110000	��
// 0x0c	00001100	��
//

//==============================================================
// �v���g�^�C�v�錾
//==============================================================
void fillBox(int x1, int y1, int x2, int y2, int color);

//==============================================================
// �萔�錾
//==============================================================
#define GOAL_HP_MAX	6		// �����S�[���ɒH�蒅���Ă悢��
#define GOAL_X	4
#define GOAL_Y	45
#define GOAL_W	4
#define GOAL_H	10
#define GOAL_COL	0x00
#define BUG_MAX 7							// ���̍ő吔
#define BUG_W	5
#define BUG_H	3
#define BUG_Y	(GOAL_Y+GOAL_H-BUG_H)		// ����Y���W
#define BUG_COL	0xc0
#define CTLINE_Y	6	// �L���b�`���[�̗U������Y���W
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
// �O���[�o���ϐ��̐錾
//==============================================================
unsigned int goal_hp = GOAL_HP_MAX;
unsigned int bug_x[BUG_MAX] = {0, 0, 0, 0, 0, 0, 0};	//7��BUG_MAX
unsigned int bug_y[BUG_MAX];	//7��BUG_MAX
int ct_x = 0;
unsigned int ct_y = CTLINE_Y + CTBAR_H;	// �݂艺���_�̒���������������
unsigned int bug_n = 1;		// ���݂̒��̍ő�o����

unsigned int picked = 0;		// �L���b�`�������̐�

unsigned int count_b = 0;		//���̓���p�J�E���^
unsigned int count_b_lim = 2000;
unsigned int count_ct= 0;		//�L���b�`���[�p�J�E���^
unsigned int count_ct_lim = 1200;
int ct_state = 0;				// �L���b�`���[�̏��


// DEBUG:
char s[10];


// ����������
void game_init()
{
	int i=0, j=0;
	for (i=0; i<BUG_MAX; i++) {
		bug_y[i] = BUG_Y;
		bug_x[i] = 40+ i*(BUG_W+2);
	}
}

//==============================================================
// PIC�Q�[���̃��C���֐�
//==============================================================
void game_main()
{
	// ����������
	int i, j;

	// ����
	if (BTN1 && ct_state ==  CT_STATE_NORMAL) {
		printfDx("BTN1 && CT_STATE_NORMAL");
		ct_state = CT_STATE_STOP;
	}

	// �又��
	fillBox(0, 0, H_SIZE, V_SIZE, 0xff);	// �w�i�h��Ԃ�


	// �A�j���[�V���������ޏ���
	if (count_b>count_b_lim) {
		count_b = 0;
		for (i=0; i<bug_n; i++) {
			// �ړ����ŏ㏸���Ă��Ȃ������S�[���֌�����
			if (bug_x[i] > GOAL_X && bug_y[i] == BUG_Y) {
				bug_x[i]--;
			}
		}
	}

	// �L���b�`���[
	if (count_ct>count_ct_lim) {
		count_ct = 0;
		switch (ct_state) {
		case CT_STATE_NORMAL:
			ct_x++;
			if (ct_x >= H_SIZE) { ct_x = -CT_W; }
			break;
		case CT_STATE_STOP:
			ct_state++;	// ��~�b�����J�E���g(CT_STATE_DOWN�܂�)
			break;
		case CT_STATE_DOWN:
			ct_y++;
			if (BUG_Y <= ct_y) { ct_state = CT_STATE_UP; }
			break;
		case CT_STATE_UP:
			ct_y--;
			if (ct_y <= CTLINE_Y + CTBAR_H) { ct_state = CT_STATE_NORMAL; }
			break;
		default:	//�ǂ�ł��Ȃ��l(=CT_STATE_STOP< st_state <ST_STATE_DOWN)
			ct_state++;
			break;
		}
	}



	// �`��
	// �S�[��
	fillBox(GOAL_X, GOAL_Y, GOAL_X+GOAL_W, GOAL_Y+GOAL_H, GOAL_COL);
	// ��
	for (i=0; i<bug_n; i++) {
		if (bug_x[i] > GOAL_X) {
			fillBox(bug_x[i], bug_y[i], bug_x[i]+BUG_W, bug_y[i]+BUG_H, BUG_COL);
		}
	}

	// �L���b�`���[
	fillBox(0, CTLINE_Y, H_SIZE, CTLINE_Y+2, CTLINE_COL);
	fillBox(ct_x+CT_W/2, CTLINE_Y+2, ct_x+CT_W/2+1, ct_y, CT_COL);
	fillBox(ct_x, ct_y, ct_x+CT_W, ct_y+CT_H, CT_COL);


	// �I������
	count_b++;
	count_ct++;
	return;
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
						LPSTR lpCmdLine, int nCmdShow )
{
	//����������
	ChangeWindowMode(1);
	SetGraphMode(WINDOW_W, WINDOW_H, 16);
	if( DxLib_Init() == -1 )		// �c�w���C�u��������������
	{
		return -1 ;			// �G���[���N�����璼���ɏI��
	}

	game_init();

	//���C�����[�v
	while ( ProcessMessage() == 0 && CheckHitKey( KEY_INPUT_ESCAPE ) == 0)
	{
		disp_frame();
		game_main();
	}


	// �I������
	DrawString(WINDOW_W - 80, WINDOW_H - 20, "END.", 0xaaff0000);
	WaitKey();				// �L�[���͑҂�
	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}


// (x1,y1)����(x2 - 1,y2 - 1)�܂ł̒����`��h��Ԃ��B
// ���ӁF(y1<y2) && (x1<x2)�ł��邱�ƁI
void fillBox(int x1, int y1, int x2, int y2, int color)
{
	int i, j;

	//�S���͂ݏo�Ă���΁A�Ȃɂ����Ȃ��B
	if (H_SIZE <= x1 || V_SIZE <= y1 || x2 < 0 || y2 < 0) { return; }

	// ���Əc�ł͖��o�Ă���΁A���܂�悤�ɃT�C�Y����
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
