#include"Graphics Logic.h"
using namespace std;
HWND game;
void init() {
	game = initgraph(680, 680);
	setbkcolor(RGB(255, 255, 255));
	setlinecolor(BLACK);
	cleardevice();
}

Occupied::Occupied() {
	settextstyle(50, 0, _T("良怀行书"));
	settextcolor(RED);
	outtextxy(50, 275, _T("该位置已被占用，请重新选择！"));
	Sleep(500);
}


void printboard(const Five_Chess &game, const MOUSEMSG& msg) {
	cleardevice();
	setlinecolor(BLACK);
	for(int i = 0; i < 15; ++i) {
		int offset = 50 + i * 40;
		line(50, offset, 610, offset);
		line(offset, 50, offset, 610);
	}
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 15; ++j) {
			int x = 50 + i * 40;
			int y = 50 + j * 40;
			if(game.chessboard[i][j] == '1') {
				setfillcolor(WHITE);
				setlinecolor(BLACK);
				solidcircle(x, y, 15);
				circle(x, y, 15);
			}
			else if(game.chessboard[i][j] == '2') {
				setfillcolor(BLACK);
				solidcircle(x, y, 15);
			}
		}
	}
	int _x = (msg.x - 25) / 40, _y = (msg.y - 25) / 40;
	if(_x >= 0 && _x < 15 && _y >= 0 && _y < 15 && game.chessboard[_x][_y] == ' ') {
		int x = 50 + _x * 40;
		int y = 50 + _y * 40;
		if(game.is_index_0_player_playing) setfillcolor(RGB(120, 120, 120));
		else setfillcolor(RGB(30, 30, 30));
		solidcircle(x, y, 15);
	}
}

class Button {
private:
	int x, y, width, height;
	TCHAR *text;
public:
	Button(int x, int y, int width, int height, const TCHAR *text) : x(x), y(y), width(width), height(height), text((TCHAR*)(text)) {}
	void draw(MOUSEMSG &msg) {
		if(msg.x >= x && msg.x <= x + width && msg.y >= y && msg.y <= y + height) {
			setfillcolor(RGB(233, 233, 233));
			setbkcolor(RGB(233, 233, 233));
		}
		else {
			setfillcolor(RGB(188, 188, 188));
			setbkcolor(RGB(188, 188, 188));
		}
		fillrectangle(x, y, x + width, y + height);
		settextstyle(20, 0, _T("黑体"));
		settextcolor(RGB(0, 0, 0));
		int textWidth = textwidth(text);
		int textHeight = textheight(text);
		int textX = x + (width - textWidth) / 2;
		int textY = y + (height - textHeight) / 2;
		outtextxy(textX, textY, text);
		setbkcolor(RGB(255, 255, 255));
	}
	bool isClicked(MOUSEMSG &msg) const noexcept {
		return msg.x >= x && msg.x <= x + width && msg.y >= y && msg.y <= y + height && msg.uMsg == WM_LBUTTONDOWN;
	}
};

void printTitle() {
	settextcolor(RGB(0, 0, 0));
	settextstyle(50, 0, _T("黑体"));
	outtextxy(280, 100, _T("五子棋"));
}

int multibutton(vector<Button> &buttons) {
	MOUSEMSG msg;
	BeginBatchDraw();
	while(true) {
		if(MouseHit()) {
			msg = GetMouseMsg();
			cleardevice();
			printTitle();
			for(int i = 0; i < buttons.size(); ++i) {
				buttons[i].draw(msg);
				if(buttons[i].isClicked(msg)) {
					EndBatchDraw();
					return i;
				}
			}
			FlushBatchDraw();
		}
		Sleep(4);
	}
}

int menu() {
	vector<Button> buttons;
	buttons.push_back(Button(280, 250, 120, 50, _T("单人游戏")));
	buttons.push_back(Button(280, 320, 120, 50, _T("双人游戏")));
	buttons.push_back(Button(280, 390, 120, 50, _T("AI自动战斗")));
	buttons.push_back(Button(280, 460, 120, 50, _T("选项")));
	buttons.push_back(Button(280, 530, 120, 50, _T("退出游戏")));
	return multibutton(buttons);
}
bool music_playing = true;
void options() {
	vector<Button> buttons;
	buttons.push_back(Button(240, 250, 200, 50, _T("是否播放音乐")));
	buttons.push_back(Button(240, 350, 200, 50, _T("返回")));
	while(true) {
		int choice = multibutton(buttons);
		if(choice == 0) {
			music_playing = !music_playing;
			if(music_playing) {
				mciSendString(_T("play bgm repeat"), NULL, 0, NULL);
			}
			else {
				mciSendString(_T("stop bgm"), NULL, 0, NULL);
			}
		}
		else return;
	}
}
bool side() {
	vector<Button> buttons;
	buttons.push_back(Button(240, 250, 200, 50, _T("黑子")));
	buttons.push_back(Button(240, 350, 200, 50, _T("白子")));
	if(multibutton(buttons)) return true;
	return false;
}

void putchess(Five_Chess &game, MOUSEMSG &msg) {
	int x = (msg.x - 25) / 40;
	int y = (msg.y - 25) / 40;
	try {
		if(msg.uMsg == WM_LBUTTONDOWN && x < 15 && y < 15) {
			game.putchess(x, y);
		}
	}
	catch(Occupied) {
		return;
	}
}