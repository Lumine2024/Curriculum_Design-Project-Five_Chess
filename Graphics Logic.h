#pragma once
#include<Windows.h>
#include<graphics.h>
#include"Game Logic.h"

class Occupied {
public:
	Occupied();
};
void init();
int menu();
const MOUSEMSG no_msg = { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
void printboard(const Five_Chess &game, const MOUSEMSG& msg = no_msg);
void putchess(Five_Chess &game, MOUSEMSG& msg);
void options(int &aichoice);
bool side();