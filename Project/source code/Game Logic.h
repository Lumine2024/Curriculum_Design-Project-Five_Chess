#pragma once
#include<algorithm>
#include<utility>
#include<vector>
struct MOUSEMSG;
class Search_AI;
// 五子棋棋盘类
class Five_Chess {
public:
	// 棋盘
	std::vector<std::vector<char>> chessboard;
	// 是否为第一个玩家正在打
	bool is_index_0_player_playing;
	Five_Chess() : chessboard(15, std::vector<char>(15, ' ')), is_index_0_player_playing(false) {}
	Five_Chess(const Five_Chess &other) : chessboard(other.chessboard), is_index_0_player_playing(other.is_index_0_player_playing) {}
	Five_Chess &operator=(const Five_Chess &other) {
		if(this != &other) {
			chessboard = other.chessboard;
			is_index_0_player_playing = other.is_index_0_player_playing;
		}
		return *this;
	}
	// 落子
	void putchess(int x, int y);
	// 移除棋子
	void rmchess(int x, int y);
	// 判定游戏是否结束，ch传入以获取胜利玩家
	bool has_ended(char &ch);
	// 生成所有可能的落子处
	std::vector<std::pair<int, int>> generate_possible_moves() const;
	// 返回正在玩的玩家
	bool player_playing() const { return is_index_0_player_playing; }
};