#include <yvals_core.h>
#if !_HAS_CXX17 && __cplusplus < 201703L
#error C++17 is required.
#endif
#include"Graphics Logic.h"
#include"Game Logic.h"
#include"Search_AI.h"
#pragma comment(lib,"winmm.lib")
extern HWND game;
using namespace std;
// 虽然把人当做AI的多态很抽象，但我懒得重命名了，就这样吧
class Human : public AI_Base {
public:
	Human() = default;
	void judge(const Five_Chess &fc) {
		// 你怎么判断需要我写？自己判断
	}
	void put(Five_Chess &fc) {
		MOUSEMSG msg = GetMouseMsg();
		while(msg.uMsg != WM_LBUTTONDOWN) {
			msg = GetMouseMsg();
			printboard(fc, msg);
		}
		putchess(fc, msg);
	}
	~Human() = default;
};
void _playgame(Five_Chess &fc, AI_Base *ai1, AI_Base *ai2, bool dosleep = false) {
	printboard(fc);
	const MOUSEMSG midmsg = { 0, 0, 0, 0, 0, 0, 340, 340, 0 };
	char ch;
	while(!fc.has_ended(ch)) {
		ai1->judge(fc);
		ai1->put(fc);
		printboard(fc);
		if(fc.has_ended(ch)) break;
		if(dosleep) Sleep(500);
		ai2->judge(fc);
		ai2->put(fc);
		printboard(fc);
		if(dosleep && !fc.has_ended(ch)) Sleep(500);
	}
	settextcolor(RED);
	settextstyle(50, 0, _T("良怀行书"));
	if(ch == '1') {
		outtextxy(250, 220, _T("白棋获胜！"));
	}
	else if(ch == '2') {
		outtextxy(250, 220, _T("黑棋获胜！"));
	}
	else {
		outtextxy(250, 220, _T("平局！"));
	}
	Sleep(1000);
	setlinecolor(BLACK);
}
int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmd, _In_ int n_cmd) {
	mciSendString(_T("open FiveChess.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("play bgm repeat"), NULL, 0, NULL);
	Sleep(1000);
	init();
	int aichoice = 0;
	while(true) {
		int choice = menu();
		switch(choice) {
			case 0: {
				bool _myside = side();
				Five_Chess fc;
				AI_Base *ai = new Search_AI(14);
				Human h;
				if(_myside) fc.putchess(7, 7);
				_playgame(fc, &h, ai);
				delete ai;
				break;
			} // case 0
			case 1: {
				Five_Chess fc;
				MOUSEMSG msg;
				Human h1, h2;
				_playgame(fc, &h1, &h2);
				break;
			} // case 1
			case 2: {
				const MOUSEMSG midmsg = { 0, 0, 0, 0, 0, 0, 340, 340, 0 };
				Five_Chess fc;
				char ch = ' ';
				AI_Base *ai1 = new Search_AI(14);
				AI_Base *ai2 = new Search_AI(14);
				SYSTEMTIME time;
				GetLocalTime(&time);
				fc.putchess(7, 7);
				printboard(fc, midmsg);
				int _x = time.wMilliseconds % 3 - 1, _y = (time.wMilliseconds / 3) % 3 - 1;
				if(_x == 0 && _y == 0) ++_y;
				Sleep(500);
				fc.putchess(7 + _x, 7 + _y);
				printboard(fc, midmsg);
				Sleep(500);
				_playgame(fc, ai1, ai2, true);
				delete ai1;
				delete ai2;
				break;
			} // case 2
			case 3: {
				options();
				break;
			}
			case 4: {
				closegraph();
				return 0;
			}
			case 5: {
				ShellExecute(nullptr, _T("open"), _T("D:/文档/Visual Studio 2022/C++/c++课设 五子棋AI/c++课设 五子棋AI.sln"), 0, 0, SW_SHOWNORMAL);
				closegraph();
				return 0;
			}
		}
	}
}