#pragma once
#include<vector>
#include<xutility>
#include"Game Logic.h"
#include"Player_base.h"
#include<cmath>
// ËÑË÷AIÀà
class Search_AI : public Player_Base {
public:
	std::vector<std::vector<unsigned long long>> judgement;
	unsigned long long max_of_judgement;
	int maxdepth;
	Search_AI(int md = 14) : maxdepth(md), max_of_judgement(0ULL) {}
	void judge(const Five_Chess &fc);
	unsigned long long put(Five_Chess& fc, int depth);
	void put(Five_Chess &fc) { put(fc, 0); }
	~Search_AI() = default;
};