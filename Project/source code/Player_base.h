#pragma once
#include "Common.h"
class Five_Chess;
// 抽象玩家基类，AI和真人玩家都是基于此类的多态
class Player_Base {
public:
	// 判断棋盘局势
	virtual void judge(const Five_Chess &fc) noexcept = 0;
	// 落子
	virtual void put(Five_Chess &fc) noexcept = 0;
	virtual ~Player_Base() = default;
};