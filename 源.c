#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<Windows.h>
#include<Winuser.h>

#define high 25		//����߶�
#define width 60	//�����
#define border -1	//�߽�
#define blank 0		//�հ�
#define plane 1		//�ɻ�
#define	bullet 2	//�ӵ�
#define enemy 3		//�л�
#define destroy 4   //�ݻٱ��

int canvas[high + 2][width + 2];		//��Ϸ�����߶ȿ��
int pos_h, pos_w;						//�ɻ�λ��
int enemynum;							//�л�����
int interval;							//ͬ���ƴ���ģ��ʱ����
int itv_move;							//�л��ƶ���ʱ����
int itv_new;							//�л�ˢ�µ�ʱ����
int score;								//�÷�
int IsOver;								//�ж���Ϸ�Ƿ����

void StartUp();							//��Ϸ��ֵ��ʼ��
void Show();							//��Ϸ�������
void UpdateInput();						//���û������йص�״̬����
void UpdateNormal();					//���û������޹ص�״̬����
void HideCursor();						//���ع��
void gotoxy(int x, int y);				//�ص����


int main(void)
{
	StartUp();							//��ʼ��

	while (IsOver)
	{
		UpdateInput();
		UpdateNormal();
		Show();
	}

	printf("\t\tGame Over\n");
	Sleep(2500);						//��ͣ��Ϸ��������

	system("pause");
	return EXIT_SUCCESS;
}


void StartUp()
{
	IsOver = 1;
	score = 0;							//��ʼ������
	for (int i = 0; i < high + 2; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			if (i == 0 || i == high + 1 || j == 0 || j == width + 1)
			{
				canvas[i][j] = border;
			}
			else
			{
				canvas[i][j] = blank;
			}
		}
	}
	pos_h = high / 2;					//��ʼ���ɻ���ֱ����
	pos_w = width / 2;					//��ʼ���ɻ�ˮƽ����
	canvas[pos_h][pos_w] = plane;		//��ʼ���ɻ�λ��
	enemynum = 3;						//�л�����
	srand(time(NULL));
	interval = 4;						//��ʼ��ʱ��������
	itv_move = 5;						//��ʼ���л��ƶ�ʱ����
	itv_new = 40;						//��ʼ���л�ˢ��ʱ����
}

void Show()
{
	HideCursor();								//���ع��
	gotoxy(1, 1);								//�ص����,ˢ�»���

	for (int i = 0; i < high + 2; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			if (canvas[i][j] == plane)			//��ǰλ��Ϊ�ɻ�λ��
			{
				printf("*");
			}
			else if (canvas[i][j] == bullet)	//��ǰλ��Ϊ�ӵ�λ��
			{
				printf("|");
			}
			else if ( canvas[i][j] == enemy )	//��ǰλ��Ϊ�л�λ��
			{
				printf("$");
			}
			else if (canvas[i][j] == border)	//�߽�
			{
				printf("#");
			}
			else if (canvas[i][j] == blank)		//����,���ڱ߽���
			{
				printf(" ");
			}
			else if (canvas[i][j] == destroy)
			{
				printf("x");
			}
		}
		printf("\n");
	}
	printf("\n�÷�: %d", score);
}

void UpdateInput()
{
	char key_w = GetKeyState('w'),			//���� w �Ƿ���
		key_a = GetKeyState('a'),			//���� a �Ƿ���
		key_s = GetKeyState('s'),			//���� s �Ƿ���
		key_d = GetKeyState('d'),			//���� d �Ƿ���
		key_attack = GetKeyState(' ');		//���ӿո��Ƿ���
	/*���м�����ʱ*/
	if (kbhit())						
	{
		if (key_w < 0)						//��w����ʱ  ����
		{
			if (pos_h > 1)
			{
				canvas[pos_h][pos_w] = blank;
				if (canvas[pos_h - 1][pos_w] == enemy)		//�¸�λ���ǵл�,�ݻ�
				{
					canvas[pos_h][pos_w] = destroy;
					IsOver = 0;
				}
				else
				{
					canvas[--pos_h][pos_w] = plane;
				}
			}
		}
		if (key_s < 0)
		{
			if (pos_h < high)
			{
				canvas[pos_h][pos_w] = blank;
				if (canvas[pos_h + 1][pos_w] == enemy)
				{
					canvas[pos_h + 1][pos_w] = destroy;
					IsOver = 0;
				}
				else
				{
					canvas[++pos_h][pos_w] = plane;
				}
			}
		}
		if (key_a < 0)
		{
			if (pos_w > 1)
			{
				canvas[pos_h][pos_w] = blank;
				if (canvas[pos_h][pos_w - 1] == enemy)
				{
					canvas[pos_h][pos_w - 1] = destroy;
					IsOver = 0;
				}
				else
				{
					canvas[pos_h][--pos_w] = plane;
				}
			}
		}
		if (key_d < 0)
		{
			if (pos_w < width)
			{
				canvas[pos_h][pos_w] = blank;
				if (canvas[pos_h][pos_w + 1] == enemy)
				{
					canvas[pos_h][pos_w + 1] = destroy;
					IsOver = 0;
				}
				else
				{
					canvas[pos_h][++pos_w] = plane;
				}
			}
		}
		if (key_attack < 0)
		{
			if (pos_h != 1)
			{
				canvas[pos_h - 1][pos_w] = bullet;
			}
		}
	}
}

void UpdateNormal()
{
	int temp[high + 2][width + 2];			//�����ж�ԭλ��
	for (int i = 1; i <= high; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			temp[i][j] = canvas[i][j];
		}
	}

	for (int i = 1; i <= high; i++)
	{
		for (int j = 1; j <= width; j++)
		{
			if (temp[i][j] == enemy && interval % itv_move == 0)		//λ��Ϊ�л�
			{
				canvas[i][j] = blank;
				if (temp[i + 1][j] == bullet)
				{
					canvas[i + 1][j] = blank;
					score++;
					printf("\a");			//������Ч
				}
				else if (i < high)
				{
					canvas[i + 1][j] = enemy;
				}
				if (i + 1 == pos_h && j == pos_w)
				{
					canvas[i + 1][j] = destroy;
					IsOver = 0;
				}
			}

			if (temp[i][j] == bullet)
			{
				canvas[i][j] = blank;
				if (temp[i - 1][j] == enemy)
				{
					canvas[i - 1][j] = blank;
					score++;
					printf("\a");
				}
				else if (i > 1)
				{
					canvas[i - 1][j] = bullet;
				}
			}
		}
	}
	if (interval % itv_new == 0)		//�պõ�ʱ����
	{
		for (int i = 0; i < enemynum; i++)		//�����л�Ⱥ
		{
			canvas[rand() % 2 + 1][rand() % width + 1] = enemy;
		}
	}
	if (interval <= 100)	
	{
		interval++;			//ʱ�����ƴ�
	}
	else
	{
		interval = 0;			//ʱ�����ƴ�����
	}

}

void HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;
	cursor.dwSize = sizeof(cursor);
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorInfo(handle, &cursor);
}

void gotoxy(int x, int y)
{
	COORD pos;
	pos.X = x - 1;
	pos.Y = y - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}