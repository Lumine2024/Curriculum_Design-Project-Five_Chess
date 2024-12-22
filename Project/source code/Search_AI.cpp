#include "Search_AI.h"
#include<unordered_map>
#ifdef _DEBUG
#include <iostream>
#include <iomanip>
// 分隔符
constexpr char separator[] = "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n";
#endif
using namespace std;
#pragma warning(disable: 4244) // double转score_t可能会有精度损失，但是这里不需要精度
// 棋盘类型对应的得分估值函数
inline Search_AI::score_t ScoreRefToCnt(int cnt) {
	switch(cnt) {
		case 0:return 1;
		case 1:return 100;
		case 2:return 100000;
		case 3:return 100000000ULL;
		case 4:return 10000000000000ULL;
		default:__assume(false); // 每个子只扫4格，cnt不可能比4大，更不可能比0小
	}
}

void Search_AI::judge(const Five_Chess &fc) noexcept {
	judgement = vector<vector<score_t>>(15, vector<score_t>(15, 0)); // 初始化判断
	max_of_judgement = 0; // 初始化最大值
	auto spaces = fc.generate_possible_moves(); // 获取所有的空格
	for(auto &[x, y] : spaces) { // 对每一个空格进行估值
		// 对水平、竖直、斜向左上、斜向右上四个方向的分数进行计算，将结果相加
		for(int i = 0; i < 4; ++i) {
			int dx = i % 3 - 1, dy = i / 3 - 1; // 计算方向
			int leftcnt = 1, rightcnt = 1, lefthasblock = 0, righthasblock = 0; // 计算相连棋子的个数以及是否被遮挡
			char leftrecord = ' ', rightrecord = ' '; // 记录左右两边的棋子
			if(x + dx < 0 || y + dy < 0 || x + dx > 14 || y + dy > 14) rightcnt = 0; // 如果靠边，则必有一边为0
			else if(fc.chessboard[x + dx][y + dy] == ' ') rightcnt = 0; // 相连的一格是空的
			else for(int j = 1; j < 5; ++j) { // 扫4格
				int nx = x + dx * j, ny = y + dy * j; // 计算偏移量
				if(nx >= 0 && nx <= 14 && ny >= 0 && ny <= 14 &&
					nx + dx >= 0 && nx + dx <= 14 && ny + dy >= 0 && ny + dy <= 14) { // 保证不越界
					if(fc.chessboard[nx][ny] == fc.chessboard[nx + dx][ny + dy] && fc.chessboard[nx][ny] != ' ') {
						rightrecord = fc.chessboard[nx][ny];
						++rightcnt; // 相同，说明连续
					}
					else if(fc.chessboard[nx + dx][ny + dy] != ' ') {
						righthasblock = 1; // 不相同，说明封路
						break;
					}
					else break; // 空，退出
				}
				// 如果越界，外面是下不了的
				else {
					righthasblock = 1;
					break;
				}
			}
			// 同上逻辑
			if(x - dx < 0 || y - dy < 0 || x - dx > 14 || y - dy > 14) leftcnt = 0;
			else if(fc.chessboard[x - dx][y - dy] == ' ') leftcnt = 0;
			else for(int j = 1; j < 5; ++j) {
				int nx2 = x - dx * j, ny2 = y - dy * j;
				if(nx2 > 0 && nx2 < 15 && ny2 > 0 && ny2 < 15 &&
					nx2 - dx > 0 && nx2 - dx < 15 && ny2 - dy > 0 && ny2 - dy < 15) {
					if(fc.chessboard[nx2][ny2] == fc.chessboard[nx2 - dx][ny2 - dy] &&
						fc.chessboard[nx2][ny2] != ' ') {
						leftrecord = fc.chessboard[nx2][ny2];
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
			// 计算该点的得分
			// 如果两边都有封路且总数小于4，这里不可能连成五子连珠，直接跳过
			if(lefthasblock && righthasblock && leftcnt + rightcnt < 4) {
				continue;
			}
			// 如果两边均不为空且不相等，且每一边均小于4，这里不可能连成五子连珠，直接跳过
			else if(leftrecord != ' ' && rightrecord != ' ' &&
				leftrecord != rightrecord && leftcnt < 4 && rightcnt < 4) {
				continue;
			}
			else if(leftcnt + rightcnt >= 4 && leftrecord == rightrecord) {
				judgement[x][y] += 1000000000000000ULL; // 下一步落子到此处必胜
			}
			else {
				judgement[x][y] +=
					(lefthasblock ? 0.1 : 1.) * ScoreRefToCnt(leftcnt) *
					(righthasblock ? 0.1 : 1.) * ScoreRefToCnt(rightcnt);
			}
		}
		// 更新最大值
		max_of_judgement = max(max_of_judgement, judgement[x][y]);
	}
}

Search_AI::score_t Search_AI::put(Five_Chess &fc, int depth) noexcept {
	// 先判断
	judge(fc);
	unordered_map<int, int> ht;
	// 递归到最深处，直接返回
	if(depth >= maxdepth) {
		return max_of_judgement;
	}
	// 当最大值为0时，说明游戏必为平局
	if(max_of_judgement == 0) {
		auto [x, y] = fc.generate_possible_moves()[0];
		fc.putchess(x, y);
		return 0ULL;
	}
	else {
		// 获取分数最大的所有点
		for(int i = 0; i < 15; ++i) {
			for(int j = 0; j < 15; ++j) {
				if(judgement[i][j] == max_of_judgement) {
					ht[i] = j;
				}
			}
		}
		// 如果最大的点只有一个，直接返回
		if(ht.size() == 1) {
			auto [i, j] = *(ht.begin());
			fc.putchess(i, j);
			// 特判：游戏是否结束
			char ch;
			if(fc.has_ended(ch)) {
				if(depth != 0) fc.rmchess(i, j); // 只有当深度不为0时才需要移除该棋子
				return static_cast<score_t>(-1);
			}
			judge(fc);
			auto ret = max_of_judgement;
			if(depth != 0) fc.rmchess(i, j);
			else return static_cast<score_t>(-1);
			return ret;
		}
		int _x = -1, _y = -1;
		unsigned long long maxn = 0;
		// 对于每一个最大的点，递归搜索在继续落子的情况下，对应的最大的点
		for(auto hashes : ht) {
			auto [i, j] = hashes;
			fc.putchess(i, j);
			char ch;
			if(fc.has_ended(ch)) {
				if(depth != 0) {
					fc.rmchess(i, j);
				}
				return static_cast<score_t>(-1);
			}
			judge(fc);
			auto retthis = put(fc, depth + 1);
			if(maxn < retthis) {
				maxn = retthis;
				_x = i, _y = j;
			}
			fc.rmchess(i, j);
		}
		// 如果深度为0，在最大点落子
		if(depth == 0) {
			fc.putchess(_x, _y);
			return static_cast<score_t>(-1);
		}
		else {
			return maxn;
		}
	}
}

void Search_AI::put(Five_Chess &fc) noexcept {
	put(fc, 0);
#ifdef _DEBUG
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 15; ++j) {
			cout << setw(16) << judgement[j][i] << "\t";
		}
		cout << "\n\n";
	}
	cout << separator;
#endif
}