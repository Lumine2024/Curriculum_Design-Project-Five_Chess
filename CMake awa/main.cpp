#include"Graphics Logic.h"
#include"Game Logic.h"
#include"Search_AI.h"
#pragma comment(lib,"winmm.lib")
extern HWND game;
using namespace std;
int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	mciSendString(_T("open FiveChess.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("play bgm repeat"), NULL, 0, NULL);
	Sleep(1000);
	init();
	while(true) {
		int choice = menu();
		switch(choice) {
			case 0: {
				bool _myside = side();
				Five_Chess fc;
				MOUSEMSG msg;
				bool clicked = true;
				char ch = ' ';
				printboard(fc);
				Search_AI ai;
				if(_myside) {
					fc.putchess(7, 7);
					printboard(fc, msg);
					clicked = true;
					while(!fc.has_ended(ch)) {
						msg = GetMouseMsg();
						if(msg.uMsg == WM_LBUTTONDOWN && clicked == false) {
							putchess(fc, msg);
							printboard(fc, msg);
							if(fc.has_ended(ch)) break;
							ai.judge(fc);
							ai.put(fc);
							printboard(fc, msg);
							clicked = true;
						}
						else if(msg.uMsg != WM_LBUTTONDOWN) {
							clicked = false;
							printboard(fc, msg);
						}
						else {
							printboard(fc, msg);
						}
					}
					settextcolor(RED);
					settextstyle(50, 0, _T("Á¼»³ÐÐÊé"));
					if(ch == '1') {
						outtextxy(250, 220, _T("°×Æå»ñÊ¤£¡"));
					}
					else {
						outtextxy(250, 220, _T("ºÚÆå»ñÊ¤£¡"));
					}
				}
				else {
					while(!fc.has_ended(ch)) {
						msg = GetMouseMsg();
						if(msg.uMsg == WM_LBUTTONDOWN && clicked == false) {
							putchess(fc, msg);
							printboard(fc, msg);
							if(fc.has_ended(ch)) break;
							ai.judge(fc);
							ai.put(fc);
							printboard(fc, msg);
							clicked = true;
						}
						else if(msg.uMsg != WM_LBUTTONDOWN) {
							clicked = false;
							printboard(fc, msg);
						}
						else {
							printboard(fc, msg);
						}
					}
					settextcolor(RED);
					settextstyle(50, 0, _T("Á¼»³ÐÐÊé"));
					if(ch == '1') {
						outtextxy(250, 220, _T("°×Æå»ñÊ¤£¡"));
					}
					else {
						outtextxy(250, 220, _T("ºÚÆå»ñÊ¤£¡"));
					}
				}
				Sleep(1000);
				setlinecolor(BLACK);
				break;
			} // case 0
			case 1: {
				Five_Chess fc;
				MOUSEMSG msg;
				bool clicked = true;
				char ch = ' ';
				printboard(fc);
				while(!fc.has_ended(ch)) {
					msg = GetMouseMsg();
					if(msg.uMsg == WM_LBUTTONDOWN && clicked == false) {
						putchess(fc, msg);
						clicked = true;
					}
					else if(msg.uMsg != WM_LBUTTONDOWN) {
						clicked = false;
						printboard(fc, msg);
					}
					else {
						printboard(fc, msg);
					}
					Sleep(4);
				}
				settextcolor(RED);
				settextstyle(50, 0, _T("Á¼»³ÐÐÊé"));
				if(ch == '1') {
					outtextxy(250, 220, _T("°×Æå»ñÊ¤£¡"));
				}
				else {
					outtextxy(250, 220, _T("ºÚÆå»ñÊ¤£¡"));
				}
				Sleep(1000);
				setlinecolor(BLACK);
				break;
			} // case 1
			case 2: {
				options();
				break;
			}
			case 3: {
				closegraph();
				return 0;
			}
		}
	}
}