#include"Graphics Logic.h"
using namespace std;

void init() {
	initgraph(680, 680);
	setbkcolor(RGB(255, 255, 255));
	setlinecolor(BLACK);
	cleardevice();
	BeginBatchDraw();
}

Occupied::Occupied() {
	settextstyle(50, 0, _T("良怀行书"));
	settextcolor(RED);
	outtextxy(50, 275, _T("该位置已被占用，请重新选择！"));
	FlushBatchDraw();
	Sleep(500);
}


void printboard(const Five_Chess &game, const MOUSEMSG &msg) {
	cleardevice();
	// 画空棋盘
	setlinecolor(BLACK);
	for(int i = 0; i < 15; ++i) {
		int offset = 50 + i * 40;
		line(50, offset, 610, offset);
		line(offset, 50, offset, 610);
	}
	// 画棋子
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
	// 根据鼠标消息画鼠标下方的子，做提示
	if(msg.x < 5 && msg.y < 5) return;
	int _x = (msg.x - 25) / 40, _y = (msg.y - 25) / 40;
	if(_x >= 0 && _x < 15 && _y >= 0 && _y < 15 && game.chessboard[_x][_y] == ' ') {
		int x = 50 + _x * 40;
		int y = 50 + _y * 40;
		if(game.is_index_0_player_playing) setfillcolor(LIGHTGRAY);
		else setfillcolor(DARKGRAY);
		solidcircle(x, y, 15);
	}
	FlushBatchDraw();
}
// 按钮类
class Button {
private:
	int x, y, width, height;
	TCHAR *text;
public:
	// 构造函数
	Button(int x, int y, int width, int height, const TCHAR *text) : x(x), y(y), width(width), height(height), text((TCHAR*)(text)) {}
	// 绘制按钮
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
	// 返回该按钮是否被按下
	bool isClicked(MOUSEMSG &msg) const noexcept {
		return msg.x >= x && msg.x <= x + width && msg.y >= y && msg.y <= y + height && msg.uMsg == WM_LBUTTONDOWN;
	}
};
// 多个按钮，fn为需要执行的其他命令，如打印标题
template<class Function> int multibutton(vector<Button> &buttons, Function fn) {
	MOUSEMSG msg;
	BeginBatchDraw();
	while(true) {
		if(MouseHit()) {
			msg = GetMouseMsg();
			cleardevice();
			fn();
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
	vector<Button> buttons = {
		Button(280, 250, 120, 50, _T("单人游戏")),
		Button(280, 320, 120, 50, _T("双人游戏")),
		Button(280, 390, 120, 50, _T("AI自动战斗")),
		Button(280, 460, 120, 50, _T("选项")),
		Button(280, 530, 120, 50, _T("退出游戏"))
	#ifdef _DEBUG
		,Button(50, 600, 200, 50, _T("打开工程文件debug")) // 仅在DEBUG模式下生效
	#endif
	};
	return multibutton(buttons, []() {
		settextcolor(RGB(0, 0, 0));
		settextstyle(50, 0, _T("黑体"));
		outtextxy(280, 100, _T("五子棋"));
	});
}
// 是否正在播放音乐
bool music_playing = true;
void options() {
	vector<Button> buttons = {
		Button(240, 300, 200, 50, _T("是否播放音乐")),
		Button(240, 400, 200, 50, _T("返回"))
	};
	while(true) {
		int choice = multibutton(buttons, []() {
			settextcolor(RGB(0, 0, 0));
			settextstyle(50, 0, _T("黑体"));
			outtextxy(300, 100, _T("选项"));
		});
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

int side() {
	vector<Button> buttons = {
		Button(240, 250, 200, 50, _T("黑子")),
		Button(240, 350, 200, 50, _T("白子")),
		Button(240, 450, 200, 50, _T("返回"))
	};
	return (multibutton(buttons, []() {
		settextcolor(RGB(0, 0, 0));
		settextstyle(50, 0, _T("黑体"));
		outtextxy(240, 100, _T("选择方向"));
	}));
}

void putchess(Five_Chess &game, MOUSEMSG &msg) {
	// 把鼠标消息转换为棋盘坐标
	int x = (msg.x - 25) / 40;
	int y = (msg.y - 25) / 40;
	// 如果在棋盘内则落子，可能抛出异常Occupied，此时需要在外部处理
	if(msg.uMsg == WM_LBUTTONDOWN && x < 15 && y < 15) {
		game.putchess(x, y);
	}
}