#pragma once
class Five_Chess;
// ���󷽷���һ��࣬AI��������Ҷ��ǻ��ڴ���Ķ�̬
class Player_Base {
public:
	// �ж����̾���
	virtual void judge(const Five_Chess &fc) = 0;
	// ����
	virtual void put(Five_Chess &fc) = 0;
	virtual ~Player_Base() = default;
};