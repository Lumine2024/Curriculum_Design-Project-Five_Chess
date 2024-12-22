#pragma once
#include "Common.h"
#include "Game Logic.h"
#include <graphics.h>
// ���Ե�ö����
enum Language {
	Chinese = 0,
	English = 1
};
// ���е�ȫ�ֱ�������ͷ�ļ��ж���Ҫextern
extern Language language;
// ��ӡ��ʾ��Ϣ
RELEASE_INLINE void printhint(int x, int y, const TCHAR *str);
// ��ռ��ʱ�׳����쳣��
class Occupied {
public:
	// ����ʱ������ʾ
	Occupied() noexcept;
};
// ��ʼ������
void init() noexcept;
// ��ʼ�˵�
int menu() noexcept;
const MOUSEMSG no_msg = { 0, 0, 0, 0, 0, 0, 0, 0, 0 }; // ��������Ͻǵ������Ϣ
// ��ʾ����
void printboard(const Five_Chess &game, const MOUSEMSG &msg = no_msg) noexcept;
// ѡ�����
void options() noexcept;
// ѡ��
int side() noexcept;