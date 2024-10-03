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

using score_t = unsigned long long;

score_t ScoreRefToCnt(int cnt) {
	switch(cnt) {
		case 0:return 1;
		case 1:return 100;
		case 2:return 100000;
		case 3:return 1000000000;
		case 4:return 100000000000ULL;
	}
}

void Search_AI::judge(const Five_Chess& fc) {
	auto spaces = fc.generate_possible_moves();
	judgement = vector<vector<score_t>>(15, vector<score_t>(15, 1));
	for(auto &space : spaces) {
		auto x = space.first, y = space.second;
		for(int i = 0; i < 4; ++i) {
			int dx = i % 3 - 1, dy = i / 3 - 1;
			int leftcnt = 1, rightcnt = 1, lefthasblock = 0, righthasblock = 0;
			if(x + dx < 0 || y + dy < 0 || x + dx > 14 || y + dy > 14) rightcnt = 0;
			else if(fc.chessboard[x + dx][y + dy] == ' ') rightcnt = 0;
			else for(int j = 1; i < 5; ++j) {
				int nx = x + dx * j, ny = y + dy * j;
				if(nx >= 0 && nx <= 14 && ny >= 0 && ny <= 14 && nx + dx >= 0 && nx + dx <= 14 && ny + dy >= 0 && ny + dy <= 14) {
					if(fc.chessboard[nx][ny] == fc.chessboard[nx + dx][ny + dy] && fc.chessboard[nx][ny] != ' ') {
						++rightcnt;
					}
					else if(fc.chessboard[nx + dx][ny + dy] != ' ') {
						righthasblock = 1;
						break;
					}
					else break;
				}
				else break;
			}
			if(x - dx < 0 || y - dy < 0 || x - dx > 14 || y - dy > 14) leftcnt = 0;
			else if(fc.chessboard[x - dx][y - dy] == ' ') leftcnt = 0;
			else for(int j = 1; j < 5; ++j) {
				int nx2 = x - dx * j, ny2 = y - dy * j;
				if(nx2 > 0 && nx2 < 15 && ny2 > 0 && ny2 < 15 && nx2 - dx > 0 && nx2 - dx < 15 && ny2 - dy > 0 && ny2 - dy < 15) {
					if(fc.chessboard[nx2][ny2] == fc.chessboard[nx2 - dx][ny2 - dy] && fc.chessboard[nx2][ny2] != ' ') {
						++leftcnt;
					}
					else if(fc.chessboard[nx2 - dx][ny2 - dy] != ' ') {
						lefthasblock = 1;
						break;
					}
					else break;
				}
				else break;
			}
			judgement[x][y] += (lefthasblock ? 0.4 : 1) * ScoreRefToCnt(leftcnt);
			judgement[x][y] += (righthasblock ? 0.4 : 1) * ScoreRefToCnt(rightcnt);
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
	if(hash.size() == 1) {
		for(auto &elem : hash) {
			a = elem.first;
			b = elem.second;
			fc.putchess(a, b);
			return;
		}
	}
	else for(auto &elem : hash) {
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