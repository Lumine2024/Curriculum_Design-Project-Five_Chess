#pragma once
#include<Windows.h>
#include<graphics.h>
#include"Game Logic.h"
// ��ռ��ʱ�������쳣��
class Occupied {
public:
	// ����ʱ������ʾ
	Occupied();
};
// ��ʼ������
void init();
// ��ʼ�˵�
int menu();
const MOUSEMSG no_msg = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // ��������Ͻǵ������Ϣ
// ��ʾ����
void printboard(const Five_Chess &game, const MOUSEMSG& msg = no_msg);
// �����������
void putchess(Five_Chess &game, MOUSEMSG& msg);
// ѡ�����
void options();
// ѡ��
int side();