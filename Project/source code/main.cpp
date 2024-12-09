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
// �������
class Human : public Player_Base {
public:
	Human() = default;
	// ������жϿ��������������
	void judge(const Five_Chess &fc) {}
	// ��ȡ�����Ϣ�����ʱ����
	void put(Five_Chess &fc) {
		while(true) {
			// ��ȡ�����Ϣ
			MOUSEMSG msg = GetMouseMsg();
			// û�е�������ʱ�򲻶ϻ�ȡ�����Ϣ
			while(msg.uMsg != WM_LBUTTONDOWN) {
				msg = GetMouseMsg();
				printboard(fc, msg);
			}
			// �����ڰ�������ĵط�����
			try {
				putchess(fc, msg);
				// �ɹ����ӣ��˳�ѭ��
				break;
			}
			// ����ʧ�ܣ���λ�ñ�ռ�ã�����ѭ��
			catch(Occupied &oc){}
		}
	}
	~Human() = default;
};
// ������Ϸ�ĺ���
// ���ĸ�����Ϊ���Ӻ��Ƿ�ȴ�500ms������Ϊ�˷�ֹAI���Ҷ���ʱ���ӹ����޷�����
void _playgame(Five_Chess &fc, Player_Base *player1, Player_Base *player2, bool dosleep = false) {
	// ������Ϸʱ��ӡ����
	printboard(fc);
	// ��ȡʤ���ߵĴ������
	char ch;
	// ����Ϸ����ǰѭ�����˲���
	while(!fc.has_ended(ch)) {
		player1->judge(fc);
		player1->put(fc);
		printboard(fc);
		// �����һ�����Ӻ���Ϸ�Ѿ���������ֱ���˳�ѭ��
		if(fc.has_ended(ch)) break;
		if(dosleep) Sleep(500);
		player2->judge(fc);
		player2->put(fc);
		printboard(fc);
		if(dosleep && !fc.has_ended(ch)) Sleep(500);
	}
	// ���㶯��
	settextcolor(RED);
	settextstyle(50, 0, _T("��������"));
	if(ch == '1') {
		outtextxy(250, 220, _T("�����ʤ��"));
	}
	else if(ch == '2') {
		outtextxy(250, 220, _T("�����ʤ��"));
	}
	else {
		outtextxy(250, 220, _T("ƽ�֣�"));
	}
	// �ȴ�1s�󷵻����˵�
	Sleep(1000);
	setlinecolor(BLACK);
}
// ʹ��WinMain�����������Ҫ���룬��ʹ��Visual Studio����C++17���ϱ�׼������������->ϵͳ->��ϵͳ��ѡ�񴰿�/WINDOWS
int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR cmd, _In_ int n_cmd) {
	// ����BGM
	mciSendString(_T("open ../resources/FiveChess.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("play bgm repeat"), NULL, 0, NULL);
	// �ȴ�BGM��ʼ���ţ���
	Sleep(1000);
	// ��ʾ��Ϸ����
	init();
	// ��Ϸѭ��
	while(true) {
		// ͨ���˵����ķ���ֵ��ȡ���ѡ��
		int choice = menu();
		switch(choice) {
			case 0: { // ������Ϸ
				int _side = side(); // ѡ�ߣ�0Ϊ���ӣ�1Ϊ���ӣ�2Ϊ����
				Five_Chess fc;
				Search_AI ai;
				Human h;
				if(_side == 2) break; // break�������˵�
				else if(_side == 1) fc.putchess(7, 7); // ���ѡ����ӣ��������м�ֱ�Ӱڷ�һ�ź���
				_playgame(fc, &h, &ai); // ������Ϸ
				break;
			} // case 0
			case 1: { // ˫����Ϸ
				Five_Chess fc;
				MOUSEMSG msg;
				Human h;
				_playgame(fc, &h, &h); // �������ҶԿ�
				break;
			} // case 1
			case 2: { // AI���Ҷ���
				// Ϊ�˸�AI�Ծ���������������ԣ����ǹ̶���һ���ӷ������룬�ڶ�������������������Χ
				Five_Chess fc;
				char ch = ' ';
				Search_AI ai;
				SYSTEMTIME time; // �Ե�ǰ��ʱ����Ϊ�����ڶ����ӵ�����λ��
				GetLocalTime(&time);
				fc.putchess(7, 7);
				printboard(fc);
				int _x = time.wMilliseconds % 3 - 1, _y = (time.wMilliseconds / 3) % 3 - 1;
				if(_x == 0 && _y == 0) ++_y; // �������������ص�
				Sleep(500); 
				fc.putchess(7 + _x, 7 + _y);
				printboard(fc);
				Sleep(500);
				_playgame(fc, &ai, &ai, false);
				break;
			} // case 2
			case 3: { // ѡ��
				options();
				break;
			}
			case 4: { // �˳�
				closegraph();
				return 0;
			}
#ifdef _DEBUG // ��DEBUGģʽ�����½ǻ��С��򿪱���������DEBUG����ѡ�����������ҵı����ļ����Ӷ������ҽ���DEBUG
			case 5: {
				ShellExecute(nullptr, _T("open"), _T("D:/�ĵ�/Visual Studio 2022/C++/Five_Chess"), 0, 0, SW_SHOWNORMAL);
				closegraph();
				return 0;
			}
#endif
		}
	}
}