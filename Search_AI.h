#pragma once
#ifndef SEARCH_AI_H
#define SEARCH_AI_H
#include<vector>
#include<xutility>
#include"Game Logic.h"
#include<cmath>

class Search_AI {
	std::vector<std::vector<unsigned long long>> judgement;
public:
	void judge(const Five_Chess &fc);
	void put(Five_Chess& fc);
};
#endif // SEARCH_AI_H