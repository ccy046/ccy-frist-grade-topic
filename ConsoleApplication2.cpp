#include"pch.h"
#define _CRT_SECURE_NO_WARNINGS
#undef UNICODE //取消UNICODE 讓後續編譯都以MBCS
#undef _UNICODE
#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>

#define WIN_WIDTH 640 
#define WIN_HEIGHT 800
#define MAX_BOARD 10 //樓梯最大數量
#define MAX_SPACE 70 //定義樓梯間距

class Board {//樓梯結構
public:
	int x, y;
	int length;
	DWORD color;//DWORD unsing lone
	Board() {
		x = 0;
		y = 0;
		length = 0;
	}
};

class Ball {
public:
	int x, y;
	int vx, vy;//加速度
	int r, score;
	DWORD color;
	Ball() {
		x = 0;
		y = 0;
		vx = 0;
		vy = 0;
		r = 0;
		score = 0;
	}
};
static DWORD mcolor[10] = { Pink,BLACK,WHITE,GREEN,YELLOW };

class Game_logic {
private:
	Ball ball;
	Board board[MAX_BOARD];
public:
	void GameInit() {
		//設計隨機樹種子,把系統開始到現在經過得毫秒數作為種子
		srand(GetTickCount());
		for (int i = 0; i < MAX_BOARD; i++) { //初始化樓梯

			board[i].length = rand() % 100 + 50;
			board[i].x = rand() % (WIN_WIDTH - board[i].length);//讓樓梯可以在視窗的隨機位置產生必且不會超出視窗
			if (i == 0) {
				board[i].y = rand() % 200 + 200;
			}
			else {
				board[i].y = board[i - 1].y + MAX_SPACE;//每一塊樓梯都等於上一塊的Y加上間隔 讓他間格一樣不會擠在一起
			}
			board[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
		}
		//初始化ball
		ball.r = 10; //初始化ball的半徑 
		ball.x = 120;
		ball.y = 20;
		ball.score = 0;
		ball.color = board[0].color;//讓球開始的時候等於樓梯顏色
		ball.vx = 5;
		ball.vy = 1;
	}
	void GameDraw() {
		cleardevice();//清楚樓梯的痕跡避免樓梯往上時變成像在畫畫一樣
		for (int i = 0; i < MAX_BOARD; i++) {
			setfillcolor(board[i].color);//畫樓梯
			solidrectangle(board[i].x, board[i].y, board[i].x + board[i].length, board[i].y + 10);//Y是讓樓梯的高度變高
		}
		//畫ball
		setfillcolor(ball.color);
		solidcircle(ball.x, ball.y, ball.r);
		//顯示分數 把整數ball.score轉為字串
		char tempscore[1000] = "";
		sprintf(tempscore, "分數: %d", ball.score);
		outtextxy(10, 10, tempscore);//該函數在屏幕上的指定點(x，y)上顯示文本或字串
		/*if (ball.y >= 800 || ball.y < 0) {
			return;
		}*/
	}
	void BoardMove() {//移動板子改變他的y座標

		for (int i = 0; i < MAX_BOARD; i++) {
			board[i].y -= 1;//讓樓梯往上抬
			if (board[i].y < 0) {//如果已經超過邊界,重新在丟樓梯上來 
				board[i].y = MAX_BOARD * MAX_SPACE;//得到最後一個板子的位置
				board[i].length = rand() % 100 + 50;
				board[i].x = rand() % (WIN_WIDTH - board[i].length);
				board[i].color = RGB(rand() % 256, rand() % 256, rand() % 256);
			}
		}
	}
	void BallMove(int speed) {

		bool get = true;
		if (GetAsyncKeyState(VK_LEFT)) {//左鍵 GetAsyncKeyState是讓他可以或取鍵盤滑鼠的函式
			ball.x -= speed;
			ball.score++;
		}
		if (GetAsyncKeyState(VK_RIGHT)) {//右鍵
			ball.x += speed;
			ball.score++;
		}
		if (GetAsyncKeyState(VK_DOWN)) {//下鍵
			ball.y += speed;
			ball.score++;
		}
		for (int i = 0; i < MAX_BOARD; i++) {//讓球在樓梯上跑,沒在樓梯上的話就往下掉
			if (ball.x >= board[i].x && ball.x + ball.r < board[i].x + board[i].length && ball.y + ball.r >= board[i].y && ball.y + ball.r <= board[i].y + 5) {
				//球左邊不能小於盤子的左邊 && 球的右邊不能大於盤子的右邊 && 球+半徑(這樣才是底)要大於盤子頂 && 球加半徑要小於盤子頂+高度(這樣才是盤子底)
				ball.y = board[i].y - ball.r;
				get = false;
				break;
			}
		}
		if (get) {//球如果沒在樓梯上往下墜
			ball.y += ball.vy;
		}

	}
	bool gameover() {
		if (ball.y < 0 || ball.y >= 800) {
			MessageBox(nullptr, "遊戲結束", "game over", 0);
			return true;
		}
		else {
			return false;
		}
	}
};

int main() {

	Game_logic game;
	initgraph(WIN_WIDTH, WIN_HEIGHT);
	game.GameInit();
	bool gamenow = false;
	BeginBatchDraw();//緩衝繪圖
	//原理:讓繪製的指令不會直接傳給窗口，讓他先傳給CPU需要時再傳給窗口，不會一直繪畫，避免畫面閃爍
	while (!gamenow) {
		gamenow = game.gameover();
		game.BoardMove();
		game.GameDraw();
		FlushBatchDraw();
		//FlushBatchDraw();
		game.BallMove(5);
		Sleep(3);
	}
	EndBatchDraw();
	closegraph();
	getchar();//防止閃退 
	return 0;
}