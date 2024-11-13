#include "Search_AI.h"
#include<unordered_map>
using namespace std;
using score_t = unsigned long long;
score_t ScoreRefToCnt(int cnt) {
	switch(cnt) {
		case 0:return 1;
		case 1:return 10;
		case 2:return 10000;
		case 3:return 1000000ULL;
		case 4:return 100000000000ULL;
		default:__assume(false);
	}
}
void Search_AI::judge(const Five_Chess& fc) {
	max_of_judgement = 0;
	auto spaces = fc.generate_possible_moves();
	judgement = vector<vector<score_t>>(15, vector<score_t>(15, 0));
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
				else {
					righthasblock = 1;
					break;
				}
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
				else {
					lefthasblock = 1;
					break;
				}
			}
			judgement[x][y] += (leftcnt == 0 ? 1 : (lefthasblock ? 0.4 : 1) * ScoreRefToCnt(leftcnt)) * (rightcnt == 0 ? 1 : (righthasblock ? 0.4 : 1) * ScoreRefToCnt(rightcnt));
		}
		max_of_judgement = max(max_of_judgement, judgement[x][y]);
	}
}
unsigned long long Search_AI::put(Five_Chess &fc, int depth) {
	judge(fc);
	unordered_map<int, int> ht;
	if(depth >= maxdepth) {
		score_t best_score = 0ULL;
		for(int i = 0; i < 15; ++i) {
			for(int j = 0; j < 15; ++j) {
				if(judgement[i][j] == max_of_judgement) {
					fc.putchess(i, j);
					judge(fc);
					best_score = max(best_score, max_of_judgement);
					fc.rmchess(i, j);
				}
			}
		}
		return best_score;
	}
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 15; ++j) {
			if(judgement[i][j] == max_of_judgement) {
				ht[i] = j;
			}
		}
	}
	if(ht.size() == 1) {
		auto [i, j] = *(ht.begin());
		fc.putchess(i, j);
		judge(fc);
		auto ret = max_of_judgement;
		if(depth != 0) fc.rmchess(i, j);
		return ret;
	}
	int _x = -1, _y = -1;
	unsigned long long maxn = 0;
	for(auto hashes : ht) {
		auto [i, j] = hashes;
		fc.putchess(i, j);
		judge(fc);
		auto retthis = put(fc, depth + 1);
		if(maxn < retthis) {
			maxn = retthis;
			_x = i, _y = j;
		}
		fc.rmchess(i, j);
	}
	if(depth == 0) {
		fc.putchess(_x, _y);
		return static_cast<unsigned long long>(-1);
	}
	else {
		return maxn;
	}
}