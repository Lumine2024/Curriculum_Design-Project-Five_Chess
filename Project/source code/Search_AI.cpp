#include "Search_AI.h"
#include <unordered_map>
#ifdef _DEBUG
#include <iostream>
#include <iomanip>
// 分隔符
constexpr char separator[] = "--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n\n";
#endif
using namespace std;
#pragma warning(disable: 4244) // double转score_t可能会有精度损失，但是这里不需要精度

constexpr Search_AI::score_t score_ref[5] = { 1, 100, 100000, 100000000ULL, 10000000000000ULL }; // 棋盘类型对应的得分估值数组
constexpr int directions[4][2] = { {1, 0}, {0, 1}, {1, 1}, {1, -1} }; // 方向数组
void Search_AI::judge(const Five_Chess &fc) noexcept {
	judgement = vector<vector<score_t>>(15, vector<score_t>(15, 0)); // 初始化判断
	max_of_judgement = 0; // 初始化最大值
	auto spaces = fc.generate_possible_moves(); // 获取所有的空格
	for(auto &[x, y] : spaces) { // 对每一个空格的得分进行估值
		for(int i = 0; i < 4; ++i) {
			int dx = directions[i][0], dy = directions[i][1]; // 计算方向
			int leftcnt = 0, rightcnt = 0, lefthasblock = 0, righthasblock = 0; // 计算相连棋子的个数以及是否被遮挡
			char leftrecord = ' ', rightrecord = ' '; // 记录左右两边的棋子
			if(x + dx < 0 || y + dy < 0 || x + dx > 14 || y + dy > 14) { // 如果越界，直接认为没有相连的棋子
				rightcnt = 0;
				righthasblock = 1;
			}
			else if(fc.chessboard[x + dx][y + dy] == ' ') rightcnt = 0; // 相连的一格是空的
			else {
				rightrecord = fc.chessboard[x + dx][y + dy]; // 记录相连的一格
				for(int j = 1; j < 5; ++j) { // 扫4格
					int nx = x + dx * j, ny = y + dy * j; // 计算偏移量
					if(nx >= 0 && nx <= 14 && ny >= 0 && ny <= 14) { // 保证不越界
						if(fc.chessboard[nx][ny] == rightrecord) {
							rightcnt = j; // 相同，计数
						}
						else if(fc.chessboard[nx][ny] != ' ') {
							righthasblock = 1; // 不相同，说明封路
							break;
						}
						else break; // 空，退出
					}
					else { // 如果越界，外面是下不了的，认为有封路
						righthasblock = 1;
						break;
					}
				}
			}
			// 同上逻辑
			if(x - dx < 0 || y - dy < 0 || x - dx > 14 || y - dy > 14) {
				leftcnt = 0;
				lefthasblock = 1;
			}
			else if(fc.chessboard[x - dx][y - dy] == ' ') leftcnt = 0;
			else {
				leftrecord = fc.chessboard[x - dx][y - dy];
				for(int j = 1; j < 5; ++j) {
					int nx2 = x - dx * j, ny2 = y - dy * j;
					if(nx2 >= 0 && nx2 <= 14 && ny2 >= 0 && ny2 <= 14) {
						if(fc.chessboard[nx2][ny2] == leftrecord) {
							leftcnt = j;
						}
						else if(fc.chessboard[nx2][ny2] != ' ') {
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
			}
			// 计算该点的得分
			// 如果两边相等，都有封路且总数小于4，这里不可能连成五子连珠，直接跳过
			if((leftrecord == rightrecord) && lefthasblock && righthasblock && (leftcnt + rightcnt < 4)) {
				continue;
			}
			// 如果两边有一边越界，且另一边小于4，这里不可能连成五子连珠，直接跳过
			else if((leftrecord == ' ') ^ (rightrecord == ' ') && (leftcnt < 4) && (rightcnt < 4) &&
				lefthasblock && righthasblock) {
				continue;
			}
			// 如果两边均不为空且不相等，且每一边均小于4，这里不可能连成五子连珠，直接跳过
			else if((leftrecord != ' ') && (rightrecord != ' ') &&
				(leftrecord != rightrecord) && (leftcnt < 4) && (rightcnt < 4)) {
				continue;
			}
			else {
				judgement[x][y] +=
					(lefthasblock	? 0.1 : 1.)	* score_ref[leftcnt] *
					(righthasblock	? 0.1 : 1.)	* score_ref[rightcnt];
			}
		}
		// 更新最大值
		max_of_judgement = max(max_of_judgement, judgement[x][y]);
	}
}

Search_AI::score_t Search_AI::put(Five_Chess &fc, int depth) noexcept {
	// 先判断
	judge(fc);
	// 保存最大值的点
	vector<pair<int, int>> maxs;
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
					maxs.emplace_back(i, j);
				}
			}
		}
		// 如果最大的点只有一个，直接返回
		if(maxs.size() == 1) {
			auto [i, j] = maxs[0];
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
		for(auto [i, j] : maxs) {
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
#ifdef _DEBUG
	// 在DEBUG模式下打印judgement
	for(int i = 0; i < 15; ++i) {
		for(int j = 0; j < 15; ++j) {
			cout << setw(16) << judgement[j][i] << "\t";
		}
		cout << "\n\n";
	}
	cout << separator;
#endif
	put(fc, 0);
}