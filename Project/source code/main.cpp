#ifdef _MSC_VER
#include <yvals_core.h>
#endif
#if !_HAS_CXX17 && __cplusplus < 201703L
#error C++17 is required.
#endif
#include"Graphics Logic.h"
#include"Game Logic.h"
#include"Search_AI.h"
#pragma comment(lib,"winmm.lib")
using namespace std;
// 人类操作
class Human : public Player_Base {
public:
	Human() = default;
	// 人类的判断可由人类自行完成
	void judge(const Five_Chess &fc) {}
	// 获取鼠标信息，点击时落子
	void put(Five_Chess &fc) {
		while(true) {
			// 获取鼠标信息
			MOUSEMSG msg = GetMouseMsg();
			// 没有点击左键的时候不断获取鼠标信息
			while(msg.uMsg != WM_LBUTTONDOWN) {
				msg = GetMouseMsg();
				printboard(fc, msg);
			}
			// 尝试在按下左键的地方落子
			try {
				putchess(fc, msg);
				// 成功落子，退出循环
				break;
			}
			// 落子失败，该位置被占用，继续循环
			catch(Occupied &oc){}
		}
	}
	~Human() = default;
};
// 进行游戏的函数
// 第四个参数为落子后是否等待500ms，这是为了防止AI自我对弈时落子过快无法看清
void _playgame(Five_Chess &fc, Player_Base *player1, Player_Base *player2, bool dosleep = false) {
	// 进入游戏时打印棋盘
	printboard(fc);
	// 获取胜利者的代理对象
	char ch;
	// 在游戏结束前循环二人操作
	while(!fc.has_ended(ch)) {
		player1->judge(fc);
		player1->put(fc);
		printboard(fc);
		// 如果第一人落子后游戏已经结束，则直接退出循环
		if(fc.has_ended(ch)) break;
		if(dosleep) Sleep(500);
		player2->judge(fc);
		player2->put(fc);
		printboard(fc);
		if(dosleep && !fc.has_ended(ch)) Sleep(500);
	}
	// 结算动画
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
	// 等待1s后返回主菜单
	Sleep(1000);
	setlinecolor(BLACK);
}
// 使用WinMain函数，如果需要编译，请使用Visual Studio，打开C++17以上标准，并在链接器->系统->子系统处选择窗口/WINDOWS
int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmd, _In_ int n_cmd) {
	// 播放BGM
	mciSendString(_T("open ../resources/FiveChess.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("play bgm repeat"), NULL, 0, NULL);
	// 等待BGM开始播放（雾）
	Sleep(1000);
	// 显示游戏画面
	init();
	// 游戏循环
	while(true) {
		// 通过菜单栏的返回值获取玩家选项
		int choice = menu();
		switch(choice) {
			case 0: { // 单人游戏
				int _side = side(); // 选边，0为黑子，1为白子，2为返回
				Five_Chess fc;
				Search_AI ai;
				Human h;
				if(_side == 2) break; // break返回主菜单
				else if(_side == 1) fc.putchess(7, 7); // 玩家选择白子，在棋盘中间直接摆放一颗黑子
				_playgame(fc, &h, &ai); // 进行游戏
				break;
			} // case 0
			case 1: { // 双人游戏
				Five_Chess fc;
				MOUSEMSG msg;
				Human h;
				_playgame(fc, &h, &h); // 玩家与玩家对抗
				break;
			} // case 1
			case 2: { // AI自我对弈
				// 为了给AI对决尽可能增加随机性，我们固定第一颗子放在中央，第二颗子随机放在中央的周围
				Five_Chess fc;
				char ch = ' ';
				Search_AI ai;
				SYSTEMTIME time; // 以当前的时间作为决定第二颗子的落子位置
				GetLocalTime(&time);
				fc.putchess(7, 7);
				printboard(fc);
				int _x = time.wMilliseconds % 3 - 1, _y = (time.wMilliseconds / 3) % 3 - 1;
				if(_x == 0 && _y == 0) ++_y; // 不能让两颗子重叠
				Sleep(500); 
				fc.putchess(7 + _x, 7 + _y);
				printboard(fc);
				Sleep(500);
				_playgame(fc, &ai, &ai, false);
				break;
			} // case 2
			case 3: { // 选项
				options();
				break;
			}
			case 4: { // 退出
				closegraph();
				return 0;
			}
#ifdef _DEBUG // 在DEBUG模式下左下角会有“打开编译器进行DEBUG”的选项，点击会启动我的本地文件，从而方便我进行DEBUG
			case 5: {
				ShellExecute(nullptr, _T("open"), _T("D:/文档/Visual Studio 2022/C++/Five_Chess"), 0, 0, SW_SHOWNORMAL);
				closegraph();
				return 0;
			}
#endif
		}
	}
}