#include "Search_AI.h"
#include<unordered_map>
using namespace std;
struct VectorCharHash {
	std::size_t operator()(const std::vector<char> &v) const {
		std::size_t hash = 0;
		for(char c : v) {
			hash ^= std::hash<char>{}(c) +0x9e3779b9 + (hash << 6) + (hash >> 2);
		}
		return hash;
	}
};
struct VectorCharEqual {
	bool operator()(const std::vector<char> &v1, const std::vector<char> &v2) const {
		return v1 == v2;
	}
};
const unordered_map<vector<char>, size_t, VectorCharHash, VectorCharEqual> mscores() {
	std::unordered_map<std::vector<char>, size_t, VectorCharHash, VectorCharEqual> scores;
	scores[{'2', '2', '2', '2', '2' }] = 100000000;
	scores[{'2', '2', '2', '2', ' ' }] = 1000000;
	scores[{'2', '2', '2', ' ', '2' }] = 1000000;
	scores[{'2', '2', ' ', '2', '2' }] = 1000000;
	scores[{'2', ' ', '2', '2', '2' }] = 1000000;
	scores[{' ', '2', '2', '2', '2' }] = 1000000;
	scores[{'2', '2', '2', ' ', ' ' }] = 10000;
	scores[{'2', '2', '2', ' ', '1' }] = 8000;
	scores[{'2', '2', ' ', '2', ' ' }] = 10000;
	scores[{'2', '2', ' ', ' ', '2' }] = 9000;
	scores[{'2', ' ', '2', '2', ' ' }] = 9000;
	scores[{'2', ' ', '2', ' ', '2' }] = 9000;
	scores[{'2', ' ', ' ', '2', '2' }] = 9000;
	scores[{' ', '2', '2', ' ', '2' }] = 9000;
	scores[{' ', '2', '2', '2', ' ' }] = 10000;
	scores[{' ', '2', ' ', '2', '2' }] = 9000;
	scores[{' ', ' ', '2', '2', '2' }] = 1000;
	scores[{'1', ' ', '2', '2', '2' }] = 3800;
	scores[{'2', ' ', ' ', ' ', '2' }] = 280;
	scores[{'2', ' ', ' ', '2', ' ' }] = 150;
	scores[{'2', ' ', '2', ' ', ' ' }] = 400;
	scores[{' ', '2', ' ', ' ', '2' }] = 150;
	scores[{' ', '2', ' ', '2', ' ' }] = 400;
	scores[{' ', ' ', '2', ' ', '2' }] = 300;
	scores[{'2', '2', ' ', ' ', ' ' }] = 1000;
	scores[{' ', '2', '2', ' ', ' ' }] = 500;
	scores[{' ', ' ', '2', '2', ' ' }] = 200;
	scores[{' ', ' ', ' ', '2', '2' }] = 100;
	scores[{'2', ' ', ' ', ' ', ' ' }] = 10;
	scores[{' ', '2', ' ', ' ', ' ' }] = 8;
	scores[{' ', ' ', '2', ' ', ' ' }] = 6;
	scores[{' ', ' ', ' ', '2', ' ' }] = 4;
	scores[{' ', ' ', ' ', ' ', '2' }] = 2;
	scores[{' ', ' ', ' ', ' ', ' ' }] = 1;
	scores[{'1', '1', '1', '1', '1' }] = 100000000;
	scores[{'1', '1', '1', '1', ' ' }] = 1000000;
	scores[{'1', '1', '1', ' ', '1' }] = 1000000;
	scores[{'1', '1', ' ', '1', '1' }] = 1000000;
	scores[{'1', ' ', '1', '1', '1' }] = 1000000;
	scores[{' ', '1', '1', '1', '1' }] = 1000000;
	scores[{'1', '1', '1', ' ', ' ' }] = 10000;
	scores[{'1', '1', '1', ' ', '2' }] = 8000;
	scores[{'1', '1', ' ', '1', ' ' }] = 10000;
	scores[{'1', '1', ' ', ' ', '1' }] = 9000;
	scores[{'1', ' ', '1', '1', ' ' }] = 9000;
	scores[{'1', ' ', '1', ' ', '1' }] = 9000;
	scores[{'1', ' ', ' ', '1', '1' }] = 9000;
	scores[{' ', '1', '1', ' ', '1' }] = 9000;
	scores[{' ', '1', '1', '1', ' ' }] = 10000;
	scores[{' ', '1', ' ', '1', '1' }] = 9000;
	scores[{' ', ' ', '1', '1', '1' }] = 1000;
	scores[{'2', ' ', '1', '1', '1' }] = 3800;
	scores[{'1', ' ', ' ', ' ', '1' }] = 280;
	scores[{'1', ' ', ' ', '1', ' ' }] = 350;
	scores[{'1', ' ', '1', ' ', ' ' }] = 400;
	scores[{' ', '1', ' ', ' ', '1' }] = 150;
	scores[{' ', '1', ' ', '1', ' ' }] = 400;
	scores[{' ', ' ', '1', ' ', '1' }] = 300;
	scores[{'1', '1', ' ', ' ', ' ' }] = 1000;
	scores[{' ', '1', '1', ' ', ' ' }] = 500;
	scores[{' ', ' ', '1', '1', ' ' }] = 200;
	scores[{' ', ' ', ' ', '1', '1' }] = 100;
	scores[{'1', ' ', ' ', ' ', ' ' }] = 10;
	scores[{' ', '1', ' ', ' ', ' ' }] = 8;
	scores[{' ', ' ', '1', ' ', ' ' }] = 6;
	scores[{' ', ' ', ' ', '1', ' ' }] = 4;
	scores[{' ', ' ', ' ', ' ', '1' }] = 2;
	return scores;
}

void Search_AI::judge(const Five_Chess& fc) {
	judgement = vector<vector<unsigned long long>>(15, vector<unsigned long long>(15, 0));
	auto scores = mscores();
	auto spaces = fc.generate_possible_moves();
	for(auto &space : spaces) {
		int x = space.first, y = space.second;
		for(int i = 0; i < 9; ++i) {
			int dx = i % 3 - 1, dy = i / 3 - 1;
			if(dx == 0 && dy == 0) continue;
			vector<char> v;
			for(int j = 0; j < 5; ++j) {
				int nx = x + dx * j, ny = y + dy * j;
				if(nx >= 0 && nx < 15 && ny >= 0 && ny < 15) {
					v.push_back(fc.chessboard[nx][ny]);
				}
				else {
					v.push_back(' ');
				}
			}
			judgement[x][y] += scores[v];
		}
	}
}

void Search_AI::put(Five_Chess& fc) {
	int a = -1, b = -1;
	unsigned long long maxn = 0;
	unordered_map<int, int> hash;
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 15; ++j) {
			maxn = max(maxn, judgement[i][j]);
		}
	}
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 15; ++j) {
			if(judgement[i][j] == maxn) {
				hash[i] = j;
			}
		}
	}
	for(auto &elem : hash) {
		a = elem.first;
		b = elem.second;
		try {
			fc.putchess(a, b);
		}
		catch(...) {
			continue;
		}
		judge(fc);
		for(int i = 0; i < 15; ++i) {
			for(int j = 0; j < 15; ++j) {
				if(judgement[i][j] > maxn) {
					maxn = judgement[i][j];
				}
			}
		}
		for(int i = 0; i < 15; ++i) {
			for(int j = 0; j < 15; ++j) {
				if(judgement[i][j] == maxn) {
					return;
				}
			}
		}
		fc.rmchess(a, b);
		judge(fc);
	}
	fc.putchess(a, b);
}