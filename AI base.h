#pragma once
class Five_Chess;
class AI_Base {
public:
	virtual void judge(const Five_Chess &fc) = 0;
	virtual void put(Five_Chess &fc) = 0;
	virtual ~AI_Base() = default;
};