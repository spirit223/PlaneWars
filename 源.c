#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<conio.h>
#include<time.h>
#include<Windows.h>
#include<Winuser.h>

#define high 25		//画面高度
#define width 60	//画面宽
#define border -1	//边界
#define blank 0		//空白
#define plane 1		//飞机
#define	bullet 2	//子弹
#define enemy 3		//敌机
#define destroy 4   //摧毁标记

int canvas[high + 2][width + 2];		//游戏场景高度宽度
int pos_h, pos_w;						//飞机位置
int enemynum;							//敌机数量
int interval;							//同个计次来模拟时间间隔
int itv_move;							//敌机移动的时间间隔
int itv_new;							//敌机刷新的时间间隔
int score;								//得分
int IsOver;								//判断游戏是否结束

void StartUp();							//游戏数值初始化
void Show();							//游戏界面输出
void UpdateInput();						//跟用户输入有关的状态更新
void UpdateNormal();					//与用户输入无关的状态更新
void HideCursor();						//隐藏光标
void gotoxy(int x, int y);				//回调光标


int main(void)
{
	StartUp();							//初始化

	while (IsOver)
	{
		UpdateInput();
		UpdateNormal();
		Show();
	}

	printf("\t\tGame Over\n");
	Sleep(2500);						//暂停游戏结束界面

	system("pause");
	return EXIT_SUCCESS;
}


void StartUp()
{
	IsOver = 1;
	score = 0;							//初始化分数
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
	pos_h = high / 2;					//初始化飞机竖直坐标
	pos_w = width / 2;					//初始化飞机水平坐标
	canvas[pos_h][pos_w] = plane;		//初始化飞机位置
	enemynum = 3;						//敌机数量
	srand(time(NULL));
	interval = 4;						//初始化时间间隔计数
	itv_move = 5;						//初始化敌机移动时间间隔
	itv_new = 40;						//初始化敌机刷新时间间隔
}

void Show()
{
	HideCursor();								//隐藏光标
	gotoxy(1, 1);								//回调光标,刷新画面

	for (int i = 0; i < high + 2; i++)
	{
		for (int j = 0; j < width + 2; j++)
		{
			if (canvas[i][j] == plane)			//当前位置为飞机位置
			{
				printf("*");
			}
			else if (canvas[i][j] == bullet)	//当前位置为子弹位置
			{
				printf("|");
			}
			else if ( canvas[i][j] == enemy )	//当前位置为敌机位置
			{
				printf("$");
			}
			else if (canvas[i][j] == border)	//边界
			{
				printf("#");
			}
			else if (canvas[i][j] == blank)		//无物,且在边界内
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
	printf("\n得分: %d", score);
}

void UpdateInput()
{
	char key_w = GetKeyState('w'),			//监视 w 是否按下
		key_a = GetKeyState('a'),			//监视 a 是否按下
		key_s = GetKeyState('s'),			//监视 s 是否按下
		key_d = GetKeyState('d'),			//监视 d 是否按下
		key_attack = GetKeyState(' ');		//监视空格是否按下
	/*当有键按下时*/
	if (kbhit())						
	{
		if (key_w < 0)						//当w按下时  上移
		{
			if (pos_h > 1)
			{
				canvas[pos_h][pos_w] = blank;
				if (canvas[pos_h - 1][pos_w] == enemy)		//下个位置是敌机,摧毁
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
	int temp[high + 2][width + 2];			//用来判断原位置
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
			if (temp[i][j] == enemy && interval % itv_move == 0)		//位置为敌机
			{
				canvas[i][j] = blank;
				if (temp[i + 1][j] == bullet)
				{
					canvas[i + 1][j] = blank;
					score++;
					printf("\a");			//击中音效
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
	if (interval % itv_new == 0)		//刚好到时间间隔
	{
		for (int i = 0; i < enemynum; i++)		//新增敌机群
		{
			canvas[rand() % 2 + 1][rand() % width + 1] = enemy;
		}
	}
	if (interval <= 100)	
	{
		interval++;			//时间间隔计次
	}
	else
	{
		interval = 0;			//时间间隔计次清零
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