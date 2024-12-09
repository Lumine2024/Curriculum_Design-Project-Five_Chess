#pragma once
#include<Windows.h>
#include<graphics.h>
#include"Game Logic.h"
// 被占据时丢出的异常类
class Occupied {
public:
	// 构造时发出提示
	Occupied();
};
// 初始化操作
void init();
// 初始菜单
int menu();
const MOUSEMSG no_msg = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // 鼠标在左上角的鼠标消息
// 显示棋盘
void printboard(const Five_Chess &game, const MOUSEMSG& msg = no_msg);
// 人类操作落子
void putchess(Five_Chess &game, MOUSEMSG& msg);
// 选项界面
void options();
// 选边
int side();